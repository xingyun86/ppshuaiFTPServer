
/********************************************************************/
/*																	*/
/*  CONNECTSOCKET.CPP												*/
/*																	*/
/*  Implementation of the Connect Socket.							*/
/*	This class is a part of the ConnectThread which handles			*/
/*  socket connections. Incomming data is processed in OnReceive	*/
/*																	*/
/*  Programmed by xingyun86								*/
/*	http://www.ppsbbs.tech									*/
/*																	*/
/*  Last updated: 10 july 2002										*/
/*																	*/
/********************************************************************/

#include "stdafx.h"
#include "FTPServerApp.h"
#include "FTPServer.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"
#include "ApplicationDlg.h"
#include "DataSocket.h"

extern CFTPServer theServer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/********************************************************************/
/*																	*/
/* Function name : CConnectSocket::CConnectSocket					*/
/* Description   : Constructor										*/
/*																	*/
/********************************************************************/
CConnectSocket::CConnectSocket()
{
	m_bLoggedon = FALSE;
	m_bRenameFile = FALSE;

	// reset transfer status
	m_TransferStatus.m_pDataSocket = NULL;
	m_TransferStatus.m_strRemoteHost = "";
	m_TransferStatus.m_nRemotePort = -1;
	m_TransferStatus.m_bPassiveMode = FALSE;
	m_TransferStatus.m_nRest = -1;
}


/********************************************************************/
/*																	*/
/* Function name : CConnectSocket::~CConnectSocket					*/
/* Description   : Destructor										*/
/*																	*/
/********************************************************************/
CConnectSocket::~CConnectSocket()
{
	DestroyDataSocket();

	// tell our thread we have been closed
	AfxGetThread()->PostThreadMessage(WM_QUIT,0,0);

	TRACE0("CConnectSocket destroyed.\n");
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CConnectSocket, CSocket)
	//{{AFX_MSG_MAP(CConnectSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


/********************************************************************/
/*																	*/
/* Function name : OnClose											*/		
/* Description   : Send WM_QUIT message to the thread containing	*/
/*				   the socket to shutdown once the connection is	*/
/*                 closed.											*/
/*																	*/
/********************************************************************/
void CConnectSocket::OnClose(int nErrorCode) 
{
	Close();
	// destroy connection
	m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
	TRACE("CConnectSocket() OnClose()\n");

	CSocket::OnClose(nErrorCode);
}


#define BUFFERSIZE 4096

/********************************************************************/
/*																	*/
/* Function name : OnReceive										*/		
/* Description   : Called by the framework to notify this socket	*/
/*                 that there is data in the buffer.				*/
/*																	*/
/********************************************************************/
void CConnectSocket::OnReceive(int nErrorCode) 
{
	TCHAR buff[BUFFERSIZE];

	int nRead = Receive(buff, BUFFERSIZE);
	switch (nRead)
	{
		case 0:
			Close();
			break;

		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				TCHAR szError[256];
				wsprintf(szError, "OnReceive error: %d", GetLastError());
				
				AfxMessageBox (szError);
			}
			break;

		default:
			if (nRead != SOCKET_ERROR && nRead != 0)
			{
				((CConnectThread *)AfxGetThread())->IncReceivedBytes(nRead);

				// terminate the string
				buff[nRead] = 0; 
				m_RxBuffer += CString(buff);
				
				GetRxLine();
			}	
			break;
	}
	CSocket::OnReceive(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name: GetRxCommand										*/		
/* Description  : Get command from receiver buffer.					*/
/*																	*/
/********************************************************************/
BOOL CConnectSocket::GetRxCommand(CString &strCommand, CString &strArguments)
{
	if (!m_strCommands.IsEmpty())
	{
		CString strBuff = m_strCommands.RemoveHead();
		int nIndex = strBuff.Find(" ");
		if (nIndex != -1)
		{
			CString strPassword = strBuff;
			strPassword.MakeUpper();
			// make password invisible
			if (strPassword.Left(5) == "PASS ")
			{
				strPassword = strBuff.Left(5);
				for (int i=0; i<strBuff.GetLength()-5; i++)
					strPassword += "*";
				FireStatusMessage(strPassword, 1);
			}
			else
			{
				FireStatusMessage(strBuff, 1);
			}
			strCommand = strBuff.Left(nIndex);
			strArguments = strBuff.Mid(nIndex+1);
		}
		else
		{
			FireStatusMessage(strBuff, 1);
			strCommand = strBuff;
		}

		if (strCommand != "")
		{
			strCommand.MakeUpper();

			// who screwed up ???
			if (strCommand.Right(4) == "ABOR")
			{
				strCommand = "ABOR";
			}
			
			TRACE2("COMMAND: %s, ARGS: %s\n", strCommand, strArguments);
			return TRUE;
		}
	}
	return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name: GetRxLine											*/		
/* Description  : Parse complete command line						*/
/*																	*/
/********************************************************************/
void CConnectSocket::GetRxLine()
{
	CString strTemp;
	int nIndex;

	while(!m_RxBuffer.IsEmpty())
	{
		nIndex = m_RxBuffer.Find("\r\n");
		if (nIndex != -1)
		{
			strTemp = m_RxBuffer.Left(nIndex);
			m_RxBuffer = m_RxBuffer.Mid(nIndex + 2);
			if (!strTemp.IsEmpty())
			{
				m_strCommands.AddTail(strTemp);
				// parse and execute command
				ParseCommand();
			}
		}
		else
			break;
	}
}


/********************************************************************/
/*																	*/
/* Function name: OnConnect											*/		
/* Description  : Called by the framework to notify this connecting	*/
/*				  socket that its connection attempt is completed.  */
/*																	*/
/********************************************************************/
void CConnectSocket::OnConnect(int nErrorCode) 
{
	CSocket::OnConnect(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name: HasConnectionDropped								*/		
/* Description  : Check if connection has been dropped.				*/
/*				  Used to detect if client has crashed.				*/
/*																	*/
/********************************************************************/
BOOL CConnectSocket::HasConnectionDropped(void)
{
	BOOL bConnDropped = FALSE;
	INT iRet = 0;
	BOOL bOK = TRUE;
	
	if (m_hSocket == INVALID_SOCKET)
		return TRUE;

	struct timeval timeout = { 0, 0 };
	fd_set readSocketSet;
	
	FD_ZERO(&readSocketSet);
	FD_SET(m_hSocket, &readSocketSet);
	
	iRet = ::select(0, &readSocketSet, NULL, NULL, &timeout);
	bOK = (iRet > 0);
	
	if(bOK)
	{
		bOK = FD_ISSET(m_hSocket, &readSocketSet);
	}
	
	if(bOK)
	{
		CHAR szBuffer[1] = "";
		iRet = ::recv(m_hSocket, szBuffer, 1, MSG_PEEK);
		bOK = (iRet > 0);
		if(!bOK)
		{
			INT iError = ::WSAGetLastError();
			bConnDropped = (( iError == WSAENETRESET) ||
				(iError == WSAECONNABORTED) ||
				(iError == WSAECONNRESET) ||
				(iError == WSAEINVAL) ||
				(iRet == 0));
		}
	}
    return(bConnDropped);
}


/********************************************************************/
/*																	*/
/* Function name: SendResponse										*/		
/* Description  : Send response to client.							*/
/*																	*/
/********************************************************************/
BOOL CConnectSocket::SendResponse(CString strData)
{
	// is connection still active ?
	if (HasConnectionDropped())
	{
		FireStatusMessage("could not send reply, disconnected.", 2);	

		Close();
		// tell our thread we have been closed
		
		// destroy connection
		m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
		return FALSE;
	}

	int nBytes = CSocket::Send(strData + "\r\n", strData.GetLength()+2);
	if (nBytes == SOCKET_ERROR)
	{
		Close();
		FireStatusMessage("could not send reply, disconnected.", 2);	

		// tell our thread we have been closed
		m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);

		return FALSE;
	}

	FireStatusMessage(strData, 2);

	((CConnectThread *)AfxGetThread())->IncSentBytes(nBytes);
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name: ParseCommand										*/		
/* Description  : Parse and execute command from client.			*/
/*																	*/
/********************************************************************/
void CConnectSocket::ParseCommand()
{
	static CFTPCommand commandList[] = 
	{
		{TOK_USER,	"USER", TRUE},
		{TOK_PASS,	"PASS", TRUE},
		{TOK_CWD,	"CWD",	TRUE},
		{TOK_PWD,	"PWD",	FALSE},
		{TOK_PORT,	"PORT", TRUE},
		{TOK_PASV,	"PASV", FALSE},
		{TOK_TYPE,	"TYPE", TRUE},
		{TOK_LIST,	"LIST", FALSE},
		{TOK_REST,	"REST", TRUE},
		{TOK_CDUP,	"CDUP", FALSE},
		{TOK_RETR,	"RETR", TRUE},
		{TOK_STOR,	"STOR", TRUE},
		{TOK_SIZE,	"SIZE", TRUE},
		{TOK_DELE,	"DELE", TRUE},
		{TOK_RMD,	"RMD",	TRUE},
		{TOK_MKD,	"MKD",	TRUE},
		{TOK_RNFR,	"RNFR", TRUE},
		{TOK_RNTO,	"RNTO", TRUE},
		{TOK_ABOR,	"ABOR", FALSE}, 
		{TOK_SYST,	"SYST", FALSE},
		{TOK_NOOP,	"NOOP", FALSE},
		{TOK_BYE,	"BYE",  FALSE},
		{TOK_QUIT,	"QUIT",  FALSE},
		{TOK_ERROR,	"",		FALSE},
	};
	
	// parse command
	CString strCommand, strArguments;
	if (!GetRxCommand(strCommand, strArguments))
	{
		return;
	}

	int nCommand;

	// find command in command list
	for (nCommand = TOK_USER; nCommand < TOK_ERROR; nCommand++)
	{
		// found command ?
		if (strCommand == commandList[nCommand].m_pszName)
		{
			// did we expect an argument ?
			if (commandList[nCommand].m_bHasArguments && (strArguments == ""))
			{
				SendResponse("501 Syntax error");
				return;
			}
			break;			
		}
	}

	if (nCommand == TOK_ERROR)
	{
		// command is not in our list
		SendResponse("500 Syntax error, command unrecognized.");
		return;
	}
	
	// no commands are excepted before successfull logged on
	if (nCommand > TOK_PASS && !m_bLoggedon)
	{
		SendResponse("530 Please log in with USER and PASS first.");
		return;
	}

	// proces command
	switch(nCommand)
	{
		// specify username
		case TOK_USER:
		{
			strArguments.MakeLower();
			m_bLoggedon = FALSE;
			m_strUserName = strArguments;

			CString strPeerAddress;
			UINT nPeerPort;
			GetPeerName(strPeerAddress, nPeerPort);

			// tell FTP server a new user has connected
			CConnectThread *pThread = (CConnectThread *)m_pThread;
			((CFTPServer *)pThread->m_pWndServer)->m_pEventSink->OnFTPUserConnected(m_pThread->m_nThreadID, m_strUserName, strPeerAddress);

			SendResponse("331 Password required for " + strArguments);
		}
		break;

		// specify password
		case TOK_PASS:
		{
			// already logged on ?
			if (m_bLoggedon)
			{
				SendResponse("503 Bad sequence of commands.");
			}
			else
			{
				// check user and password
				CUser user;
				if (theServer.m_UserManager.CheckUser(m_strUserName, strArguments, user))
				{
					// set home directory of user
					m_strCurrentDir = "/";

					// succesfully logged on
					m_bLoggedon = TRUE;
					SendResponse("230 Logged on");
				}
				else 
					SendResponse("530 Login or password incorrect!");
			}
		}
		break;
		
		// change current directory
		case TOK_CWD:
		{
			int nResult = theServer.m_UserManager.ChangeDirectory(m_strUserName, m_strCurrentDir, strArguments);
			
			CString str;
			switch(nResult)
			{
				case 0:
					str.Format("250 CWD successful. \"%s\" is current directory.", m_strCurrentDir);
					SendResponse(str);
					break;
				case 1:
					str.Format("550 CWD failed. \"%s\": Permission denied.", strArguments);
					SendResponse(str);
					break;
				default:
					str.Format("550 CWD failed. \"%s\": directory not found.", strArguments);
					SendResponse(str);
					break;
			}
		}
		break; 

		// print current directory
		case TOK_PWD:
		{
			CString str;
			str.Format("257 \"%s\" is current directory.", m_strCurrentDir);
			SendResponse(str);	
		}
		break;

		// specify IP and port (PORT a1,a2,a3,a4,p1,p2) -> IP address a1.a2.a3.a4, port p1*256+p2. 
		case TOK_PORT:
		{
			CString strSub;
			int nCount=0;

			while (AfxExtractSubString(strSub, strArguments, nCount++, ','))
			{
				switch(nCount)
				{
					case 1:	// a1
						m_TransferStatus.m_strRemoteHost = strSub;
						m_TransferStatus.m_strRemoteHost += ".";
						break;
					case 2:	// a2
						m_TransferStatus.m_strRemoteHost += strSub;
						m_TransferStatus.m_strRemoteHost += ".";
						break;
					case 3:	// a3
						m_TransferStatus.m_strRemoteHost += strSub;
						m_TransferStatus.m_strRemoteHost += ".";
						break;
					case 4:	// a4
						m_TransferStatus.m_strRemoteHost += strSub;
						break;
					case 5:	// p1
						m_TransferStatus.m_nRemotePort = 256*atoi(strSub);
						break;
					case 6:	// p2
						m_TransferStatus.m_nRemotePort += atoi(strSub);
						break;
				}
			}
			m_TransferStatus.m_bPassiveMode = FALSE;
			SendResponse("200 Port command successful");
			break;
		}
		
		// switch to passive mode
		case TOK_PASV:
		{
			// delete existing datasocket
			DestroyDataSocket();

			// create new data socket
			m_TransferStatus.m_pDataSocket = new CDataSocket(this, -1);

			if (!m_TransferStatus.m_pDataSocket->Create())
			{
				DestroyDataSocket();				
				SendResponse("421 Can't create socket");
				break;
			}
			// start listening
			m_TransferStatus.m_pDataSocket->Listen();
			m_TransferStatus.m_pDataSocket->AsyncSelect();
			
			CString strIP, strTmp;
			UINT nPort;
			
			// get our ip address
			GetSockName(strIP, nPort);
			// Now retrieve the port
			m_TransferStatus.m_pDataSocket->GetSockName(strTmp, nPort);
			// Reformat the ip
			strIP.Replace(".",",");
			// tell the client which address/port to connect to
			CString str;
			str.Format("227 Entering Passive Mode (%s,%d,%d)", strIP, nPort/256, nPort%256);
			SendResponse(str);
			m_TransferStatus.m_bPassiveMode = TRUE;
			break;
		} 

		case TOK_TYPE:
		{
			SendResponse("200 Type set to " + strArguments);
		}
		break;

		// list current directory
		case TOK_LIST:
		{
			if(!m_TransferStatus.m_bPassiveMode && (m_TransferStatus.m_strRemoteHost == "" || m_TransferStatus.m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
			}
			else
			{
				// if client did not specify a directory use current dir
				if (strArguments == "")
				{
					strArguments = m_strCurrentDir;
				}
				else
				{
					// check if argument is file or directory
					CString strResult;
					int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_LIST, strResult);
					if (nResult == 0)
					{
						strArguments = strResult;
					}
				}
				CString strListing;
				int nResult = theServer.m_UserManager.GetDirectoryList(m_strUserName, strArguments, strListing);
				switch(nResult)
				{
					case 1:
						SendResponse("550 Permission denied");
						break;
					case 2:
						SendResponse("550 Directory not found");
						break;
					default:
						if (!m_TransferStatus.m_bPassiveMode)
						{
							CDataSocket *pDataSocket = new CDataSocket(this, 0);
							pDataSocket->Create();
							pDataSocket->SetData(strListing);
							pDataSocket->AsyncSelect();
							m_TransferStatus.m_pDataSocket = pDataSocket;
							if (!pDataSocket->Connect(m_TransferStatus.m_strRemoteHost, m_TransferStatus.m_nRemotePort))
							{
								if (GetLastError() != WSAEWOULDBLOCK)
								{
									SendResponse("425 Can't open data connection");
									break;
								}
							}
							SendResponse("150 Opening data channel for directory list."); 
						}
						else
						{
							m_TransferStatus.m_pDataSocket->SetData(strListing);
							m_TransferStatus.m_pDataSocket->SetTransferType(0);
						}
						break;
				}
			} 
			break;
		} 
		// change to parent directory
		case TOK_CDUP:
		{
			CString strDirectory = "..";
			CString str;
			int nResult = theServer.m_UserManager.ChangeDirectory(m_strUserName, m_strCurrentDir, strDirectory);
			switch(nResult)
			{
				case 0:
					str.Format("250 CWD successful. \"%s\" is current directory.", m_strCurrentDir);
					SendResponse(str);
					break;
				case 1:
					str.Format("550 CWD failed. \"%s\": Permission denied.", strDirectory);
					SendResponse(str);
					break;
				case 2:
					str.Format("550 CWD failed. \"%s\": directory not found.", strDirectory);
					SendResponse(str);
					break;
			}
		}
		break; 

		// retrieve file
		case TOK_RETR:
		{
			if(!m_TransferStatus.m_bPassiveMode && (m_TransferStatus.m_strRemoteHost == "" || m_TransferStatus.m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			
			CString strResult;
			int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_DOWNLOAD, strResult);
			switch(nResult)
			{
				case 1:
					SendResponse("550 Permission denied");
					break;
				case 2:
					SendResponse("550 File not found");
					break;
				default:
					if (!m_TransferStatus.m_bPassiveMode)
					{
						CDataSocket *pDataSocket = new CDataSocket(this, 1);
						m_TransferStatus.m_pDataSocket = pDataSocket;
						pDataSocket->Create();
						pDataSocket->AsyncSelect();
						pDataSocket->SetData(strResult);
						if (pDataSocket->Connect(m_TransferStatus.m_strRemoteHost ,m_TransferStatus.m_nRemotePort) == 0)
						{
							if (GetLastError() != WSAEWOULDBLOCK)
							{
								SendResponse("425 Can't open data connection");
								break;
							}
						}
						SendResponse("150 Opening data channel for file transfer.");
					}
					else
					{
						m_TransferStatus.m_pDataSocket->SetData(strResult);
						m_TransferStatus.m_pDataSocket->SetTransferType(1);
					}
					break;
			}
			break;	
		}

		// client wants to upload file
		case TOK_STOR:
		{
			if(m_TransferStatus.m_bPassiveMode == -1)
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			if(!m_TransferStatus.m_bPassiveMode && (m_TransferStatus.m_strRemoteHost == "" || m_TransferStatus.m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			
			CString strResult;
			int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_UPLOAD, strResult);
			switch(nResult)
			{
				case 1:
					SendResponse("550 Permission denied");
					break;
				case 2:
					SendResponse("550 Filename invalid");
					break;
				default:
					if (!m_TransferStatus.m_bPassiveMode)
					{
						CDataSocket *pDataSocket = new CDataSocket(this, 2);
						m_TransferStatus.m_pDataSocket = pDataSocket;
						pDataSocket->Create();
						pDataSocket->AsyncSelect();
						pDataSocket->SetData(strResult);

						if (pDataSocket->Connect(m_TransferStatus.m_strRemoteHost, m_TransferStatus.m_nRemotePort) == 0)
						{
							if (GetLastError() != WSAEWOULDBLOCK)
							{
								SendResponse("425 Can't open data connection");
								break;
							}
						}
						SendResponse("150 Opening data channel for file transfer.");
					}
					else
					{
						m_TransferStatus.m_pDataSocket->SetData(strResult);
						m_TransferStatus.m_pDataSocket->SetTransferType(2);
					}
					break;
			}
		}
		break;
		
		// get file size
		case TOK_SIZE:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_DOWNLOAD, strResult);
			switch(nResult)
			{
				case 1:
					SendResponse("550 Permission denied");
					break;
				case 2:
					SendResponse("550 File not found");
					break;
				default:
				{
					CFileStatus status;
					CFile::GetStatus(strResult, status);
					CString strResponse;
					strResponse.Format("213 %d", status.m_size);
					SendResponse(strResponse);
					break;
				}
			}
		}
		break;
		
		// delete file
		case TOK_DELE:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_DELETE, strResult);
			switch(nResult)
			{
				case 1:
					SendResponse("550 Permission denied");
					break;
				case 2:
					SendResponse("550 File not found");
					break;
				default:
					try
					{
						CFile::Remove(strResult);
					}
					catch(CFileException *e)
					{
						e->Delete();
						SendResponse("450 Internal error deleting the file.");
						break;
					}
					SendResponse("250 File deleted successfully");			
					break;
			}
		}
		break;
		
		// remove directory
		case TOK_RMD: 
		{
			CString strResult;
			int nResult = theServer.m_UserManager.GetDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_DELETE, strResult);
			switch(nResult)
			{
				case 1:
					SendResponse("550 Permission denied");
					break;
				case 2:
					SendResponse("550 Directory not found");
					break;
				default:
					if (!RemoveDirectory(strResult))
					{
						if (GetLastError() == ERROR_DIR_NOT_EMPTY)
							SendResponse("550 Directory not empty.");
						else
							SendResponse("450 Internal error deleting the directory.");
					}
					else
					{
						SendResponse("250 Directory deleted successfully");
					}
					break;
			}
		}
		break;	
		
		// create directory
		case TOK_MKD: 
		{
			CString strResult;
			int nResult = theServer.m_UserManager.GetDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_CREATE_DIR, strResult);
			switch(nResult)
			{
				case 0:
					SendResponse("550 Directory already exists");
					break;
				case 1:
					SendResponse("550 Can't create directory. Permission denied");
					break;
				case 3:
					SendResponse("550 Directoryname not valid");
					break;
				default:
					strResult += "\\";
					CString strDir;
					BOOL bResult;
					// create directory structure one part at a time
					while (strResult != "")
					{
						strDir += strResult.Left(strResult.Find("\\")+1);
						strResult = strResult.Mid(strResult.Find("\\")+1);
						bResult = CreateDirectory(strDir, 0);
					}
					if (!bResult)
					{
						SendResponse("450 Internal error creating the directory.");
					}
					else
						SendResponse("250 Directory created successfully");
					break;
			}
		}
		break;
		
		// rename file or directory (part 1)
		case TOK_RNFR:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
			if (nResult == 0)
			{
				m_strRenameFile = strResult;
				m_bRenameFile = TRUE;
				SendResponse("350 File exists, ready for destination name.");
				break;
			}
			else
			{
				// client wants to rename directory
				nResult = theServer.m_UserManager.GetDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
				switch(nResult)
				{
					case 0:
						m_strRenameFile = strResult;
						m_bRenameFile = FALSE;
						SendResponse("350 Directory exists, ready for destination name.");
						break;
					case 1:
						SendResponse("550 Permission denied");
						break;
					default: 
						SendResponse("550 file/directory not found");
						break;
				}
			}	
		}
		break;

		// rename file or directory (part 2)
		case TOK_RNTO:
		{
			if (m_strRenameFile.IsEmpty())
			{
				SendResponse("503 Bad sequence of commands!");
				break;
			}
			if (m_bRenameFile)
			{
				CString strResult;
				// check destination filename
				int nResult = theServer.m_UserManager.GetFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
				switch(nResult)
				{
					case 0:
						SendResponse("550 file exists");
						break;
					case 1:
						SendResponse("550 Permission denied");
						break;
					default:
						if (!MoveFile(m_strRenameFile, strResult))
							SendResponse("450 Internal error renaming the file");
						else
							SendResponse("250 file renamed successfully");
						break;
				}
			}
			else
			{
				CString strResult;
				// check destination directory name
				int nResult = theServer.m_UserManager.GetDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
				switch(nResult)
				{
					case 0:
						SendResponse("550 directory exists");
						break;
					case 1:
						SendResponse("550 Permission denied");
						break;
					case 3:
						SendResponse("550 Filename invalid");
						break;
					default:
						if (!MoveFile(m_strRenameFile, strResult))
							SendResponse("450 Internal error renaming the file");
						else
							SendResponse("250 file renamed successfully");
						break;
				}
			}		
		}
		break;

		// abort transfer
		case TOK_ABOR:
		{
			if (m_TransferStatus.m_pDataSocket)
			{
				if (m_TransferStatus.m_pDataSocket->GetStatus() != XFERMODE_IDLE)
					SendResponse("426 Connection closed; transfer aborted.");

				// destroy data socket
				m_pThread->PostThreadMessage(WM_THREADMSG, 0, 0);
//				DestroyDataSocket();
			}
			SendResponse("226 ABOR command successful");
			break;
		} 

		// get system info
		case TOK_SYST:
			SendResponse("215 UNIX emulated by Pablo's FTP Server");
			break;
		
		// close connection
		case TOK_BYE:
		case TOK_QUIT:
		{
			// send goodbye message to client
			CConnectThread *pThread = (CConnectThread *)m_pThread;
			CString strText = ((CFTPServer *)pThread->m_pWndServer)->GetGoodbyeMessage();
			SendResponse("220 " + strText);

			Close();
			
			// tell our thread we have been closed
		
			// destroy connection
			m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
			break;
		}
		// dummy instruction
		case TOK_NOOP:
			SendResponse("200 OK");
			break;

		default:
			SendResponse("502 Command not implemented.");
			break;
	}
}


/********************************************************************/
/*																	*/
/* Function name: FireStatusMessage									*/		
/* Description  : Fire status message.								*/
/*																	*/
/********************************************************************/
void CConnectSocket::FireStatusMessage(LPCTSTR lpszStatus, int nType)
{
	CConnectThread *pThread = (CConnectThread *)m_pThread;
	((CFTPServer *)pThread->m_pWndServer)->AddTraceLine(nType, "[%d] %s", m_pThread->m_nThreadID, lpszStatus);
}


/********************************************************************/
/*																	*/
/* Function name: DestroyDataSocket									*/		
/* Description  : Close data transfer socket.						*/
/*																	*/
/********************************************************************/
void CConnectSocket::DestroyDataSocket()
{
	if (!m_TransferStatus.m_pDataSocket)
		return;

//	m_TransferStatus.m_pDataSocket->Close();
	
	delete m_TransferStatus.m_pDataSocket;

	// reset transfer status
	m_TransferStatus.m_pDataSocket = NULL;
	m_TransferStatus.m_strRemoteHost = "";
	m_TransferStatus.m_nRemotePort = -1;
	m_TransferStatus.m_bPassiveMode = FALSE;
	m_TransferStatus.m_nRest = -1;
}




char *CConnectSocket::GetResponseMessage(int nResponse) 
{
	switch(nResponse) 
	{
		case 110 : return "Restart marker reply"; break;
		case 120 : return "Service ready in nnn minutes."; break;
		case 125 : return "Data connection already open; transfer starting."; break;
		case 150 : return "File status okay; about to open data connection."; break;
		case 200 : return "Command okay."; break;
		case 202 : return "Command not implemented, superfluous at this site."; break;
		case 211 : return "System status, or system help reply."; break;
		case 212 : return "Directory status."; break;
		case 213 : return "File status."; break;
		case 214 : return "Help message."; break;
		case 215 : return "NAME system type."; break;
		case 220 : return "Service ready for new user."; break;
		case 221 : return "Service closing control connection."; break;
		case 225 : return "Data connection open; no transfer in progress."; break;
		case 226 : return "Closing data connection successfully."; break;
		case 227 : return "Entering Passive Mode (h1,h2,h3,h4,p1,p2)."; break;
		case 230 : return "User logged in, proceed."; break;
		case 250 : return "Requested file action okay, completed."; break;
		case 257 : return "PATHNAME created."; break;
		case 331 : return "User name okay, need password."; break;
		case 332 : return "Need account for login."; break;
		case 350 : return "Requested file action pending further information."; break;
		case 421 : return "Service not available, closing control connection."; break;
		case 425 : return "Can't open data connection."; break;
		case 426 : return "Connection closed; transfer aborted."; break;
		case 450 : return "Requested file action not taken - File unavailable."; break;
		case 451 : return "Requested action aborted: local error in processing."; break;
		case 452 : return "Requested action not taken - Insufficient storage space in system."; break;
		case 500 : return "Syntax error, command unrecognized."; break;
		case 501 : return "Syntax error in parameters or arguments."; break;
		case 502 : return "Command not implemented."; break;
		case 503 : return "Bad sequence of commands."; break;
		case 504 : return "Command not implemented for that parameter."; break;
		case 530 : return "Not logged in."; break;
		case 532 : return "Need account for storing files."; break;
		case 550 : return "Requested action not taken - File unavailable."; break;
		case 551 : return "Requested action aborted: page type unknown."; break;
		case 552 : return "Requested file action aborted - Exceeded storage allocation"; break;
		case 553 : return "Requested action not taken - File name not allowed."; break;
		default  : return "Unknown error";
	}
}


void CConnectSocket::Shutdown(int nReason)
{
	DestroyDataSocket();

	switch(nReason)
	{
		case 1:
			SendResponse("421 Connection timed out.");
			break;
		case 2:
			SendResponse("421 No-transfer-time exceeded. Closing control connection.");
			break;
		case 3:
			SendResponse("421 Login time exceeded. Closing control connection.");
			break;
		case 4:
			SendResponse("421 Kicked by Administrator");
			break;
		case 5:
			SendResponse("421 Problem with server");
			break;
		default:
			SendResponse("421 Server is going offline");
			break;
	}
	FireStatusMessage("disconnected.", 0);	
	Close();

	// tell our thread we have been closed
//	m_pThread->PostThreadMessage(WM_QUIT,0,0);
}
