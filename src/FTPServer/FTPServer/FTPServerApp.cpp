
/****************************************************************/
/*																*/
/*  FTPServerApp.cpp											*/
/*																*/
/*  Defines the class behaviors for the application.			*/
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
#include "ApplicationDlg.h"
#include "FTPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CFTPServerApp, CWinApp)
	//{{AFX_MSG_MAP(CFTPServerApp)
	ON_COMMAND(ID_APP_SHOW, OnAppShow)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPServerApp construction

CFTPServerApp::CFTPServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFTPServerApp object

CFTPServerApp theApp;

CFTPServer theServer;

/////////////////////////////////////////////////////////////////////////////
// CFTPServerApp initialization

BOOL CFTPServerApp::InitInstance()
{
	SetRegistryKey(_T("Pablo Software Solutions"));

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	InitCommonControls();

	// Create the tray icon
	if (!m_TrayIcon.Create(WM_ICON_NOTIFY,			// Icon notify message to use
					_T("Pablo's FTP Server"),		// tooltip
					LoadIcon(IDI_FTPSERVER),		// Icon
					IDR_POPUP_MENU))				// ID of tray icon
		return -1;

	int nLogLevel = GetProfileInt("Settings", "LogLevel" , 1);

	if (nLogLevel)
	{
		// create configuration filename
		CString strFileName;

		GetAppDir(strFileName);
		strFileName += "ftptrace.txt";

		m_LogFile.SetLogLevel(nLogLevel);

		// open log file
		m_LogFile.Open((LPCTSTR)strFileName);
	}

	CApplicationDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CFTPServerApp::OnAppShow() 
{
	if (m_pMainWnd)
		m_pMainWnd->ShowWindow(SW_SHOW);	
}

void CFTPServerApp::OnAppExit() 
{
	m_pMainWnd->DestroyWindow();
}

void CFTPServerApp::OnAppAbout() 
{
//    CSettingsSheet propsheet("Settings", AfxGetApp()->m_pMainWnd, 2);

    // remove Help button
  //  propsheet.m_psh.dwFlags &= ~(PSH_HASHELP);

	// display Settings dialogbox
  //  propsheet.DoModal();
}


/********************************************************************/
/*																	*/
/* Function name : OnHelpIndex										*/
/* Description   : Command to show help file.						*/
/*																	*/
/********************************************************************/
void CFTPServerApp::OnHelpIndex() 
{
	// launch help
	::WinHelp(AfxGetMainWnd()->m_hWnd, AfxGetApp()->m_pszHelpFilePath, HELP_CONTENTS, 0L);	
}
