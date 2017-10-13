
// ConnectThread.h : header file

#if !defined(AFX_CONNECTT_H__B7C54BD2_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
#define AFX_CONNECTT_H__B7C54BD2_A555_11D0_8996_00AA00B92B2E__INCLUDED_

#include "ConnectSocket.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CConnectThread : public CWinThread
{
	DECLARE_DYNCREATE(CConnectThread)
protected:
	CConnectThread();           // protected constructor used by dynamic creation

// Attributes
public:
	void IncReceivedBytes(int nBytes);
	void IncSentBytes(int nBytes);

    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT uIDEvent, DWORD dwTime); 
// Operations
public:
	void UpdateStatistic(int nType);
	CString m_strRemoteHost;
	CWnd *m_pWndServer;
	// Used to pass the socket handle from the main thread to this thread.
	SOCKET m_hSocket;
	
	// CSocket derived class that handles the connection.
	CConnectSocket m_ConnectSocket;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_nTimerID;
	virtual ~CConnectThread();

	int m_nReceivedBytes;
	int m_nSentBytes;
	CTime m_LastDataTransferTime;

	// Generated message map functions
	//{{AFX_MSG(CConnectThread)
	//}}AFX_MSG
	afx_msg void OnThreadMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTT_H__B7C54BD2_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
