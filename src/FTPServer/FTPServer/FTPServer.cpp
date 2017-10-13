
/****************************************************************/
/*																*/
/*  FTPServer.cpp												*/
/*																*/
/*  Implementation of the CFTPServer class.						*/
/*	This class is a part of the FTP Server Application			*/
/*																*/
/*  Programmed by xingyun86							*/
/*  Copyright @2017					*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "FTPServer.h"


CFTPServer::CFTPServer()
{
	m_nPort = 21;
	m_nMaxUsers = 10;
	m_strWelcomeMessage = "Welcome to Pablo's FTP Server";
	m_strGoodbyeMessage = "Bye";
	m_nTimeout = 5;
	m_bRunning = FALSE;
	m_hWnd = NULL;
	m_nConnectionCount = 0;

	// intialize statistics
	m_dwTotalSentBytes = 0;
	m_dwTotalReceivedBytes = 0;
	m_nTotalConnections = 0;
	m_nFilesDownloaded = 0;
	m_nFilesUploaded = 0;
	m_nFailedDownloads = 0;
	m_nFailedUploads = 0;

	m_nSecurityMode = 0;

	m_nStatisticsInterval = 0;

	// load users
	m_UserManager.Serialize(FALSE);

	// load security
	m_SecurityManager.Serialize(FALSE);
}

CFTPServer::~CFTPServer()
{
	Stop();
}


BEGIN_MESSAGE_MAP(CFTPServer, CWnd)
	//{{AFX_MSG_MAP(CFTPServer)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADSTART, OnThreadStart)
	ON_MESSAGE(WM_THREADCLOSE, OnThreadClose)
	ON_MESSAGE(WM_THREADMSG, OnThreadMessage)
END_MESSAGE_MAP()



/********************************************************************/
/*																	*/
/* Function name : Start											*/
/* Description   : Start listining on port 21 and accept new		*/
/*				   connections.										*/
/*																	*/
/********************************************************************/
BOOL CFTPServer::Start()
{
	if (m_bRunning)
		return FALSE;

	// create dummy window for message routing
    if (!CWnd::CreateEx(0, AfxRegisterWndClass(0), "FTP Server Notification Sink", WS_POPUP, 0,0,0,0, NULL, 0))
	{
		AddTraceLine(0, "Failed to create notification window.");
		return FALSE;
	}
	// created the listen socket
	if (m_ListenSocket.Create(m_nPort))
	{
		// start listening
		if (m_ListenSocket.Listen())
		{
			m_ListenSocket.m_pWndServer = this;
			m_bRunning = TRUE;	
			
			SetTimer(1, m_nStatisticsInterval, NULL);

			AddTraceLine(0, "FTP Server started on port %d.", m_nPort);
			return TRUE;
		}
	}
	AddTraceLine(0, "FTP Server failed to listen on port %d.", m_nPort);

	// destroy notification window
	if (IsWindow(m_hWnd))
		DestroyWindow();
	m_hWnd = NULL;

	return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : Stop												*/
/* Description   : Stop FTP server.									*/
/*																	*/
/********************************************************************/
void CFTPServer::Stop()
{
	if (!m_bRunning)
		return;

	// stop statistics timer
	KillTimer(1);

	m_bRunning = FALSE;	
	m_ListenSocket.Close();

	CConnectThread* pThread = NULL;

	// close all running threads
	do
	{
		m_CriticalSection.Lock();

		POSITION pos = m_ThreadList.GetHeadPosition();
		if (pos != NULL)
		{
			pThread = (CConnectThread *)m_ThreadList.GetAt(pos);
		
			m_CriticalSection.Unlock();

			// save thread members
			int nThreadID = pThread->m_nThreadID;
			HANDLE hThread = pThread->m_hThread;

			AddTraceLine(0, "[%d] Shutting down thread...", nThreadID);

			// tell thread to stop
			pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
			pThread->PostThreadMessage(WM_QUIT,0,0);

			// wait for thread to end, while keeping the messages pumping (max 5 seconds)
			if (WaitWithMessageLoop(hThread, 5000) == FALSE)
			{
				// thread doesn't want to stopped
				AddTraceLine(0, "[%d] Problem while killing thread.", nThreadID);
				// don't try again, so remove
				m_CriticalSection.Lock();
				POSITION rmPos = m_ThreadList.Find(pThread);
				if (rmPos != NULL)
					m_ThreadList.RemoveAt(rmPos);
				m_CriticalSection.Unlock();
			}
			else
			{
				AddTraceLine(0, "[%d] Thread successfully stopped.", nThreadID);
			}
		}
		else
		{
			m_CriticalSection.Unlock();	
			pThread = NULL;
		}
	}
	while (pThread != NULL);

	AddTraceLine(0, "FTP Server stopped.");

	if (IsWindow(m_hWnd))
		DestroyWindow();

	m_hWnd = NULL;
}


/********************************************************************/
/*																	*/
/* Function name : IsActive											*/
/* Description   : Is FTP server active?							*/
/*																	*/
/********************************************************************/
BOOL CFTPServer::IsActive()
{
	return m_bRunning;
}


/********************************************************************/
/*																	*/
/* Function name : SetMaxUsers										*/
/* Description   : Set maximum number of users						*/
/*																	*/
/********************************************************************/
void CFTPServer::SetMaxUsers(int nValue)
{
	m_nMaxUsers = nValue;
}


/********************************************************************/
/*																	*/
/* Function name : SetPort											*/
/* Description   : Set listening port for new connections			*/
/*																	*/
/********************************************************************/
void CFTPServer::SetPort(int nValue)
{
	m_nPort = nValue;
}


/********************************************************************/
/*																	*/
/* Function name : SetTimeout										*/
/* Description   : Set connection timeout							*/
/*																	*/
/********************************************************************/
void CFTPServer::SetTimeout(int nValue)
{
	m_nTimeout = nValue;
}


/********************************************************************/
/*																	*/
/* Function name : SetTimeout										*/
/* Description   : Set connection timeout							*/
/*																	*/
/********************************************************************/
void CFTPServer::SetStatisticsInterval(int nValue)
{
	m_nStatisticsInterval = nValue;
	if (m_nStatisticsInterval != 0)
	{
		KillTimer(1);
		SetTimer(1, nValue, NULL);
	}
	else
	{
		KillTimer(1);
	}
}


/********************************************************************/
/*																	*/
/* Function name : SetWelcomeMessage								*/
/* Description   : Set welcome message								*/
/*																	*/
/********************************************************************/
void CFTPServer::SetWelcomeMessage(LPCTSTR lpszText)
{
	m_strWelcomeMessage = lpszText;
}


/********************************************************************/
/*																	*/
/* Function name : SetGoodbyeMessage								*/
/* Description   : Set goodbye message								*/
/*																	*/
/********************************************************************/
void CFTPServer::SetGoodbyeMessage(LPCTSTR lpszText)
{
	m_strGoodbyeMessage = lpszText;
}


/********************************************************************/
/*																	*/
/* Function name : Initialize										*/
/* Description   : Initialize event sink.							*/
/*																	*/
/********************************************************************/
void CFTPServer::Initialize(CFTPEventSink *pEventSink)
{
	m_pEventSink = pEventSink;
}


/********************************************************************/
/*																	*/
/* Function name : AddTraceLine										*/
/* Description   : FTP status change.								*/
/*																	*/
/********************************************************************/
void CFTPServer::AddTraceLine(int nType, LPCTSTR pstrFormat, ...)
{
	CString str;

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	m_pEventSink->OnFTPStatusChange(nType, str);
}


/********************************************************************/
/*																	*/
/* Function name : OnThreadStart									*/
/* Description   : Called when a new thread has started.			*/
/*																	*/
/********************************************************************/
LRESULT CFTPServer::OnThreadStart(WPARAM wParam, LPARAM)
{
	m_nConnectionCount++;
	m_nTotalConnections++;
	CConnectThread *pThread = (CConnectThread *)wParam;

	UINT port;

	pThread->m_ConnectSocket.GetPeerName(pThread->m_strRemoteHost, port);
	AddTraceLine(0, "[%d] Client connected from %s.", pThread->m_nThreadID, pThread->m_strRemoteHost);

	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnThreadClose									*/
/* Description   : Called when a thread is about to stop.			*/
/*																	*/
/********************************************************************/
LRESULT CFTPServer::OnThreadClose(WPARAM wParam, LPARAM lParam)
{
	m_nConnectionCount--;
	CConnectThread *pThread = (CConnectThread *)wParam;

	AddTraceLine(0, "[%d] Client disconnected from %s.", pThread->m_nThreadID, pThread->m_strRemoteHost);
	
	m_pEventSink->OnFTPUserDisconnected(pThread->m_nThreadID, pThread->m_ConnectSocket.m_strUserName);
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnThreadMessage									*/
/* Description   : Message sent from connect connection.			*/
/*																	*/
/********************************************************************/
LRESULT CFTPServer::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 0:
			m_dwTotalSentBytes += (int)lParam;
			break;
		case 1:
			m_dwTotalReceivedBytes += (int)lParam;
			break;
		case 2:
			switch(lParam)
			{
				case FTPSTAT_DOWNLOADSUCCEEDED:
					m_nFilesDownloaded++;
					break;
				case FTPSTAT_UPLOADSUCCEEDED:
					m_nFilesUploaded++;
					break;
				case FTPSTAT_DOWNLOADFAILED:
					m_nFailedDownloads++;
					break;
				case FTPSTAT_UPLOADFAILED:
					m_nFailedUploads++;
					break;
			}
			break;
		default:
			
			break;
	}
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : CheckMaxUsers									*/
/* Description   : Reached maximum number of connections?			*/
/*																	*/
/********************************************************************/
BOOL CFTPServer::CheckMaxUsers()
{
	if (m_nConnectionCount > m_nMaxUsers)
		return TRUE;
	else
		return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : OnTimer											*/
/* Description   : Update statictics.								*/
/*																	*/
/********************************************************************/
void CFTPServer::OnTimer(UINT nIDEvent) 
{
	// update statictics ?
	if (nIDEvent == 1)
	{
		m_pEventSink->OnFTPSentBytesChange(m_dwTotalSentBytes);
		m_pEventSink->OnFTPReceivedBytesChange(m_dwTotalReceivedBytes);
		m_pEventSink->OnFTPStatisticChange(0, m_nTotalConnections);
		m_pEventSink->OnFTPStatisticChange(1, m_nConnectionCount);
		m_pEventSink->OnFTPStatisticChange(2, m_nFilesDownloaded);
		m_pEventSink->OnFTPStatisticChange(3, m_nFilesUploaded);
		m_pEventSink->OnFTPStatisticChange(4, m_nFailedDownloads);
		m_pEventSink->OnFTPStatisticChange(5, m_nFailedUploads);
	}
	CWnd::OnTimer(nIDEvent);
}


/********************************************************************/
/*																	*/
/* Function name : SetSecurityMode									*/
/* Description   : Set security mode.								*/
/*																	*/
/********************************************************************/
void CFTPServer::SetSecurityMode(BOOL bBlockSpecific)
{
	m_nSecurityMode = bBlockSpecific ? 0 : 1;
}


/********************************************************************/
/*																	*/
/* Function name : IsIPAddressAllowed								*/
/* Description   : Check (based on blockmode) if IP is allowed.		*/
/*																	*/
/********************************************************************/
BOOL CFTPServer::IsIPAddressAllowed(LPCTSTR lpszIPAddress)
{
	if (m_nSecurityMode == 0)
	{
		return !m_SecurityManager.IsIPAddressBlocked(lpszIPAddress);
	}
	else
	{
		return m_SecurityManager.IsIPAddressNonBlocked(lpszIPAddress);
	}
}
