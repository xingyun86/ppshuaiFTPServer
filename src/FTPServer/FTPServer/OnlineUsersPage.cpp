
/****************************************************************/
/*																*/
/*  OnlineUsersPage.cpp											*/
/*																*/
/*  Implementation of the COnlineUsersPage class.				*/
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
#include "OnlineUsersPage.h"
#include "UserAccountsDlg.h"
#include "ApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFTPServer theServer;

IMPLEMENT_DYNCREATE(COnlineUsersPage, CDialog)

COnlineUsersPage::COnlineUsersPage() : CDialog(COnlineUsersPage::IDD)
{
	//{{AFX_DATA_INIT(COnlineUsersPage)
	//}}AFX_DATA_INIT
}

COnlineUsersPage::~COnlineUsersPage()
{
}

void COnlineUsersPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnlineUsersPage)
	DDX_Control(pDX, IDC_ONLINE_USERS, m_OnlineUsers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COnlineUsersPage, CDialog)
	//{{AFX_MSG_MAP(COnlineUsersPage)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_KICK_USER, OnKickUser)
	ON_COMMAND(IDC_EDIT_USER, OnEditUserAccount)
	ON_COMMAND(IDC_BLOCK_IP, OnBlockIp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_INITDIALOG message.							*/
/*																	*/
/********************************************************************/
BOOL COnlineUsersPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_OnlineUsers.InsertColumn(0, "ThreadID");		
	m_OnlineUsers.InsertColumn(1, "Username");	
	m_OnlineUsers.InsertColumn(2, "IP Adress");	
	m_OnlineUsers.InsertColumn(3, "Login Time");
	
	DWORD dwStyle = m_OnlineUsers.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
    m_OnlineUsers.SetExtendedStyle(dwStyle);
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_ONLINE_USERS)))
	{
		CRect rect;
		GetClientRect(rect);
		m_OnlineUsers.MoveWindow(rect);
		m_OnlineUsers.SetColumnWidth(0, 0);
		m_OnlineUsers.SetColumnWidth(1, rect.Width()/3-2);
		m_OnlineUsers.SetColumnWidth(2, rect.Width()/3-2);
		m_OnlineUsers.SetColumnWidth(3, rect.Width()/3-2);
	}	
}


/********************************************************************/
/*																	*/
/* Function name : AddUser											*/
/* Description   : Add new connected user to list					*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::AddUser(DWORD nThreadID, LPCTSTR lpszName, LPCTSTR lpszAddress)
{
	CString strThreadID;
	strThreadID.Format("%d", nThreadID);

	LVFINDINFO info;
	
	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = (LPCTSTR)strThreadID;

	int nIndex = m_OnlineUsers.FindItem(&info);
	if (nIndex == -1)
	{
		nIndex = m_OnlineUsers.InsertItem(0, strThreadID);
	}

	m_OnlineUsers.SetItemText(nIndex, 1, lpszName);
	m_OnlineUsers.SetItemText(nIndex, 2, lpszAddress);
	m_OnlineUsers.SetItemText(nIndex, 3, CTime::GetCurrentTime().Format("%H:%M:%S"));
	
}


/********************************************************************/
/*																	*/
/* Function name : RemoveUser										*/
/* Description   : Remove disconnected user from list				*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::RemoveUser(DWORD nThreadID)
{
	LVFINDINFO info;
	
	CString strThreadID;
	strThreadID.Format("%d", nThreadID);

	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = (LPCTSTR)strThreadID;

	int nIndex = m_OnlineUsers.FindItem(&info);
	if (nIndex != -1)
	{
		m_OnlineUsers.DeleteItem(nIndex);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnContextMenu									*/
/* Description   : Show context menu								*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// get selected user
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nIndex == -1)
        return;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_ONLINE_MENU));
	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);			

}


/********************************************************************/
/*																	*/
/* Function name : OnKickUser										*/
/* Description   : Close connection for this user(s)				*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::OnKickUser() 
{
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	while (nIndex != -1)
	{
		CString strThreadID = m_OnlineUsers.GetItemText(nIndex, 0);
		PostThreadMessage(atoi(strThreadID), WM_QUIT, 0 ,0);
		nIndex = m_OnlineUsers.GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnEditUserAccount								*/
/* Description   : Change user rights of selected user				*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::OnEditUserAccount() 
{
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if (nIndex != -1)	
	{
		CUserAccountsDlg dlg;

		theServer.m_UserManager.GetUserList(dlg.m_UserArray);
		dlg.m_strUserName = m_OnlineUsers.GetItemText(nIndex, 1);
		if (dlg.DoModal() == IDOK)
		{
			theServer.m_UserManager.UpdateUserList(dlg.m_UserArray);
		}	
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnBlockIp										*/
/* Description   : Change user rights of selected user				*/
/*																	*/
/********************************************************************/
void COnlineUsersPage::OnBlockIp() 
{
	int nIndex = m_OnlineUsers.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if (nIndex == -1)
		return;

	while (nIndex != -1)
	{
		CString strIP = m_OnlineUsers.GetItemText(nIndex, 2);
		((CApplicationDlg *)GetParent())->m_SecurityPage.AddIPToBlockList(strIP);
		nIndex = m_OnlineUsers.GetNextItem(nIndex, LVNI_ALL | LVNI_SELECTED);
	}
}
