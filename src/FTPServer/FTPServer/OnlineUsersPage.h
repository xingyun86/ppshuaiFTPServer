
#if !defined(AFX_ONLINEUSERSPAGE_H__749D54B1_2C20_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_ONLINEUSERSPAGE_H__749D54B1_2C20_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnlineUsersPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COnlineUsersPage dialog

class COnlineUsersPage : public CDialog
{
	DECLARE_DYNCREATE(COnlineUsersPage)

// Construction
public:
	void RemoveUser(DWORD nThreadID);
	void AddUser(DWORD nThreadID, LPCTSTR lpszName, LPCTSTR lpszAddress);
	COnlineUsersPage();
	~COnlineUsersPage();

// Dialog Data
	//{{AFX_DATA(COnlineUsersPage)
	enum { IDD = IDD_PROPPAGE_ONLINE };
	CListCtrl	m_OnlineUsers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COnlineUsersPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COnlineUsersPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKickUser();
	afx_msg void OnEditUserAccount();
	afx_msg void OnBlockIp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLINEUSERSPAGE_H__749D54B1_2C20_11D6_AB38_00D0B70C3D79__INCLUDED_)
