
/****************************************************************/
/*																*/
/*  LISTSOCKET.CPP												*/
/*																*/
/*  Implementation of the Listen Socket.						*/
/*  The server listens for connections. When a new connection	*/
/*  is requested, the server accepts the connection and then	*/
/*  creates a connect thread to handle the connection.			*/
/*																*/
/*  Programmed by xingyun86							*/
/*  Copyright @2017					*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "FTPServerApp.h"	
#include "FTPServer.h"	
#include "ApplicationDlg.h"
#include "ListenSocket.h"
#include "ConnectThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CListenSocket::CListenSocket()
{
	m_pWndServer = NULL;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


/********************************************************************/
/*																	*/
/* Function name : OnAccept											*/		
/* Description   : Called by the framework to notify this listening	*/
/*				   socket that it can accept pending connection		*/
/*				   requests by calling the Accept member function.	*/
/*																	*/
/********************************************************************/
void CListenSocket::OnAccept(int nErrorCode) 
{
	// New connection is being established
	CSocket sockit;

	// Accept the connection using a temp CSocket object.
	Accept(sockit);

	// Create a thread to handle the connection. The thread is created suspended so that we can
	// set variables in CConnectThread before it starts executing.
	CConnectThread* pThread = (CConnectThread*)AfxBeginThread(RUNTIME_CLASS(CConnectThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (!pThread)
	{
		sockit.Close();
		TRACE("Could not create thread\n");
		return;
	}

	CFTPServer *pWnd = (CFTPServer *)m_pWndServer;

    // since everything is successful, add the thread to our list
	pWnd->m_CriticalSection.Lock();
    pWnd->m_ThreadList.AddTail(pThread);
	pWnd->m_CriticalSection.Unlock();

	// save pointer
	pThread->m_pWndServer = m_pWndServer;

	// Pass the socket to the thread by passing the socket handle. You cannot pass
	// a CSocket object across threads.
	pThread->m_hSocket = sockit.Detach();

	// Now start the thread.
	pThread->ResumeThread();

	CAsyncSocket::OnAccept(nErrorCode);
}
