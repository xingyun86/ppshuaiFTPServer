
/****************************************************************/
/*																*/
/*  SecurityPage.cpp											*/
/*																*/
/*  Implementation of the CSecurityPage class.					*/
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
#include "ftpserver.h"
#include "SecurityPage.h"
#include "AddIPDlg.h"

extern CFTPServer theServer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSecurityPage::CSecurityPage(CWnd* pParent /*=NULL*/)
	: CDialogResize(CSecurityPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecurityPage)
	m_bBlockAll = FALSE;
	//}}AFX_DATA_INIT
}


void CSecurityPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecurityPage)
	DDX_Control(pDX, IDC_BLOCKEDLIST, m_BlockedList);
	DDX_Control(pDX, IDC_NONBLOCKEDLIST, m_NonBlockedList);
	DDX_Check(pDX, IDC_BLOCK_ALL, m_bBlockAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecurityPage, CDialogResize)
	//{{AFX_MSG_MAP(CSecurityPage)
	ON_BN_CLICKED(IDC_BLOCK_ALL, OnBlockAll)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ADD_BLOCK, OnAddBlock)
	ON_BN_CLICKED(IDC_EDIT_BLOCK, OnEditBlock)
	ON_BN_CLICKED(IDC_ADD_NONBLOCK, OnAddNonblock)
	ON_BN_CLICKED(IDC_EDIT_NONBLOCK, OnEditNonblock)
	ON_BN_CLICKED(IDC_REMOVE_BLOCK, OnRemoveBlock)
	ON_BN_CLICKED(IDC_REMOVE_NONBLOCK, OnRemoveNonblock)
	ON_LBN_DBLCLK(IDC_BLOCKEDLIST, OnDblclkBlockedlist)
	ON_LBN_DBLCLK(IDC_NONBLOCKEDLIST, OnDblclkNonblockedlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CSecurityPage)
	DLGRESIZE_CONTROL(IDC_BLOCKEDLIST, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_NONBLOCKEDLIST, DLSZ_SIZE_X)
END_DLGRESIZE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/
/* Description   : Initialize dialog								*/
/*																	*/
/********************************************************************/
BOOL CSecurityPage::OnInitDialog() 
{
	CDialogResize::OnInitDialog();
	
	InitResizing(FALSE, FALSE, WS_CLIPCHILDREN);

	m_bBlockAll = AfxGetApp()->GetProfileInt("Settings", "BlockAll", 0);
	GetDlgItem(IDC_NONBLOCKEDLIST)->EnableWindow(m_bBlockAll);
	GetDlgItem(IDC_ADD_NONBLOCK)->EnableWindow(m_bBlockAll);
	GetDlgItem(IDC_EDIT_NONBLOCK)->EnableWindow(m_bBlockAll);
	GetDlgItem(IDC_REMOVE_NONBLOCK)->EnableWindow(m_bBlockAll);

	GetDlgItem(IDC_BLOCKEDLIST)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_ADD_BLOCK)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_EDIT_BLOCK)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_REMOVE_BLOCK)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_STATIC1)->EnableWindow(!m_bBlockAll);

	UpdateData(FALSE);

	CStringArray strArray;
	theServer.m_SecurityManager.GetBlockedList(strArray);
	for (int i=0; i < strArray.GetSize(); i++)
	{
		m_BlockedList.AddString(strArray[i]);
	}

	theServer.m_SecurityManager.GetNonBlockedList(strArray);
	for (int j=0; j < strArray.GetSize(); j++)
	{
		m_NonBlockedList.AddString(strArray[j]);
	}

	// get list of all ip addresses in use by this system (only show first two...)
	char szHostName[128];
	HOSTENT *lpHost=NULL;
    struct sockaddr_in sock;

	gethostname(szHostName, sizeof(szHostName));
    lpHost = gethostbyname(szHostName);
	if (lpHost != NULL)
    {
		for(int i=0; lpHost->h_addr_list[i] != NULL ;i++)
        {
			memcpy(&(sock.sin_addr), lpHost->h_addr_list[i], lpHost->h_length);
			if (i == 0)
			{
				SetDlgItemText(IDC_IPADDRESS1, inet_ntoa(sock.sin_addr));
			}
			else
			if (i == 1)
			{
				SetDlgItemText(IDC_IPADDRESS2, inet_ntoa(sock.sin_addr));
			}
		}
	}
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnDestroy										*/
/* Description   : Dialog is about to be destroyed.					*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnDestroy() 
{
	UpdateData();

	AfxGetApp()->WriteProfileInt("Settings", "BlockAll", m_bBlockAll);
	CDialogResize::OnDestroy();
}


/********************************************************************/
/*																	*/
/* Function name : OnBlockAll										*/
/* Description   : Block all except... has been clicked.			*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnBlockAll() 
{
	UpdateData();
	
	GetDlgItem(IDC_NONBLOCKEDLIST)->EnableWindow(m_bBlockAll);
	GetDlgItem(IDC_ADD_NONBLOCK)->EnableWindow(m_bBlockAll);
	GetDlgItem(IDC_EDIT_NONBLOCK)->EnableWindow(m_bBlockAll);
	GetDlgItem(IDC_REMOVE_NONBLOCK)->EnableWindow(m_bBlockAll);

	GetDlgItem(IDC_BLOCKEDLIST)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_ADD_BLOCK)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_EDIT_BLOCK)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_REMOVE_BLOCK)->EnableWindow(!m_bBlockAll);
	GetDlgItem(IDC_STATIC1)->EnableWindow(!m_bBlockAll);

	theServer.SetSecurityMode(!m_bBlockAll);
}


/********************************************************************/
/*																	*/
/* Function name : OnAddBlock										*/
/* Description   : Add IP address to blocked list.					*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnAddBlock() 
{
	CAddIPDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		for (int i=0; i < m_BlockedList.GetCount(); i++)
		{
			CString strText;
			m_BlockedList.GetText(i, strText);
			if (strText.CompareNoCase(dlg.m_strIPaddress) == 0)
			{
				// already exists !
				return;
			}
		}
		int nIndex = m_BlockedList.AddString(dlg.m_strIPaddress);
		m_BlockedList.SetCurSel(nIndex);
		
		UpdateSecurityData(0);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnEditBlock										*/
/* Description   : Edit IP address from blocked list.				*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnEditBlock() 
{
	int nIndex = m_BlockedList.GetCurSel();
	if (nIndex == -1)
		return; 

	CAddIPDlg dlg;

	dlg.m_strTitle = "Edit IP address";
	m_BlockedList.GetText(nIndex, dlg.m_strIPaddress);

	if (dlg.DoModal() == IDOK)
	{
		for (int i=0; i < m_BlockedList.GetCount(); i++)
		{
			CString strText;
			m_BlockedList.GetText(i, strText);
			if (strText.CompareNoCase(dlg.m_strIPaddress) == 0)
			{
				// already exists !
				return;
			}
		}
		m_BlockedList.DeleteString(nIndex);
		nIndex = m_BlockedList.AddString(dlg.m_strIPaddress);
		m_BlockedList.SetCurSel(nIndex);
		
		UpdateSecurityData(0);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnRemoveBlock									*/
/* Description   : Remove IP address from blocked list.				*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnRemoveBlock() 
{
	int nIndex = m_BlockedList.GetCurSel();
	if (nIndex == -1)
		return; 
	m_BlockedList.DeleteString(nIndex);
	m_BlockedList.SetCurSel(0);
	UpdateSecurityData(0);
}


/********************************************************************/
/*																	*/
/* Function name : OnAddNonblock									*/
/* Description   : Add IP address to non-blocked list.				*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnAddNonblock() 
{
	CAddIPDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		for (int i=0; i < m_NonBlockedList.GetCount(); i++)
		{
			CString strText;
			m_NonBlockedList.GetText(i, strText);
			if (strText.CompareNoCase(dlg.m_strIPaddress) == 0)
			{
				// already exists !
				return;
			}
		}
		int nIndex = m_NonBlockedList.AddString(dlg.m_strIPaddress);
		m_NonBlockedList.SetCurSel(nIndex);
		UpdateSecurityData(1);
	}

}


/********************************************************************/
/*																	*/
/* Function name : OnEditNonblock									*/
/* Description   : Edit IP address from non-blocked list.			*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnEditNonblock() 
{
	int nIndex = m_NonBlockedList.GetCurSel();
	if (nIndex == -1)
		return; 

	CAddIPDlg dlg;
	dlg.m_strTitle = "Edit IP address";

	m_NonBlockedList.GetText(nIndex, dlg.m_strIPaddress);

	if (dlg.DoModal() == IDOK)
	{
		for (int i=0; i < m_NonBlockedList.GetCount(); i++)
		{
			CString strText;
			m_NonBlockedList.GetText(i, strText);
			if (strText.CompareNoCase(dlg.m_strIPaddress) == 0)
			{
				// already exists !
				return;
			}
		}
		m_NonBlockedList.DeleteString(nIndex);
		nIndex = m_NonBlockedList.AddString(dlg.m_strIPaddress);
		m_NonBlockedList.SetCurSel(nIndex);
		UpdateSecurityData(1);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnRemoveNonblock									*/
/* Description   : Remove IP address from non-blocked list.			*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnRemoveNonblock() 
{
	int nIndex = m_NonBlockedList.GetCurSel();
	if (nIndex == -1)
		return; 
	m_NonBlockedList.DeleteString(nIndex);
	m_NonBlockedList.SetCurSel(0);	
	UpdateSecurityData(1);
}


/********************************************************************/
/*																	*/
/* Function name : UpdateSecurityData								*/
/* Description   : Update security data.							*/
/*																	*/
/********************************************************************/
void CSecurityPage::UpdateSecurityData(int nType)
{
	CStringArray strArray;

	if (nType == 0)
	{
		for (int i=0; i < m_BlockedList.GetCount(); i++)
		{
			CString strText;
			m_BlockedList.GetText(i, strText);
			strArray.Add(strText);
		}
		theServer.m_SecurityManager.UpdateBlockedList(strArray);
	}
	else
	{
		for (int i=0; i < m_NonBlockedList.GetCount(); i++)
		{
			CString strText;
			m_NonBlockedList.GetText(i, strText);
			strArray.Add(strText);
		}
		theServer.m_SecurityManager.UpdateNonBlockedList(strArray);
	}
}


/********************************************************************/
/*																	*/
/* Function name : AddIPToBlockList									*/
/* Description   : Add IP address to blocked list.					*/
/*																	*/
/********************************************************************/
void CSecurityPage::AddIPToBlockList(LPCTSTR lpszIP)
{
	for (int i=0; i < m_BlockedList.GetCount(); i++)
	{
		CString strText;
		m_BlockedList.GetText(i, strText);
		if (strText.CompareNoCase(lpszIP) == 0)
		{
			// already exists !
			return;
		}
	}
	int nIndex = m_BlockedList.AddString(lpszIP);
	m_BlockedList.SetCurSel(nIndex);
	
	UpdateSecurityData(0);
}


/********************************************************************/
/*																	*/
/* Function name : OnDblclkBlockedlist								*/
/* Description   : Edit IP address from blocked list.				*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnDblclkBlockedlist() 
{
	OnEditBlock();
}


/********************************************************************/
/*																	*/
/* Function name : OnDblclkNonblockedlist							*/
/* Description   : Edit IP address from non-blocked list.			*/
/*																	*/
/********************************************************************/
void CSecurityPage::OnDblclkNonblockedlist() 
{
	OnEditNonblock();
}
