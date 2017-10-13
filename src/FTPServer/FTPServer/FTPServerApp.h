
// FTPServerApp.h : main header file for the FTPServerApp application
//

#if !defined(AFX_FTPSERVERAPP_H__D3E3BA85_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_)
#define AFX_FTPSERVERAPP_H__D3E3BA85_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SystemTray.h"
#include "LogFile.h"

class CFTPServerApp : public CWinApp
{
public:
	CFTPServerApp();
	CSystemTray m_TrayIcon;
	CLogFile m_LogFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFTPServerApp)
	afx_msg void OnAppShow();
	afx_msg void OnAppExit();
	afx_msg void OnAppAbout();
	afx_msg void OnHelpIndex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSERVERAPP_H__D3E3BA85_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_)
