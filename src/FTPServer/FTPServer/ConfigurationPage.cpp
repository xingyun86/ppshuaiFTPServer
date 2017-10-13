
/****************************************************************/
/*																*/
/*  ConfigurationPage.cpp										*/
/*																*/
/*  Implementation of the CConfigurationPage class.				*/
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
#include "FTPServerApp.h"
#include "FTPServer.h"
#include "ConfigurationPage.h"
#include "UserAccountsDlg.h"
#include "ApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFTPServer theServer;
extern CFTPServerApp theApp;

CConfigurationPage::CConfigurationPage() : CDialogResize(CConfigurationPage::IDD)
{
	//{{AFX_DATA_INIT(CConfigurationPage)
	m_nPort = 0;
	m_nMaxUsers = 0;
	m_strGoodbyeMessage = _T("");
	m_bStartInTray = FALSE;
	m_nTimeout = 0;
	m_strWelcomeMessage = _T("");
	m_bLaunchOnStartup = FALSE;
	m_bAutoActivate = FALSE;
	m_nLogLevel = 0;
	m_strApplicationName = _T("");
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
}

CConfigurationPage::~CConfigurationPage()
{
}

void CConfigurationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigurationPage)
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDX_Text(pDX, IDC_MAX_USERS, m_nMaxUsers);
	DDX_Text(pDX, IDC_GOODBYE_MSG, m_strGoodbyeMessage);
	DDX_Check(pDX, IDC_START_IN_TRAY, m_bStartInTray);
	DDX_Text(pDX, IDC_TIMEOUT, m_nTimeout);
	DDX_Text(pDX, IDC_WELCOME_MSG, m_strWelcomeMessage);
	DDX_Check(pDX, IDC_LAUNCH_ON_STARTUP, m_bLaunchOnStartup);
	DDX_Check(pDX, IDC_AUTOACTIVATE, m_bAutoActivate);
	DDX_CBIndex(pDX, IDC_LOGLEVEL, m_nLogLevel);
	DDX_Text(pDX, IDC_APPLICATIONNAME, m_strApplicationName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigurationPage, CDialogResize)
	//{{AFX_MSG_MAP(CConfigurationPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LAUNCH_ON_STARTUP, OnLaunchOnStartup)
	ON_BN_CLICKED(IDC_VIEW, OnView)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_UPDATE_COMMAND_UI(IDC_APPLY, OnUpdateApply)
	ON_EN_CHANGE(IDC_PORT, OnSomethingChanged)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_EN_CHANGE(IDC_MAX_USERS, OnSomethingChanged)
	ON_EN_CHANGE(IDC_TIMEOUT, OnSomethingChanged)
	ON_EN_CHANGE(IDC_WELCOME_MSG, OnSomethingChanged)
	ON_EN_CHANGE(IDC_GOODBYE_MSG, OnSomethingChanged)
	ON_CBN_SELCHANGE(IDC_LOGLEVEL, OnSomethingChanged)
	ON_BN_CLICKED(IDC_START_IN_TRAY, OnSomethingChanged)
	ON_BN_CLICKED(IDC_AUTOACTIVATE, OnSomethingChanged)
	ON_EN_CHANGE(IDC_APPLICATIONNAME, OnSomethingChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CConfigurationPage)
	DLGRESIZE_CONTROL(IDC_WELCOME_MSG, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_GOODBYE_MSG, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_APPLICATIONNAME, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_VIEW, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_CLEAR, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_APPLY, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_IPLIST, DLSZ_SIZE_Y)
END_DLGRESIZE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_INITDIALOG message.							*/
/*																	*/
/********************************************************************/
BOOL CConfigurationPage::OnInitDialog() 
{
	CDialogResize::OnInitDialog();
	
	InitResizing(FALSE, FALSE, WS_CLIPCHILDREN);
	UpdateDialogControls(this, FALSE);
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnDestroy										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_DESTROY message.								*/
/*																	*/
/********************************************************************/
void CConfigurationPage::OnDestroy() 
{
	UpdateData();

	// save settings
	AfxGetApp()->WriteProfileInt("Settings", "Port", m_nPort);
	AfxGetApp()->WriteProfileInt("Settings", "MaxUsers", m_nMaxUsers);
	AfxGetApp()->WriteProfileInt("Settings", "Timeout", m_nTimeout);
	AfxGetApp()->WriteProfileString("Settings", "WelcomeMsg", m_strWelcomeMessage); 
	AfxGetApp()->WriteProfileString("Settings", "GoodbyeMsg", m_strGoodbyeMessage); 
	AfxGetApp()->WriteProfileInt("Settings", "LaunchOnStartup", m_bLaunchOnStartup);
	AfxGetApp()->WriteProfileInt("Settings", "StartInTray", m_bStartInTray);
	AfxGetApp()->WriteProfileInt("Settings", "AutoActivate", m_bAutoActivate);
	AfxGetApp()->WriteProfileInt("Settings", "LogLevel", m_nLogLevel);
	AfxGetApp()->WriteProfileString("Settings", "AppName", m_strApplicationName);
	CDialogResize::OnDestroy();
}


/********************************************************************/
/*																	*/
/* Function name : OnLaunchOnStartup								*/		
/* Description   : Launch application on startup?					*/
/*																	*/
/********************************************************************/
void CConfigurationPage::OnLaunchOnStartup() 
{
	UpdateData(TRUE);
	
	if (m_bLaunchOnStartup)
	{
		char szModuleFilename[MAX_PATH];
		
		// Get application's full path.
		::GetModuleFileName(NULL, szModuleFilename, MAX_PATH);
		// create shortcut
		CreateStartMenuShortcut(szModuleFilename, "FTP Server", "Startup");
	}
	else
	{
		// remove shortcut
		RemoveStartMenuShortcut("FTP Server", "Startup");
	}	
	OnSomethingChanged();
}


/********************************************************************/
/*																	*/
/* Function name : OnView											*/		
/* Description   : View log file contents.							*/	
/*																	*/
/********************************************************************/
void CConfigurationPage::OnView() 
{
    int ExecStatus = (UINT)ShellExecute(m_hWnd, "open", "NOTEPAD.EXE", (LPCTSTR)theApp.m_LogFile.m_FileName, 0, SW_SHOW);
    switch(ExecStatus)
    {
        case 0:
            MessageBox("Systeem is out of Memory","ShellExecute Error",MB_OK | MB_ICONEXCLAMATION);
            break;
        case 2:
            MessageBox("'NOTEPAD.EXE' is not found !","ShellExecute Error",MB_OK | MB_ICONEXCLAMATION);
            break;
        default:
            if (ExecStatus <= 32)
                MessageBox("Problem with executing of NOTEPAD.EXE","ShellExecute Error",MB_OK | MB_ICONEXCLAMATION);
            break;
    }	
}


/********************************************************************/
/*																	*/
/* Function name : OnClear											*/		
/* Description   : Clear log file contents.							*/	
/*																	*/
/********************************************************************/
void CConfigurationPage::OnClear() 
{
    theApp.m_LogFile.Clear();	
}


/********************************************************************/
/*																	*/
/* Function name : OnApply											*/		
/* Description   : Update FTP server settings.						*/	
/*																	*/
/********************************************************************/
void CConfigurationPage::OnApply() 
{
	m_bModified = FALSE;
	UpdateDialogControls(this, FALSE);	
	
	AfxGetMainWnd()->SendMessage(WM_COMMAND, IDC_APPLY);	

	// save settings
	AfxGetApp()->WriteProfileInt("Settings", "Port", m_nPort);
	AfxGetApp()->WriteProfileInt("Settings", "MaxUsers", m_nMaxUsers);
	AfxGetApp()->WriteProfileInt("Settings", "Timeout", m_nTimeout);
	AfxGetApp()->WriteProfileString("Settings", "WelcomeMsg", m_strWelcomeMessage); 
	AfxGetApp()->WriteProfileString("Settings", "GoodbyeMsg", m_strGoodbyeMessage); 
	AfxGetApp()->WriteProfileInt("Settings", "LaunchOnStartup", m_bLaunchOnStartup);
	AfxGetApp()->WriteProfileInt("Settings", "StartInTray", m_bStartInTray);
	AfxGetApp()->WriteProfileInt("Settings", "AutoActivate", m_bAutoActivate);
	AfxGetApp()->WriteProfileInt("Settings", "LogLevel", m_nLogLevel);
	AfxGetApp()->WriteProfileString("Settings", "AppName", m_strApplicationName);
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateApply									*/		
/* Description   : Update button state.								*/	
/*																	*/
/********************************************************************/
void CConfigurationPage::OnUpdateApply(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bModified);			
}


/********************************************************************/
/*																	*/
/* Function name : OnSomethingChanged								*/
/* Description   : Something has changed...							*/
/*																	*/
/********************************************************************/
void CConfigurationPage::OnSomethingChanged() 
{
	m_bModified = TRUE;	
	UpdateDialogControls(this, FALSE);	
}

