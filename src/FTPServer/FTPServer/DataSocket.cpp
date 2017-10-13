
/********************************************************************/
/*																	*/
/*  DataSocket.cpp													*/
/*																	*/
/*  Implementation of the Data Socket.								*/
/*																	*/
/*  Programmed by xingyun86								*/
/*	http://www.ppsbbs.tech									*/
/*																	*/
/*  Last updated: 10 july 2002										*/
/*																	*/
/********************************************************************/


#include "stdafx.h"
#include "resource.h"
#include "DataSocket.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PACKET_SIZE 4096

/********************************************************************/
/*																	*/
/* Function name : CDataSocket::CDataSocket							*/
/* Description   : Constructor										*/
/*																	*/
/********************************************************************/
CDataSocket::CDataSocket(CConnectSocket *pSocket, int nTransferType)
{
	m_nTransferType = nTransferType;
	m_pConnectSocket = pSocket;
	m_nStatus = XFERMODE_IDLE;
	m_strData = "";
	m_File.m_hFile = NULL;
	m_bConnected = FALSE;
	m_bPassiveMode = FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::~CDataSocket						*/
/* Description   : Destructor										*/
/*																	*/
/********************************************************************/
CDataSocket::~CDataSocket()
{
	m_bConnected = FALSE;
	TRACE0("CDataSocket destroyed.\n");
}


#if 0
BEGIN_MESSAGE_MAP(CDataSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CDataSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::OnSend								*/
/* Description   : Called by the framework to notify socket	that	*/
/*				   it can send data by calling the Send method.		*/
/*																	*/
/********************************************************************/
void CDataSocket::OnSend(int nErrorCode) 
{
	switch(m_nStatus)
	{
		case XFERMODE_LIST:
		{
			while (m_nTotalBytesTransfered < m_nTotalBytesSend)
			{
				DWORD dwRead;
				int dwBytes;

				CString strData;
				
				dwRead = m_strData.GetLength();
				
				if (dwRead <= PACKET_SIZE)
				{
					strData = m_strData;
				}
				else
				{
					strData = m_strData.Left(PACKET_SIZE);
					dwRead = strData.GetLength();
				}
				
				if ((dwBytes = Send(strData, dwRead)) == SOCKET_ERROR)
				{
					if (GetLastError() == WSAEWOULDBLOCK) 
					{
						Sleep(0);
						break;
					}
					else
					{
						TCHAR szError[256];
						wsprintf(szError, "Server Socket failed to send: %d", GetLastError());

						// close the data connection.
						Close();

						m_nTotalBytesSend = 0;
						m_nTotalBytesTransfered = 0;

						// change status
						m_nStatus = XFERMODE_IDLE;

						m_pConnectSocket->SendResponse("426 Connection closed; transfer aborted.");

						// destroy this socket
						AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
					}
				}
				else
				{
					m_nTotalBytesTransfered += dwBytes;

					m_strData = m_strData.Mid(dwBytes);
					
					((CConnectThread *)AfxGetThread())->IncSentBytes(dwBytes);
				}
			}
			if (m_nTotalBytesTransfered == m_nTotalBytesSend)
			{
                // close the data connection.
                Close();

                m_nTotalBytesSend = 0;
                m_nTotalBytesTransfered = 0;

                // change status
				m_nStatus = XFERMODE_IDLE;

				// tell the client the transfer is complete.
				m_pConnectSocket->SendResponse("226 Transfer complete");
				// destroy this socket
				AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
			}
			break;
//			if (m_bPassiveMode)
/*			int dwBytes;
			if (dwBytes = Send(m_strData, m_strData.GetLength()) == SOCKET_ERROR)
			{
				m_nStatus = XFERMODE_IDLE;
			}

			m_nStatus = XFERMODE_IDLE;
			Close();
			// tell the client the transfer is complete.
			m_pConnectSocket->SendResponse("226 Transfer complete");
			// destroy this socket
			AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0); 
			break; */
		}
		case XFERMODE_SEND:
		{
			while (m_nTotalBytesTransfered < m_nTotalBytesSend)
			{
				// allocate space to store data
				byte data[PACKET_SIZE];
				
				m_File.Seek(m_nTotalBytesTransfered, CFile::begin);

				DWORD dwRead = m_File.Read(data, PACKET_SIZE);
    
				int dwBytes;

				if ((dwBytes = Send(data, dwRead)) == SOCKET_ERROR)
				{
					if (GetLastError() == WSAEWOULDBLOCK) 
					{
						Sleep(0);
						break;
					}
					else
					{
						TCHAR szError[256];
						wsprintf(szError, "Server Socket failed to send: %d", GetLastError());

						// close file.
						m_File.Close();
						m_File.m_hFile = NULL;

						// close the data connection.
						Close();

						m_nTotalBytesSend = 0;
						m_nTotalBytesTransfered = 0;

						// change status
						m_nStatus = XFERMODE_IDLE;

						m_pConnectSocket->SendResponse("426 Connection closed; transfer aborted.");

						// destroy this socket
						AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);

						// download failed
						((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_DOWNLOADFAILED);
					}
				}
				else
				{
					m_nTotalBytesTransfered += dwBytes;
					
					((CConnectThread *)AfxGetThread())->IncSentBytes(dwBytes);
				}
			}
			if (m_nTotalBytesTransfered == m_nTotalBytesSend)
			{
				// close file.
                m_File.Close();
				m_File.m_hFile = NULL;

                // close the data connection.
                Close();

                m_nTotalBytesSend = 0;
                m_nTotalBytesTransfered = 0;

                // change status
				m_nStatus = XFERMODE_IDLE;

				// tell the client the transfer is complete.
				m_pConnectSocket->SendResponse("226 Transfer complete");
				// destroy this socket
				AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
				// download successfull
				((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_DOWNLOADSUCCEEDED);
			}
			break;
		}
		default:
			break;
	}

	CAsyncSocket::OnSend(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::OnConnect							*/
/* Description   : Called by the framework to notify connecting		*/
/*				   socket that its connection attempt is completed	*/
/*																	*/
/********************************************************************/
void CDataSocket::OnConnect(int nErrorCode) 
{
	if (nErrorCode)
	{
		m_nStatus = XFERMODE_ERROR;
		m_pConnectSocket->SendResponse("425 Can't open data connection.");
		// destroy this socket
		AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
	}
	else
	{
		switch (m_nTransferType)
		{
			case 0:	// List Directory
				m_nStatus = XFERMODE_LIST;
				m_bConnected = TRUE;
//				m_nTotalBytesSend = m_strData.GetLength();
//				m_nTotalBytesTransfered = 0;
				OnSend(0);
//				Send(m_strData, m_strData.GetLength());
				break;
			case 1:	// Send File
				if (PrepareSendFile(m_strData))
				{
					m_nStatus = XFERMODE_SEND;
					m_bConnected = TRUE;
				}
				else
				{
					Close();
				}
				break;
			case 2:	// Receive File
				if (PrepareReceiveFile(m_strData))
				{
					m_nStatus = XFERMODE_RECEIVE;
					m_bConnected = TRUE;
				}
				else
				{
					Close();
					m_pConnectSocket->SendResponse("450 can't access file.");
					// destroy this socket
					AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
					// upload failed
					((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_UPLOADFAILED);
				}
				break;
		}
	}
	CAsyncSocket::OnConnect(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::OnClose								*/
/* Description   : Called by the framework to notify this socket	*/
/*				   that the connected socket is closed.				*/
/*																	*/
/********************************************************************/
void CDataSocket::OnClose(int nErrorCode) 
{
	TRACE("CDataSocket() OnClose()\n");
	if (m_pConnectSocket)
	{
		// shutdown sends
		ShutDown(1);

		if (m_nStatus == XFERMODE_RECEIVE)
		{
			while(Receive() != 0)
			{
				// receive remaining data				
			}
		}
		else
		{
			m_pConnectSocket->SendResponse("426 Connection closed; transfer aborted.");
			// destroy this socket
			AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
			// upload failed
			((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_UPLOADFAILED);
		}
	}
	m_nStatus = XFERMODE_IDLE;	
	m_bConnected = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::OnAccept							*/
/* Description   : Notify this listening socket that it can accept	*/
/*				   pending connection requests.						*/
/*																	*/
/********************************************************************/
void CDataSocket::OnAccept(int nErrorCode) 
{
	CAsyncSocket socky;
	Accept(socky);
	
	SOCKET socket = socky.Detach();
	Close();

	Attach(socket);

	m_bConnected = TRUE;
	m_pConnectSocket->SendResponse("150 Connection accepted");

	CAsyncSocket::OnAccept(nErrorCode);

	m_bPassiveMode = TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::GetStatus							*/
/* Description   : Get socket status.								*/
/*																	*/
/********************************************************************/
int CDataSocket::GetStatus()
{
	return m_nStatus;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::PrepareSendFile						*/
/* Description   : Prepare socket to send a file.					*/
/*																	*/
/********************************************************************/
BOOL CDataSocket::PrepareSendFile(LPCTSTR lpszFilename)
{
	// close file if it's already open
	if (m_File.m_hFile != NULL)
	{
		m_File.Close();
	}

	// open source file
	if (!m_File.Open(lpszFilename, CFile::modeRead | CFile::typeBinary))
	{
		return FALSE;
	}
	m_nTotalBytesSend = m_File.GetLength();
	m_nTotalBytesTransfered = 0;

	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::PrepareReceiveFile					*/
/* Description   : Prepare socket to receive a file.				*/
/*																	*/
/********************************************************************/
BOOL CDataSocket::PrepareReceiveFile(LPCTSTR lpszFilename)
{
	// close file if it's already open
	if (m_File.m_hFile != NULL)
	{
		m_File.Close();
	}

	// open destination file
	if (!m_File.Open(lpszFilename, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
	{
		return FALSE;
	}
	m_nTotalBytesReceive = 0;
	m_nTotalBytesTransfered = 0;
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::OnReceive							*/
/* Description   : Called by the framework to notify this socket	*/
/*				   that there is data in the buffer that can be		*/
/*				   retrieved by calling the Receive member function.*/
/*																	*/
/********************************************************************/
void CDataSocket::OnReceive(int nErrorCode) 
{
	CAsyncSocket::OnReceive(nErrorCode);
	
	Receive();
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::Receive								*/
/* Description   : Receive data from a socket						*/
/*																	*/
/********************************************************************/
int CDataSocket::Receive()
{
	int nRead = 0;
	
	if (m_nStatus == XFERMODE_RECEIVE)
	{
		if (m_File.m_hFile == NULL)
			return 0;

		byte data[PACKET_SIZE];
		nRead = CAsyncSocket::Receive(data, PACKET_SIZE);

		switch(nRead)
		{
			case 0:
			{
				m_File.Close();
				m_File.m_hFile = NULL;
				Close();
				// tell the client the transfer is complete.
				m_pConnectSocket->SendResponse("226 Transfer complete");
				// destroy this socket
				AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
				// upload succesfull
				((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_UPLOADSUCCEEDED);
				break;
			}
			case SOCKET_ERROR:
			{
				if (GetLastError() != WSAEWOULDBLOCK)
				{
					m_File.Close();
					m_File.m_hFile = NULL;
					Close();
					m_pConnectSocket->SendResponse("426 Connection closed; transfer aborted.");
					// destroy this socket
					AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
					// upload failed
					((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_UPLOADFAILED);

				}
				break;
			}
			default:
			{
				((CConnectThread *)AfxGetThread())->IncReceivedBytes(nRead);

				TRY
				{
					m_File.Write(data, nRead);
				}
				CATCH_ALL(e)
				{
					m_File.Close();
					m_File.m_hFile = NULL;
					Close();
					m_pConnectSocket->SendResponse("450 can't access file.");
					// destroy this socket
					AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
					// upload failed
					((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_UPLOADFAILED);
					return 0;
				}
				END_CATCH_ALL;
				break;
			}
		}
	}
	return nRead;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::SetData								*/
/* Description   : Set data that is initially send to client.		*/
/*																	*/
/********************************************************************/
void CDataSocket::SetData(LPCTSTR lpszData)
{
	m_strData = lpszData;
	m_nTotalBytesSend = m_strData.GetLength();
	m_nTotalBytesTransfered = 0;
}


/********************************************************************/
/*																	*/
/* Function name : CDataSocket::SetTransferType						*/
/* Description   : Set transfer type:								*/
/*				   0 = LIST DIR, 1 = SEND FILE, 2 = RECEIVE FILE	*/
/*																	*/
/********************************************************************/
void CDataSocket::SetTransferType(int nType)
{
	m_nTransferType = nType; 

	switch(m_nTransferType)
	{
		case 0:	// List Directory
			m_nStatus = XFERMODE_LIST;
			OnSend(0);
			break;
		case 1:	// Send File
			if (PrepareSendFile(m_strData))
			{
				m_nStatus = XFERMODE_SEND;
				m_bConnected = TRUE;
				OnSend(0);
			}
			else
			{
				Close();
			}
			break;
		case 2:	// Receive File
			if (PrepareReceiveFile(m_strData))
			{
				m_nStatus = XFERMODE_RECEIVE;
				m_bConnected = TRUE;
				OnSend(0);
			}
			else
			{
				Close();
				m_pConnectSocket->SendResponse("450 can't access file.");
				// destroy this socket
				AfxGetThread()->PostThreadMessage(WM_THREADMSG, 0, 0);
				// upload failed
				((CConnectThread *)AfxGetThread())->UpdateStatistic(FTPSTAT_UPLOADFAILED);
			}
			break;
		default:
			break;
	}
}

