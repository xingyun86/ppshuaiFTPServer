
// ApplicationDlg.h : header file
//

#if !defined(AFX_APPLICATIONDLG_H__D3E3BA87_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_)
#define AFX_APPLICATIONDLG_H__D3E3BA87_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TracePage.h"
#include "OnlineUsersPage.h"
#include "ConfigurationPage.h"
#include "StatisticsPage.h"
#include "SecurityPage.h"
#include "DialogResize.h"
#include "FTPEventSink.h"
#include "InfobarCtrl.h"

class CApplicationDlg : public CDialogResize, CFTPEventSink
{
// Construction
public:
	CApplicationDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CApplicationDlg();	
	
	CTracePage m_TracePage;
	COnlineUsersPage m_OnlineUsersPage;
	CConfigurationPage m_ConfigurationPage;
	CStatisticsPage m_StatisticsPage;
	CSecurityPage m_SecurityPage;

// Dialog Data
	//{{AFX_DATA(CApplicationDlg)
	enum { IDD = IDD_APPLICATION };
	CInfobarCtrl m_InfobarCtrl;
	CListCtrl	m_OutlookBar;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplicationDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	char* FormatSize(DWORD dwSizeLow, DWORD dwSizeHigh);

	void SetOnlineLed(BOOL bOnline);
	void SetOfflineLed(BOOL bOffline);
	BOOL m_bInitialized;
	HICON m_hIcon;

	void SetupOutlookBar();
	CImageList m_ImageList;

	CToolTipCtrl m_ToolTipCtrl;

	void MoveChilds();
	void ActivatePage(int nIndex);

	BOOL CreateStatusbar();
	CStatusBar m_wndStatusBar;
    CToolBar m_wndToolBar;

	// Generated message map functions
	//{{AFX_MSG(CApplicationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnServerStart();
	afx_msg void OnUpdateServerStart(CCmdUI* pCmdUI);
	afx_msg void OnServerStop();
	afx_msg void OnUpdateServerStop(CCmdUI* pCmdUI);
	afx_msg void OnServerExit();
	afx_msg void OnClickOutlookBar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownOutlookBar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUserAccounts();
	afx_msg void OnClose();
	afx_msg void OnViewTrace();
	afx_msg void OnViewOnlineUsers();
	afx_msg void OnViewConfiguration();
	afx_msg void OnUpdateViewTrace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOnlineUsers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewConfiguration(CCmdUI* pCmdUI);
	afx_msg void OnHelpAbout();
	afx_msg void OnApply();
	afx_msg void OnViewStatistics();
	afx_msg void OnUpdateViewStatistics(CCmdUI* pCmdUI);
	afx_msg void OnViewSecurity();
	afx_msg void OnUpdateViewSecurity(CCmdUI* pCmdUI);
	afx_msg void OnAccountWizard();
	//}}AFX_MSG
	LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	LRESULT OnFtpStatistic(WPARAM wParam, LPARAM lParam);
    BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	DECLARE_DLGRESIZE_MAP;

	virtual void OnFTPUserConnected(DWORD nThreadID, LPCTSTR lpszUser, LPCSTR lpszAddress);
	virtual void OnFTPUserDisconnected(DWORD nThreadID, LPCTSTR lpszUser);
	virtual void OnFTPStatusChange(int nType, LPCTSTR lpszText);
	virtual void OnFTPReceivedBytesChange(int nBytes);
	virtual void OnFTPSentBytesChange(int nBytes);
	virtual void OnFTPStatisticChange(int nType, int nValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLICATIONDLG_H__D3E3BA87_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_)
