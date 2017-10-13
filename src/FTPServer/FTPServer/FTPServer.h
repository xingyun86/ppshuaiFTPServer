
// FTPServer.h: interface for the CFTPServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPSERVER_H__144E8B64_2004_4709_B55A_242FE5F07BD2__INCLUDED_)
#define AFX_FTPSERVER_H__144E8B64_2004_4709_B55A_242FE5F07BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConnectThread.h"
#include "ListenSocket.h"
#include "FTPEventSink.h"
#include "UserManager.h"
#include "SecurityManager.h"

class CFTPServer : public CWnd
{
	friend CConnectSocket;

public:
	void SetGoodbyeMessage(LPCTSTR lpszText);
	void SetWelcomeMessage(LPCTSTR lpszText);
	void SetTimeout(int nValue);
	void SetPort(int nValue);
	void SetMaxUsers(int nValue);
	void SetStatisticsInterval(int nValue);
	BOOL IsActive();
	void Stop();
	BOOL Start();
	CFTPServer();
	virtual ~CFTPServer();

	CUserManager m_UserManager;
	CSecurityManager m_SecurityManager;

	CCriticalSection m_CriticalSection;

    // list of thread pointers
    CTypedPtrList<CObList, CConnectThread*> m_ThreadList;

	int GetPort() { return m_nPort;	};
	int GetMaxUsers() {	return m_nMaxUsers;	}
	int GetTimeout() { return m_nTimeout; }
	int GetConnectionCount() { return m_nConnectionCount; }
	CString GetWelcomeMessage() { return m_strWelcomeMessage; };
	CString GetGoodbyeMessage() { return m_strGoodbyeMessage; };
	void AddTraceLine(int nType, LPCTSTR pstrFormat, ...);

private:
	// socket member that listens for new connections
	CListenSocket m_ListenSocket;
	CFTPEventSink *m_pEventSink;

	int		m_nPort;
	int		m_nMaxUsers;
	CString	m_strWelcomeMessage;
	CString	m_strGoodbyeMessage;
	int		m_nTimeout;
	BOOL	m_bRunning;

	// statistics
	DWORD m_dwTotalReceivedBytes;
	DWORD m_dwTotalSentBytes;
	int	m_nConnectionCount;
	int m_nTotalConnections;
	int m_nFilesDownloaded;
	int m_nFilesUploaded;
	int m_nFailedDownloads;
	int m_nFailedUploads;

	// Operations
public:
	BOOL IsIPAddressAllowed(LPCTSTR lpszIPAddress);
	void SetSecurityMode(BOOL bBlockSpecific = TRUE);
	BOOL CheckMaxUsers();
	void Initialize(CFTPEventSink *pEventSink);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFTPServer)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	int m_nSecurityMode;
	int m_nStatisticsInterval;
	//{{AFX_MSG(CFTPServer)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnThreadClose(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadStart(WPARAM wParam, LPARAM);
	LRESULT OnThreadMessage(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FTPSERVER_H__144E8B64_2004_4709_B55A_242FE5F07BD2__INCLUDED_)
