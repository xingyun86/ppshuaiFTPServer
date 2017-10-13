
/****************************************************************/
/*																*/
/*  ApplicationDlg.cpp											*/
/*																*/
/*  Implementation of the CApplicationDlg class.				*/
/*	This class is a part of the FTP Server.						*/
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
#include "UserAccountsDlg.h"
#include "WizardPages.h"
#include "AboutDlg.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFTPServerApp theApp;
extern CFTPServer theServer;

static UINT auIDStatusBar[] = 
{
	ID_SEPARATOR,
	ID_INDICATOR_DATA_RECEIVED,
	ID_INDICATOR_DATA_SEND,
	ID_INDICATOR_ONLINELED,
	ID_INDICATOR_OFFLINELED
};


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::CApplicationDlg					*/
/* Description   : Constructor										*/
/*																	*/
/********************************************************************/
CApplicationDlg::CApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogResize(CApplicationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplicationDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_FTPSERVER);
	m_bInitialized = FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::~CApplicationDlg				*/
/* Description   : Destructor										*/
/*																	*/
/********************************************************************/
CApplicationDlg::~CApplicationDlg()
{
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::DoDataExchange					*/		
/* Description   : Called by the framework to exchange and validate */
/*				   dialog data.										*/
/*																	*/
/********************************************************************/
void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicationDlg)
	DDX_Control(pDX, IDC_INFOBAR, m_InfobarCtrl);
	DDX_Control(pDX, IDC_OUTLOOKBAR, m_OutlookBar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogResize)
	//{{AFX_MSG_MAP(CApplicationDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SERVER_START, OnServerStart)
	ON_UPDATE_COMMAND_UI(ID_SERVER_START, OnUpdateServerStart)
	ON_COMMAND(ID_SERVER_STOP, OnServerStop)
	ON_UPDATE_COMMAND_UI(ID_SERVER_STOP, OnUpdateServerStop)
	ON_COMMAND(ID_SERVER_EXIT, OnServerExit)
	ON_NOTIFY(NM_CLICK, IDC_OUTLOOKBAR, OnClickOutlookBar)
	ON_NOTIFY(LVN_KEYDOWN, IDC_OUTLOOKBAR, OnKeydownOutlookBar)
	ON_COMMAND(ID_USER_ACCOUNTS, OnUserAccounts)
	ON_WM_CLOSE()
	ON_COMMAND(IDC_VIEW_TRACE, OnViewTrace)
	ON_COMMAND(IDC_VIEW_ONLINE_USERS, OnViewOnlineUsers)
	ON_COMMAND(IDC_VIEW_CONFIGURATION, OnViewConfiguration)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_TRACE, OnUpdateViewTrace)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_ONLINE_USERS, OnUpdateViewOnlineUsers)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_CONFIGURATION, OnUpdateViewConfiguration)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(IDC_VIEW_STATISTICS, OnViewStatistics)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_STATISTICS, OnUpdateViewStatistics)
	ON_COMMAND(IDC_VIEW_SECURITY, OnViewSecurity)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_SECURITY, OnUpdateViewSecurity)
	ON_COMMAND(ID_ACCOUNT_WIZARD, OnAccountWizard)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_COMMAND(IDC_APPLY, OnApply)
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CApplicationDlg)
	DLGRESIZE_CONTROL(IDC_OUTLOOKBAR, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_DIALOG_AREA, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(AFX_IDW_STATUS_BAR, DLSZ_MOVE_Y | DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(AFX_IDW_TOOLBAR, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_INFOBAR, DLSZ_SIZE_X)
END_DLGRESIZE_MAP()



/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::OnInitDialog					*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_INITDIALOG message.							*/
/*																	*/
/********************************************************************/
BOOL CApplicationDlg::OnInitDialog()
{
	CDialogResize::OnInitDialog();
	
	// setup listview ctrl, so that it looks like the outbar control
	SetupOutlookBar();

	// Add statusbar to the dialog
    CreateStatusbar();

	CRect rect;
	GetDlgItem(IDC_STATIC1)->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right+=4;
	GetDlgItem(IDC_STATIC1)->MoveWindow(rect);
	
	InitResizing(FALSE);

	int s, t, b, r, l;

	if (AfxGetApp()->GetProfileInt("Settings", "SavePosition", 1))
	{
		// only restore if there is a previously saved position
		if ( -1 != (s = AfxGetApp()->GetProfileInt("Settings", "FrameStatus",   -1)) &&
			 -1 != (t = AfxGetApp()->GetProfileInt("Settings", "FrameTop",      -1)) &&
			 -1 != (l = AfxGetApp()->GetProfileInt("Settings", "FrameLeft",     -1)) &&
			 -1 != (b = AfxGetApp()->GetProfileInt("Settings", "FrameBottom",   -1)) &&
			 -1 != (r = AfxGetApp()->GetProfileInt("Settings", "FrameRight",    -1))
		   ) 
		{
			WINDOWPLACEMENT wp;
			
			// restore the window's status
			wp.showCmd = s;

			// restore the window's width and height
			wp.rcNormalPosition.bottom = b;
			wp.rcNormalPosition.right = r;

			// the following correction is needed when the taskbar is
			// at the left or top and it is not "auto-hidden"
			RECT workArea;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
			l += workArea.left;
			t += workArea.top;

			// make sure the window is not completely out of sight
			int max_x = GetSystemMetrics(SM_CXSCREEN) -
							GetSystemMetrics(SM_CXICON);
			int max_y = GetSystemMetrics(SM_CYSCREEN) -
							GetSystemMetrics(SM_CYICON);
			wp.rcNormalPosition.top = min(t, max_y);
			wp.rcNormalPosition.left = min(l, max_x);

			SetWindowPlacement(&wp);
		}
	}

	// create property pages
	m_TracePage.Create(IDD_PROPPAGE_TRACE, this);
	m_OnlineUsersPage.Create(IDD_PROPPAGE_ONLINE, this);
	m_ConfigurationPage.Create(IDD_PROPPAGE_CONFIGURATION, this);
	m_StatisticsPage.Create(IDD_PROPPAGE_STATISTICS, this);
	m_SecurityPage.Create(IDD_PROPPAGE_SECURITY, this);

	// activate main page 
	ActivatePage(0);

	// Set the icon for this dialog.
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// get registry values
	m_ConfigurationPage.m_nPort = AfxGetApp()->GetProfileInt("Settings", "Port", 21);
	m_ConfigurationPage.m_nMaxUsers = AfxGetApp()->GetProfileInt("Settings", "MaxUsers", 10);
	m_ConfigurationPage.m_nTimeout = AfxGetApp()->GetProfileInt("Settings", "Timeout", 5);
	m_ConfigurationPage.m_strWelcomeMessage = AfxGetApp()->GetProfileString("Settings", "WelcomeMsg", "Welcome to Pablo's FTP Server"); 
	m_ConfigurationPage.m_strGoodbyeMessage = AfxGetApp()->GetProfileString("Settings", "GoodbyeMsg", "Bye"); 
	m_ConfigurationPage.m_bLaunchOnStartup = AfxGetApp()->GetProfileInt("Settings", "LaunchOnStartup", 0);
	m_ConfigurationPage.m_bStartInTray = AfxGetApp()->GetProfileInt("Settings", "StartInTray", 0);
	m_ConfigurationPage.m_bAutoActivate = AfxGetApp()->GetProfileInt("Settings", "AutoActivate", 0);
	m_ConfigurationPage.m_nLogLevel = AfxGetApp()->GetProfileInt("Settings", "LogLevel", 1);
	m_ConfigurationPage.m_strApplicationName = AfxGetApp()->GetProfileString("Settings", "AppName", "Pablo's FTP Server");
	
	m_ConfigurationPage.UpdateData(FALSE);

	SetWindowText(m_ConfigurationPage.m_strApplicationName);

	// set log level
	theApp.m_LogFile.SetLogLevel(m_ConfigurationPage.m_nLogLevel);
	
	// and finally, let's add some tooltips
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	// minimize at startup ?
	if (m_ConfigurationPage.m_bStartInTray)
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		// this seems to be a workaround to let the statement above to get working
		CRect rect;
		GetWindowRect(rect);
		SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(),  rect.Height(), SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}
	
	theServer.Initialize(this);

	if (m_ConfigurationPage.m_bAutoActivate)
	{
		OnServerStart();
	}

	m_bInitialized = TRUE;
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::OnSysCommand					*/		
/* Description   : Here we catch the minimizing						*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	switch(nID)
	{
		case SC_MINIMIZE:
			// do not minimize to the taskbar
			ShowWindow(SW_HIDE);
			break;
		default:
			CDialog::OnSysCommand(nID, lParam);
			break;
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogResize::OnSize(nType, cx, cy);
	
	if (m_bInitialized)
		MoveChilds();
}


/********************************************************************/
/*																	*/
/* Function name : OnClose											*/		
/* Description   : Hide the dialogbox (instead of closing it)		*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnClose() 
{
	if (AfxGetApp()->GetProfileInt("Settings", "SavePosition", 1))
	{
		// Save main window position
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
		AfxGetApp()->WriteProfileInt("Settings", "FrameStatus", wp.showCmd);
		AfxGetApp()->WriteProfileInt("Settings", "FrameTop",    wp.rcNormalPosition.top);
		AfxGetApp()->WriteProfileInt("Settings", "FrameLeft",   wp.rcNormalPosition.left);
		AfxGetApp()->WriteProfileInt("Settings", "FrameBottom", wp.rcNormalPosition.bottom);
		AfxGetApp()->WriteProfileInt("Settings", "FrameRight",  wp.rcNormalPosition.right);
	}	
	CDialog::OnClose();
}


/********************************************************************/
/*																	*/
/* Function name : OnDestroy										*/		
/* Description   : Handle WM_DESTROY message						*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnDestroy() 
{
	OnServerStop();
	CDialogResize::OnDestroy();
}


/********************************************************************/
/*																	*/
/* Function name : OnServerStart									*/		
/* Description   : Start FTP server -> start listening on port 21	*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnServerStart() 
{
	m_ConfigurationPage.UpdateData();
	theServer.SetPort(m_ConfigurationPage.m_nPort);
	theServer.SetMaxUsers(m_ConfigurationPage.m_nMaxUsers);
	theServer.SetTimeout(m_ConfigurationPage.m_nTimeout);
	theServer.SetWelcomeMessage(m_ConfigurationPage.m_strWelcomeMessage);
	theServer.SetGoodbyeMessage(m_ConfigurationPage.m_strGoodbyeMessage);
	theServer.SetStatisticsInterval(5000);
	BOOL bBlockAll = AfxGetApp()->GetProfileInt("Settings", "BlockAll", 0);
	theServer.SetSecurityMode(!bBlockAll);

	if (theServer.Start())
	{
		m_wndStatusBar.SetPaneText(0, "FTP Server is online", TRUE);
		SetOnlineLed(TRUE);
		SetOfflineLed(FALSE);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateServerStart								*/		
/* Description   : Update 'Start' status							*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateServerStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theServer.IsActive());	
}


/********************************************************************/
/*																	*/
/* Function name : OnServerStop										*/		
/* Description   : Stop FTP server									*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnServerStop() 
{
	theServer.Stop();
	m_wndStatusBar.SetPaneText(0, "FTP Server is offline", TRUE);
	SetOnlineLed(FALSE);
	SetOfflineLed(TRUE);
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateServerStop								*/		
/* Description   : Update 'Stop' status								*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateServerStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theServer.IsActive());
}


/********************************************************************/
/*																	*/
/* Function name : OnKickIdle										*/		
/* Description   : Make ON_UPDATE_COMMAND_UI work for this dialog.	*/
/*																	*/
/********************************************************************/
LRESULT CApplicationDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UpdateDialogControls(this, FALSE);

	CMenu* pMainMenu = GetMenu();
    CCmdUI cmdUI;
    // update menu enable/disable state
    for (UINT n = 0; n < pMainMenu->GetMenuItemCount(); ++n)
    {
        CMenu* pSubMenu = pMainMenu->GetSubMenu(n);
        cmdUI.m_nIndexMax = pSubMenu->GetMenuItemCount();
        for (UINT i = 0; i < cmdUI.m_nIndexMax;++i)
        {
            cmdUI.m_nIndex = i;
            cmdUI.m_nID = pSubMenu->GetMenuItemID(i);
            cmdUI.m_pMenu = pSubMenu;
            cmdUI.DoUpdate(this, FALSE);
        }
    }
	// enable/disable toolbar buttons    
	if (m_wndToolBar.IsWindowVisible()) 
	{
		// OnUpdateCmdUI expects a CFrameWnd pointer, so let's fake it ..
		CFrameWnd *pParent = (CFrameWnd *)this;
		if (pParent)
			m_wndToolBar.OnUpdateCmdUI(pParent, TRUE);
	} 

	return Default();
}


/********************************************************************/
/*																	*/
/* Function name : OnHelpAbout										*/		
/* Description   : Show Aboutbox.									*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnHelpAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}


/********************************************************************/
/*																	*/
/* Function name : OnServerExit										*/		
/* Description   : Let's get out of here...							*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnServerExit() 
{
	OnCancel();	
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::SetupOutlookBar					*/		
/* Description   : Initialize listview, change color/iconspacing to	*/
/*				   make it look a little bit like the outlook bar.	*/
/*																	*/
/********************************************************************/
void CApplicationDlg::SetupOutlookBar()
{
    // create Imagelist
	m_ImageList.Create(32, 32, ILC_COLOR16|ILC_MASK,1, 4);

    HICON hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_LOG));
    m_ImageList.Add(hIcon);
    hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ONLINE_USERS));
    m_ImageList.Add(hIcon);
    hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_CONFIGURATION));
    m_ImageList.Add(hIcon);
    hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_STATISTICS));
    m_ImageList.Add(hIcon);
    hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_SECURITY));
    m_ImageList.Add(hIcon);
	
	m_OutlookBar.SetImageList(&m_ImageList, LVSIL_NORMAL);

	CRect rc;
	m_OutlookBar.GetClientRect(rc);
	
	// set new icon spacing
	m_OutlookBar.SetIconSpacing(rc.Width(), 64);

	// change colors
	m_OutlookBar.SetTextColor(RGB(255,255,255));
	m_OutlookBar.SetTextBkColor(RGB(128,128,128));
	m_OutlookBar.SetBkColor(RGB(128,128,128));	

	// insert items
	m_OutlookBar.InsertColumn(0, "OutlookBar");

	m_OutlookBar.InsertItem(0, "Server Log", 0);
	m_OutlookBar.InsertItem(1, "Online Users", 1);
	m_OutlookBar.InsertItem(2, "Configuration", 2);
	m_OutlookBar.InsertItem(3, "Statistics", 3);
	m_OutlookBar.InsertItem(4, "Security", 4);

//	m_OutlookBar.SetExtendedStyle(LVS_EX_TRACKSELECT);
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::MoveChilds						*/		
/* Description   : Move child windows into place holder area.		*/
/*																	*/
/********************************************************************/
void CApplicationDlg::MoveChilds()
{
	// position property pages 
	CRect rcDlgs;
	
	// get dialog area rect
	GetDlgItem(IDC_DIALOG_AREA)->GetWindowRect(rcDlgs);
	
	ScreenToClient(rcDlgs);
	
	m_ConfigurationPage.MoveWindow(rcDlgs); 
	m_TracePage.MoveWindow(rcDlgs); 
	m_OnlineUsersPage.MoveWindow(rcDlgs); 
	m_StatisticsPage.MoveWindow(rcDlgs); 
	m_SecurityPage.MoveWindow(rcDlgs); 
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::ActivatePage					*/
/* Description   : Called when an icon on the outlookbar is pressed.*/
/*																	*/
/********************************************************************/
void CApplicationDlg::ActivatePage(int nIndex)
{
	switch(nIndex)
	{
		case 0:
			m_OnlineUsersPage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			m_SecurityPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_SHOW);
			m_InfobarCtrl.SetText("Server Log");
			break;
		case 1:
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			m_SecurityPage.ShowWindow(SW_HIDE);	
			m_OnlineUsersPage.ShowWindow(SW_SHOW);
			m_InfobarCtrl.SetText("Online Users");
			break;
		case 2:
			m_OnlineUsersPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			m_SecurityPage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_SHOW);	
			m_InfobarCtrl.SetText("Configuration");
			break;
		case 3:
			m_OnlineUsersPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_SecurityPage.ShowWindow(SW_HIDE);	
			m_StatisticsPage.ShowWindow(SW_SHOW);	
			m_InfobarCtrl.SetText("Statistics");
			break;
		case 4:
			m_OnlineUsersPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_SecurityPage.ShowWindow(SW_SHOW);	
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			m_InfobarCtrl.SetText("Security");
			break;
		default:
			break;
	}
	MoveChilds();
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::OnClickOutlookBar				*/
/* Description   : User clicked on our listview -> activate page.	*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnClickOutlookBar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// get index of selected item
	int nIndex = m_OutlookBar.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;
		
	ActivatePage(nIndex);

	*pResult = 0;
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::OnKeydownOutlookBar				*/
/* Description   : User pressed a key -> activate page.				*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnKeydownOutlookBar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

    // get index of selected item
	int nIndex = m_OutlookBar.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;

	if (pLVKeyDow->wVKey == VK_DOWN)
	{
		if (m_OutlookBar.GetItemCount()-1 > nIndex)
		{
			ActivatePage(nIndex+1);
		}
	}
	else
	if (pLVKeyDow->wVKey == VK_UP)
	{
		if (nIndex > 0)
		{
			ActivatePage(nIndex-1);
		}
	}
	else
	if (pLVKeyDow->wVKey == VK_NEXT)
	{
		ActivatePage(m_OutlookBar.GetItemCount()-1);
	}
	else
	if (pLVKeyDow->wVKey == VK_PRIOR)
	{
		ActivatePage(0);
	}

	*pResult = 0;
}



/********************************************************************/
/*																	*/
/* Function name : CreateStatusbar									*/	
/* Description   : This function creates a statusbar on a dialogbox.*/
/*																	*/
/********************************************************************/
BOOL CApplicationDlg::CreateStatusbar()
{
	// Create the Toolbar and attach the resource
	if(!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to Create Dialog Toolbar\n");
		return FALSE;	
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	// Make it flat.
	m_wndToolBar.ModifyStyle(0, m_wndToolBar.GetStyle()|TBSTYLE_FLAT);

	// Create status bar at the bottom of the dialog window
	if (m_wndStatusBar.Create(this))
	{
		m_wndStatusBar.SetIndicators(auIDStatusBar, sizeof(auIDStatusBar)/sizeof(UINT));
		
		m_wndStatusBar.SetPaneText(0, "FTP Server is offline", TRUE);
		// Make a sunken or recessed border around the first pane
		m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL );

		SetOnlineLed(FALSE);
		SetOfflineLed(FALSE);

		m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_ONLINELED),ID_INDICATOR_ONLINELED, SBPS_NOBORDERS, 14);
//		m_wndStatusBar.GetStatusBarCtrl().SetTipText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_ONLINELED), "This status light is green when the server is online");

		m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_OFFLINELED),ID_INDICATOR_OFFLINELED, SBPS_NOBORDERS, 14);
//		m_wndStatusBar.GetStatusBarCtrl().SetTipText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_OFFLINELED), "This status light is green when the server is online");
	}

    // We need to resize the dialog to make room for control bars.
	// First, figure out how big the control bars are.
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

    // Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left, rcClientNow.top - rcClientStart.top); 

	CRect  rcChild;					
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{                               
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	
	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnUserAccounts									*/		
/* Description   : Show user accounts.								*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUserAccounts() 
{
	CUserAccountsDlg dlg;

	theServer.m_UserManager.GetUserList(dlg.m_UserArray);
	if (dlg.DoModal() == IDOK)
	{
		theServer.m_UserManager.UpdateUserList(dlg.m_UserArray);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnFTPUserConnected								*/
/* Description   : A user has connected to our server.				*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPUserConnected(DWORD nThreadID, LPCTSTR lpszUser, LPCSTR lpszAddress)
{
	m_OnlineUsersPage.AddUser(nThreadID, lpszUser, lpszAddress);
}


/********************************************************************/
/*																	*/
/* Function name : OnFTPUserDisconnected							*/
/* Description   : A user has disconnected from our server.			*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPUserDisconnected(DWORD nThreadID, LPCTSTR lpszUser)
{
	m_OnlineUsersPage.RemoveUser(nThreadID);
}


/********************************************************************/
/*																	*/
/* Function name : OnFTPStatusChange								*/		
/* Description   : FTP Status changed.								*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPStatusChange(int nType, LPCTSTR lpszText)
{
	m_TracePage.AddTraceLine(nType, lpszText);
	
	switch(nType)
	{
		case 3:
			theApp.m_LogFile << error_lvl << date << time << lpszText << endl;
			break;
		default:
			theApp.m_LogFile << trace_lvl << date << time << lpszText << endl;
			break;
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnFTPReceivedBytesChange							*/		
/* Description   : Number of received bytes has changed.			*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPReceivedBytesChange(int nBytes) 
{
	m_StatisticsPage.SetValue(6, FormatSize(nBytes, 0));

	CString strStatus;
	strStatus.Format("%s received", FormatSize(nBytes, 0));
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_DATA_RECEIVED), strStatus, TRUE);

	CClientDC dc(this);
	CSize size = dc.GetTextExtent(strStatus);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_DATA_RECEIVED), ID_INDICATOR_DATA_RECEIVED, SBPS_NORMAL, size.cx);
}


/********************************************************************/
/*																	*/
/* Function name : OnFTPSentBytesChange								*/		
/* Description   : Number of sent bytes has changed.				*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPSentBytesChange(int nBytes)
{
	m_StatisticsPage.SetValue(7, FormatSize(nBytes, 0));

	CString strStatus;
	strStatus.Format("%s sent",  FormatSize(nBytes, 0));
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_DATA_SEND), strStatus, TRUE);
	CClientDC dc(this);
	CSize size = dc.GetTextExtent(strStatus);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_DATA_SEND), ID_INDICATOR_DATA_SEND, SBPS_NORMAL, size.cx);
}


/********************************************************************/
/*																	*/
/* Function name : OnViewTrace										*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnViewTrace() 
{
	ActivatePage(0);
}


/********************************************************************/
/*																	*/
/* Function name : OnViewOnlineUsers								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnViewOnlineUsers() 
{
	ActivatePage(1);
}


/********************************************************************/
/*																	*/
/* Function name : OnViewConfiguration								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnViewConfiguration() 
{
	ActivatePage(2);
}


/********************************************************************/
/*																	*/
/* Function name : OnViewStatistics									*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnViewStatistics() 
{
	ActivatePage(3);
}


/********************************************************************/
/*																	*/
/* Function name : OnViewSecurity									*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnViewSecurity() 
{
	ActivatePage(4);
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewTrace								*/		
/* Description   : Update menu state of Trace item.					*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateViewTrace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_TracePage.IsWindowVisible());	
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewOnlineUsers							*/		
/* Description   : Update menu state of Online Users item.			*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateViewOnlineUsers(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_OnlineUsersPage.IsWindowVisible());		
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewConfiguration						*/		
/* Description   : Update menu state of Configuration item.			*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateViewConfiguration(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_ConfigurationPage.IsWindowVisible());			
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewStatistics							*/		
/* Description   : Update menu state of Statistics item.			*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateViewStatistics(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_StatisticsPage.IsWindowVisible());				
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewSecurity								*/		
/* Description   : Update menu state of Security item.				*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnUpdateViewSecurity(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_SecurityPage.IsWindowVisible());					
}


/********************************************************************/
/*																	*/
/* Function name : PreTranslateMessage								*/	
/* Description   : Needed to make tooltips work.					*/
/*																	*/
/********************************************************************/
BOOL CApplicationDlg::PreTranslateMessage(MSG* pMsg) 
{
	// enable tooltips
	if (m_hWnd)
    {
        if (IsWindow(m_ToolTipCtrl.m_hWnd))
			m_ToolTipCtrl.RelayEvent(pMsg);
    }
	
	return CDialogResize::PreTranslateMessage(pMsg);
}


/********************************************************************/
/*																	*/
/* Function name : OnToolTipText									*/	
/* Description   : Tooltip handler for this dialog.					*/
/*																	*/
/********************************************************************/
BOOL CApplicationDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
    ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString cstTipText;
	CString cstStatusText;

	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	if (nID != 0) // will be zero on a separator
	{
		AfxLoadString(nID, szFullText);
			// this is the command id, not the button index
		AfxExtractSubString(cstTipText, szFullText, 1, '\n');
		AfxExtractSubString(cstStatusText, szFullText, 0, '\n');
	}

	// Non-UNICODE Strings only are shown in the tooltip window...
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, cstTipText,
            (sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0])));
	else
		_mbstowcsz(pTTTW->szText, cstTipText,
            (sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0])));
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

    // message was handled
	return TRUE;    
}


/********************************************************************/
/*																	*/
/* Function name : SetOnlineLed										*/	
/* Description   : Turn online LED on/off.							*/
/*																	*/
/********************************************************************/
void CApplicationDlg::SetOnlineLed(BOOL bOnline)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
				bOnline ? MAKEINTRESOURCE(IDI_LED_GREEN) : MAKEINTRESOURCE(IDI_LED_OFF),
				 IMAGE_ICON, 16, 16, LR_SHARED);
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(m_wndStatusBar.CommandToIndex(ID_INDICATOR_ONLINELED), hIcon);
	m_wndStatusBar.GetStatusBarCtrl().Invalidate();
	m_wndStatusBar.GetStatusBarCtrl().UpdateWindow();
	DestroyIcon(hIcon);
}


/********************************************************************/
/*																	*/
/* Function name : SetOfflineLed									*/	
/* Description   : Turn offline LED on/off.							*/
/*																	*/
/********************************************************************/
void CApplicationDlg::SetOfflineLed(BOOL bOffline)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
				bOffline ? MAKEINTRESOURCE(IDI_LED_RED) : MAKEINTRESOURCE(IDI_LED_OFF),
				 IMAGE_ICON, 16, 16, LR_SHARED);
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(m_wndStatusBar.CommandToIndex(ID_INDICATOR_OFFLINELED), hIcon);
	m_wndStatusBar.GetStatusBarCtrl().Invalidate();
	m_wndStatusBar.GetStatusBarCtrl().UpdateWindow();
	DestroyIcon(hIcon);
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateApply									*/	
/* Description   : Update FTP server parameters.					*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnApply() 
{
	m_ConfigurationPage.UpdateData();
	theServer.SetPort(m_ConfigurationPage.m_nPort);
	theServer.SetMaxUsers(m_ConfigurationPage.m_nMaxUsers);
	theServer.SetTimeout(m_ConfigurationPage.m_nTimeout);
	theServer.SetWelcomeMessage(m_ConfigurationPage.m_strWelcomeMessage);
	theServer.SetGoodbyeMessage(m_ConfigurationPage.m_strGoodbyeMessage);

	SetWindowText(m_ConfigurationPage.m_strApplicationName);

	// set log level
	theApp.m_LogFile.SetLogLevel(m_ConfigurationPage.m_nLogLevel);

	OnFTPStatusChange(0, "FTP Server configuration updated.");
}


void CApplicationDlg::OnFTPStatisticChange(int nType, int nValue)
{
	m_StatisticsPage.SetValue(nType, nValue);
}


/********************************************************************/
/*																	*/
/* Function name : FormatSize										*/
/* Description   : Format size, the way explorer diplays it			*/
/*																	*/
/********************************************************************/
char* CApplicationDlg::FormatSize(DWORD dwSizeLow, DWORD dwSizeHigh)
{
	static char szBuff[100];

	unsigned __int64 nFileSize = ((unsigned __int64)(((DWORD)(dwSizeLow)) | 
								 ((unsigned __int64)((DWORD)(dwSizeHigh))) << 32));
	unsigned __int64 kb = 0;

	if (nFileSize > 1024)
	{
		kb = nFileSize / 1024;
		if (nFileSize % 1024)
			kb++;
	}

	// make it a string
	_ui64tot(kb, szBuff, 10);

	// add thousand seperators
	int nLength = lstrlen(szBuff);
	if (nLength > 3)
	{
		LPCTSTR ptr = szBuff;
		ptr += (nLength-1);

		char szTemp[100];

		LPTSTR ptrTemp = szTemp;
		for(int i=0; i<nLength; i++)
		{
			if (i && ((i % 3) == 0)) 
			{
				if (*ptrTemp != ',')
				{
					*ptrTemp = ',';
					ptrTemp = _tcsinc(ptrTemp);
				}
			}
			*ptrTemp = *ptr;
			ptrTemp = _tcsinc(ptrTemp);
			ptr = _tcsdec(szBuff, ptr);
		}
		// terminate string
		*ptrTemp = '\0';
		// reverse string
		_tcsrev(szTemp);
		lstrcpy(szBuff, szTemp);
	}
	// add 'KB' to it
	lstrcat(szBuff, " KB");
	return szBuff;
}


/********************************************************************/
/*																	*/
/* Function name : OnAccountWizard									*/
/* Description   : Launch New Account Wizard						*/
/*																	*/
/********************************************************************/
void CApplicationDlg::OnAccountWizard() 
{
	CBitmap bmpHeader, bmpWatermark;

	VERIFY(bmpHeader.LoadBitmap(IDB_BANNER));
	VERIFY(bmpWatermark.LoadBitmap(IDB_WATERMARK));
	
	// show windows 2000-like wizard
	CWizardSheet wizSheet("New Account Wizard", this, 0, bmpWatermark, NULL, bmpHeader);
	wizSheet.m_psh.hInstance = ::GetModuleHandle(NULL);
	if (wizSheet.DoModal() == ID_WIZFINISH)
	{
		CArray<CUser, CUser&> m_UserArray;

		theServer.m_UserManager.GetUserList(m_UserArray);
		
		int nIndex = -1;
		for (int i=0; i<m_UserArray.GetSize(); i++)
		{
			// if it already exists -> update it
			if (m_UserArray[i].m_strName.CompareNoCase(wizSheet.m_Page1.m_strAccountName) == 0)
			{
				nIndex = i;
				break;
			}
		}

		// add new account ?
		if (nIndex == -1)
		{
			CUser user;
			nIndex = m_UserArray.Add(user);		
		}
		m_UserArray[nIndex].m_bAccountDisabled = FALSE;
		m_UserArray[nIndex].m_strName = wizSheet.m_Page1.m_strAccountName;
		m_UserArray[nIndex].m_strPassword = wizSheet.m_Page2.m_strPassword;
		
		// add home directory
		CDirectory directory;
		
		directory.m_strDir = wizSheet.m_Page3.m_strHomeDirectory;
		directory.m_strAlias = "";
		directory.m_bIsHomeDir = TRUE;
		directory.m_bAllowCreateDirectory = wizSheet.m_Page4.m_bAllowCreateDirectory;
		directory.m_bAllowDelete = wizSheet.m_Page4.m_bAllowDelete;
		directory.m_bAllowDownload = wizSheet.m_Page4.m_bAllowDownload;
		directory.m_bAllowRename = wizSheet.m_Page4.m_bAllowRename;
		directory.m_bAllowUpload = wizSheet.m_Page4.m_bAllowUpload;
		directory.m_strAlias = "";

		m_UserArray[nIndex].m_DirectoryArray.Add(directory);
				
		// update 
		theServer.m_UserManager.UpdateUserList(m_UserArray);
	}	
}


