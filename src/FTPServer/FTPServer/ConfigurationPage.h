
#if !defined(AFX_SETTINGSPAGE_H__4A3EEDBC_1D9F_407C_8A55_8449F2B2D0F6__INCLUDED_)
#define AFX_SETTINGSPAGE_H__4A3EEDBC_1D9F_407C_8A55_8449F2B2D0F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogResize.h"

class CConfigurationPage : public CDialogResize
{
// Construction
public:
	CConfigurationPage();
	~CConfigurationPage();

// Dialog Data
	//{{AFX_DATA(CConfigurationPage)
	enum { IDD = IDD_PROPPAGE_CONFIGURATION };
	int		m_nPort;
	int		m_nMaxUsers;
	CString	m_strGoodbyeMessage;
	BOOL	m_bStartInTray;
	int		m_nTimeout;
	CString	m_strWelcomeMessage;
	BOOL	m_bLaunchOnStartup;
	BOOL	m_bAutoActivate;
	int		m_nLogLevel;
	CString	m_strApplicationName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigurationPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bModified;

	// Generated message map functions
	//{{AFX_MSG(CConfigurationPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnLaunchOnStartup();
	afx_msg void OnView();
	afx_msg void OnClear();
	afx_msg void OnUpdateApply(CCmdUI* pCmdUI);
	afx_msg void OnSomethingChanged();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DLGRESIZE_MAP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSPAGE_H__4A3EEDBC_1D9F_407C_8A55_8449F2B2D0F6__INCLUDED_)
