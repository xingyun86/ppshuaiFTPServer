
/****************************************************************/
/*																*/
/*  CONNECTTHREAD.CPP											*/
/*																*/
/*  Implementation of the Connect Thread.						*/
/*	Created when a client logs on to the server and processes	*/
/*  'Send' commando's.											*/
/*																*/
/*  Programmed by xingyun86							*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "FTPServerApp.h"
#include "FTPServer.h"
#include "ApplicationDlg.h"
#include "ConnectThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CConnectThread, CWinThread)


/********************************************************************/
/*																	*/
/* Function name : CConnectThread::CConnectThread					*/
/* Description   : Constructor										*/
/*																	*/
/********************************************************************/
CConnectThread::CConnectThread()
{
	m_nReceivedBytes = 0;
	m_nSentBytes = 0;
	m_nTimerID = 0;
	m_LastDataTransferTime = CTime::GetCurrentTime();
}


/********************************************************************/
/*																	*/
/* Function name : CConnectThread::~CConnectThread					*/
/* Description   : Destructor										*/
/*																	*/
/********************************************************************/
CConnectThread::~CConnectThread()
{
}


/********************************************************************/
/*																	*/
/* Function name : InitInstance										*/		
/* Description   : Perform tasks that must be completed when the	*/
/*				   thread is first created.							*/
/*																	*/
/********************************************************************/
BOOL CConnectThread::InitInstance()
{
	try
	{
		// Attach the socket handle to a CSocket object.
		// This makes sure that the socket notifications are sent to this thread.
		m_ConnectSocket.Attach(m_hSocket);
		m_ConnectSocket.m_pThread = this;

		CString strIPAddress;
		UINT nPort;
		m_ConnectSocket.GetPeerName(strIPAddress, nPort);

		// notify server that there's a new connection
		m_pWndServer->SendMessage(WM_THREADSTART, (WPARAM)this, 0);
		
		if (((CFTPServer *)m_pWndServer)->CheckMaxUsers())
		{
			m_ConnectSocket.SendResponse("421 Too many users are connected, please try again later.");
			PostThreadMessage(WM_QUIT,0,0);
		}
		else
		if (!((CFTPServer *)m_pWndServer)->IsIPAddressAllowed(strIPAddress))
		{
			m_ConnectSocket.SendResponse("421 Access denied, IP address was rejected by the server.");
			PostThreadMessage(WM_QUIT,0,0);
		}
		else
		{
			// send welcome message to client
			CString strText = ((CFTPServer *)m_pWndServer)->GetWelcomeMessage();
			m_ConnectSocket.SendResponse("220 " + strText);
			m_nTimerID = ::SetTimer(NULL, 0, 1000, TimerProc); 
		}
	}
	catch(CException *e) 
	{
		e->Delete();
	}
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : ExitInstance										*/		
/* Description   : Perform clean-up when the thread terminates.		*/
/*																	*/
/********************************************************************/
int CConnectThread::ExitInstance()
{
	CFTPServer *pWnd = (CFTPServer *)m_pWndServer;

	try
	{
		pWnd->m_CriticalSection.Lock();
		
		// delete this thread from the linked list
		POSITION pos = pWnd->m_ThreadList.Find(this);
		if(pos != NULL)
		{
			pWnd->m_ThreadList.RemoveAt(pos);
		}
		pWnd->m_CriticalSection.Unlock();    		

		// notify service main loop
		pWnd->SendMessage(WM_THREADCLOSE, (WPARAM)this, 0);
	
	}
	catch(CException *e) 
	{
		pWnd->m_CriticalSection.Unlock();
		e->Delete();
	}
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CConnectThread, CWinThread)
	//{{AFX_MSG_MAP(CConnectThread)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_THREADMSG, OnThreadMessage)
END_MESSAGE_MAP()



/********************************************************************/
/*																	*/
/* Function name : IncSentBytes										*/		
/* Description   : Increment number of bytes sent by the server.	*/
/*																	*/
/********************************************************************/
void CConnectThread::IncSentBytes(int nBytes)
{
	m_LastDataTransferTime = CTime::GetCurrentTime();
	m_nSentBytes += nBytes;
	// notify server class
	m_pWndServer->PostMessage(WM_THREADMSG, (WPARAM)0, (LPARAM)nBytes);
}


/********************************************************************/
/*																	*/
/* Function name : IncReceivedBytes									*/		
/* Description   : Increment number of bytes received by the server.*/
/*																	*/
/********************************************************************/
void CConnectThread::IncReceivedBytes(int nBytes)
{
	m_LastDataTransferTime = CTime::GetCurrentTime();
	m_nReceivedBytes += nBytes;
	// notify server class
	m_pWndServer->PostMessage(WM_THREADMSG, (WPARAM)1, (LPARAM)nBytes);
}


/********************************************************************/
/*																	*/
/* Function name : UpdateStatistic									*/	
/* Description   : Specific statistics has been changed.			*/
/*																	*/
/********************************************************************/
void CConnectThread::UpdateStatistic(int nType)
{
	// notify server class
	m_pWndServer->PostMessage(WM_THREADMSG, (WPARAM)2, (LPARAM)nType);
}


/********************************************************************/
/*																	*/
/* Function name : OnThreadMessage									*/		
/* Description   : Thread message received.							*/
/*																	*/
/********************************************************************/
void CConnectThread::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 0: // destroy data socket
			m_ConnectSocket.DestroyDataSocket();
			break;
		case 1: // quit !
			PostThreadMessage(WM_QUIT,0,0);
			break;
		default:
			break;
	}
}


/********************************************************************/
/*																	*/
/* Function name : TimerProc										*/	
/* Description   : Callback function for timer.						*/
/*																	*/
/********************************************************************/
VOID CALLBACK CConnectThread::TimerProc(HWND hwnd, UINT uMsg, UINT uIDEvent, DWORD dwTime)
{
	CConnectThread *pThread = (CConnectThread *)AfxGetThread();

	if (uIDEvent == pThread->m_nTimerID)
	{
		int nConnectionTimeout = ((CFTPServer *)pThread->m_pWndServer)->GetTimeout();
		// check for connection timeout
		CTime time = pThread->m_LastDataTransferTime;
		time += CTimeSpan(0, 0, nConnectionTimeout, 0);
		if (time < CTime::GetCurrentTime())
		{
			pThread->m_ConnectSocket.SendResponse("426 Connection timed out, aborting transfer");
			pThread->PostThreadMessage(WM_QUIT,0,0);
		}
	}
} 

