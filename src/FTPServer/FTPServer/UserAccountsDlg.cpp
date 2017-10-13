
/****************************************************************/
/*																*/
/*  UserAccountsDlg.cpp											*/
/*																*/
/*  Implementation of the CUserAccountsDlg class.				*/
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
#include "UserAccountsDlg.h"
#include "AddUserDlg.h"
#include "DirectoryDlg.h"
#include "WizardPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFTPServerApp theApp;

CUserAccountsDlg::CUserAccountsDlg(CWnd* pParent /*=NULL*/)
	: CDialogResize(CUserAccountsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserAccountsDlg)
	m_strPassword = _T("");
	m_bDisableAccount = FALSE;
	//}}AFX_DATA_INIT
	m_nPreviousIndex = LB_ERR;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_FTPSERVER);
}

CUserAccountsDlg::~CUserAccountsDlg()
{
	
}


void CUserAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserAccountsDlg)
	DDX_Control(pDX, IDC_USERS, m_UsersList);
	DDX_Control(pDX, IDC_DIR_LIST, m_DirectoryList);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_DISABLE_ACCOUNT, m_bDisableAccount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserAccountsDlg, CDialogResize)
	//{{AFX_MSG_MAP(CUserAccountsDlg)
	ON_LBN_SELCHANGE(IDC_USER_LIST, OnSelchangeUserlist)
	ON_NOTIFY(NM_DBLCLK, IDC_DIR_LIST, OnDblclkDirlist)
	ON_BN_CLICKED(IDC_ADD_DIR, OnAddDir)
	ON_BN_CLICKED(IDC_EDIT_DIR, OnEditDir)
	ON_BN_CLICKED(IDC_DEL_DIR, OnDelDir)
	ON_BN_CLICKED(IDC_SET_HOME, OnSetHome)
	ON_BN_CLICKED(IDC_ADD_USER, OnAddUser)
	ON_BN_CLICKED(IDC_EDIT_USER, OnEditUser)
	ON_BN_CLICKED(IDC_DEL_USER, OnDelUser)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_USER, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_ACCOUNT, OnUpdateDisableAccount)
	ON_NOTIFY(NM_CLICK, IDC_USERS, OnClickUsers)
	ON_NOTIFY(LVN_KEYDOWN, IDC_USERS, OnKeydownUsers)
	ON_BN_CLICKED(IDC_DISABLE_ACCOUNT, OnDisableAccount)
	ON_UPDATE_COMMAND_UI(IDC_DEL_USER, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PASSWORD, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_DIR_LIST, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_ADD_DIR, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_DIR, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_DEL_DIR, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_SET_HOME, OnUpdateControls)
	ON_BN_CLICKED(IDC_WIZARD, OnWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CUserAccountsDlg)
	DLGRESIZE_CONTROL(IDC_ADD_DIR, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_EDIT_DIR, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_DEL_DIR, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_SET_HOME, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_DIR_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_STATIC3, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_STATIC2, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_USERS, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_ADD_USER, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_EDIT_USER, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_DEL_USER, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_WIZARD, DLSZ_MOVE_Y)
END_DLGRESIZE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/
/* Description   : Initialize dialog								*/
/*																	*/
/********************************************************************/
BOOL CUserAccountsDlg::OnInitDialog() 
{
	CDialogResize::OnInitDialog();
	
	InitResizing();
	
	// Set the icon for this dialog.
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rc;
	m_UsersList.GetClientRect(rc);
	m_UsersList.InsertColumn(0, "Users", LVCFMT_LEFT, rc.Width()-5);

	// fill user list
	for (int i=0; i < m_UserArray.GetSize(); i++)
	{
		int nIndex = m_UsersList.InsertItem(0, m_UserArray[i].m_strName, 2);
		m_UsersList.SetItemData(nIndex, i);
	}
	// setup directory list
	m_DirectoryList.InsertColumn(0, "Physical Path", LVCFMT_LEFT, 100);
	m_DirectoryList.InsertColumn(1, "Virtual Path", LVCFMT_LEFT, 100);
	m_DirectoryList.InsertColumn(2, "Download", LVCFMT_LEFT, 10);
	m_DirectoryList.InsertColumn(3, "Upload", LVCFMT_LEFT, 10);
	m_DirectoryList.InsertColumn(4, "Rename", LVCFMT_LEFT, 10);
	m_DirectoryList.InsertColumn(5, "Delete", LVCFMT_LEFT, 10);
	m_DirectoryList.InsertColumn(6, "Create Dir", LVCFMT_LEFT, 10);

	// create and attach imagelist
	m_ImageList.Create(16, 16, ILC_MASK, 3, 3);
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_NONE);
	m_ImageList.Add(hIcon);
	DestroyIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_HOME);
	m_ImageList.Add(hIcon);
	DestroyIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_USER);
	m_ImageList.Add(hIcon);
	DestroyIcon(hIcon);

	m_DirectoryList.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_UsersList.SetImageList(&m_ImageList, LVSIL_SMALL);

	// select preferred user ?
	if (!m_strUserName.IsEmpty())
	{
		LVFINDINFO info;
	
		info.flags = LVFI_STRING;
		info.psz = (LPCTSTR)m_strUserName;

		int nIndex = m_UsersList.FindItem(&info);
		m_UsersList.SetItemState(nIndex ,LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
	}
	else
	{
		m_UsersList.SetItemState(0 ,LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
	}

	GetDlgItem(IDC_PASSWORD)->ModifyStyle(NULL, ES_PASSWORD);
	((CEdit *)GetDlgItem(IDC_PASSWORD))->SetPasswordChar('*');

	OnSelchangeUserlist();
	
	UpdateDialogControls(this, FALSE);
	AutoSizeColumns();	
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnClickUsers										*/
/* Description   : Selection in the user listcontrol has changed.	*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnClickUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnSelchangeUserlist();
	*pResult = 0;
}


/********************************************************************/
/*																	*/
/* Function name : OnKeydownUsers									*/
/* Description   : Selection in the user listcontrol has changed.	*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnKeydownUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

    // get index of selected item
	int nIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;

	if (pLVKeyDow->wVKey == VK_DOWN)
	{
		if (m_UsersList.GetItemCount()-1 > nIndex)
		{
			m_UsersList.SetItemState(nIndex + 1, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
		}
	}
	else
	if (pLVKeyDow->wVKey == VK_UP)
	{
		if (nIndex > 0)
		{
			m_UsersList.SetItemState(nIndex - 1, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
		}
	}
	else
	if (pLVKeyDow->wVKey == VK_NEXT)
	{
		m_UsersList.SetItemState(m_UsersList.GetItemCount()-1, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
	}
	else
	if (pLVKeyDow->wVKey == VK_PRIOR)
	{
		m_UsersList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
	}

	OnSelchangeUserlist();
	*pResult = 1;
}


/********************************************************************/
/*																	*/
/* Function name : OnSelchangeUserlist								*/
/* Description   : Selection in the user listcontrol has changed.	*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnSelchangeUserlist() 
{
	UpdateData();
	// save data of previous selected user
	if (m_nPreviousIndex != -1)
	{
		int nOldindex = m_UsersList.GetItemData(m_nPreviousIndex);
		m_UserArray[nOldindex].m_strPassword = m_strPassword;
		m_UserArray[nOldindex].m_bAccountDisabled = m_bDisableAccount;
	}

	// get selected user
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if (nSelIndex != -1)
	{
		int nUserIndex = m_UsersList.GetItemData(nSelIndex);

		// update dialog variables
		m_strPassword = m_UserArray[nUserIndex].m_strPassword;
		m_bDisableAccount = m_UserArray[nUserIndex].m_bAccountDisabled;

		m_DirectoryList.DeleteAllItems();
		
		// fill the directory list
		for (int i=0; i < m_UserArray[nUserIndex].m_DirectoryArray.GetSize(); i++)
		{
			int nItem = m_DirectoryList.InsertItem(i, m_UserArray[nUserIndex].m_DirectoryArray[i].m_strDir);

			m_DirectoryList.SetItemText(nItem, 1, m_UserArray[nUserIndex].m_DirectoryArray[i].m_strAlias);
			m_DirectoryList.SetItemText(nItem, 2, m_UserArray[nUserIndex].m_DirectoryArray[i].m_bAllowDownload ? "Y" : "N");
			m_DirectoryList.SetItemText(nItem, 3, m_UserArray[nUserIndex].m_DirectoryArray[i].m_bAllowUpload ? "Y" : "N");
			m_DirectoryList.SetItemText(nItem, 4, m_UserArray[nUserIndex].m_DirectoryArray[i].m_bAllowRename ? "Y" : "N");
			m_DirectoryList.SetItemText(nItem, 5, m_UserArray[nUserIndex].m_DirectoryArray[i].m_bAllowDelete ? "Y" : "N");
			m_DirectoryList.SetItemText(nItem, 6, m_UserArray[nUserIndex].m_DirectoryArray[i].m_bAllowCreateDirectory ? "Y" : "N");
			LVITEM lvi;
			ZeroMemory(&lvi, sizeof (LV_ITEM));
			lvi.mask = LVIF_IMAGE|LVIF_PARAM;
			lvi.iItem = nItem;
			m_DirectoryList.GetItem(&lvi);
			lvi.lParam = i;
			lvi.iImage = m_UserArray[nUserIndex].m_DirectoryArray[i].m_bIsHomeDir ? 1 : 0;
			m_DirectoryList.SetItem(&lvi);
		}
		m_nPreviousIndex = nSelIndex;
	}
	else
	{
		// nothing selected...
		m_DirectoryList.DeleteAllItems();
		m_strPassword = "";
		m_bDisableAccount = FALSE;
	}
	UpdateData(FALSE);

	UpdateDialogControls(this, FALSE);
	
	AutoSizeColumns();
}


/********************************************************************/
/*																	*/
/* Function name : OnOK												*/
/* Description   : Called when the user clicks the OK button.		*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnOK() 
{
	m_UsersList.SetItemState(-1 ,LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
	// force update
	OnSelchangeUserlist();
	
	CDialogResize::OnOK();
}


/********************************************************************/
/*																	*/
/* Function name : OnDblclkDirlist									*/
/* Description   : Double clicked a directy item.					*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnDblclkDirlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditDir();
	*pResult = 0;
}


/********************************************************************/
/*																	*/
/* Function name : OnAddUser										*/
/* Description   : Add a new user account.							*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnAddUser() 
{
	CAddUserDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		for (int i=0; i<m_UsersList.GetItemCount(); i++)
		{
			CString strName;
			strName = m_UsersList.GetItemText(i, 0);
			if (strName.CompareNoCase(dlg.m_strName) == 0)
			{
				AfxMessageBox("Sorry, this user already exists!");
				return;
			}
		}

		CUser user;
		user.m_strName = dlg.m_strName;
		user.m_strPassword = "";

		int nItem = m_UsersList.InsertItem(0, user.m_strName, 2);
		if (nItem <= m_nPreviousIndex)
			m_nPreviousIndex++;

		int index = m_UserArray.Add(user);
		
		m_UsersList.SetItemData(nItem, index);
		m_UsersList.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 

		OnSelchangeUserlist();
	} 
}


/********************************************************************/
/*																	*/
/* Function name : OnEditUser										*/
/* Description   : Edit user account name.							*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnEditUser() 
{
	// get selected user
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

	int nUserIndex = m_UsersList.GetItemData(nSelIndex);

	CAddUserDlg dlg;
	dlg.m_strTitle = "Edit User";
	dlg.m_strName = m_UserArray[nUserIndex].m_strName;

	if (dlg.DoModal() == IDOK)
	{
		// check if user already exists
		for (int i=0; i<m_UsersList.GetItemCount(); i++)
		{
			if (i != nSelIndex)
			{
				CString strName;
				strName = m_UsersList.GetItemText(i, 0);
				if (strName.CompareNoCase(dlg.m_strName) == 0)
				{
					AfxMessageBox("Sorry, this user already exists!");
					return;
				}
			}
		}

		m_UserArray[nUserIndex].m_strName = dlg.m_strName;

		m_UsersList.DeleteItem(nSelIndex);
		nSelIndex = m_UsersList.InsertItem(0, dlg.m_strName, 2);

		m_UsersList.SetItemData(nSelIndex, nUserIndex);
		m_UsersList.SetItemState(nSelIndex, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
		m_nPreviousIndex = nSelIndex;

		OnSelchangeUserlist();
	} 
}


/********************************************************************/
/*																	*/
/* Function name : OnDelUser										*/
/* Description   : Delete user account.								*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnDelUser() 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

	int nUserIndex = m_UsersList.GetItemData(nSelIndex);
	// remove user from listcontrol
	m_UsersList.DeleteItem(nSelIndex);
	// remove user from array
	m_UserArray.RemoveAt(nUserIndex);

	m_nPreviousIndex = -1;
	// update item data values
	for (int i=0; i<m_UsersList.GetItemCount(); i++)
	{
		int nItemData = m_UsersList.GetItemData(i);
		if (nItemData > nSelIndex)
			m_UsersList.SetItemData(i, nItemData-1);
	}
	OnSelchangeUserlist();	
}



/********************************************************************/
/*																	*/
/* Function name : OnAddDir											*/
/* Description   : Add directory entry to user account.				*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnAddDir() 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

	CDirectoryDlg dlg;

	dlg.m_bIsHomeDir = m_DirectoryList.GetItemCount() ? FALSE : TRUE;

	if (dlg.DoModal() == IDOK)
	{
		int nUserIndex = m_UsersList.GetItemData(nSelIndex);
		
		CDirectory dir;
		dir.m_bAllowCreateDirectory = dlg.m_bAllowCreateDirectory;
		dir.m_bAllowDelete = dlg.m_bAllowDelete;
		dir.m_bAllowDownload = dlg.m_bAllowDownload;
		dir.m_bAllowRename = dlg.m_bAllowRename;
		dir.m_bAllowUpload = dlg.m_bAllowUpload;
		dir.m_bIsHomeDir = FALSE;
		dir.m_strDir = dlg.m_strPath;
		dir.m_strAlias = dlg.m_strAlias;

		dir.m_bIsHomeDir = m_DirectoryList.GetItemCount() ? FALSE : TRUE;

		int nIndex = m_UserArray[nUserIndex].m_DirectoryArray.Add(dir);

		int nItem = m_DirectoryList.InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, 0, dlg.m_strPath, 0, 0, dir.m_bIsHomeDir?1:0, nIndex);
		m_DirectoryList.SetItemText(nItem, 1, dir.m_strAlias);
		m_DirectoryList.SetItemText(nItem, 2, dir.m_bAllowDownload ? "Y" : "N");
		m_DirectoryList.SetItemText(nItem, 3, dir.m_bAllowUpload ? "Y" : "N");
		m_DirectoryList.SetItemText(nItem, 4, dir.m_bAllowRename ? "Y" : "N");
		m_DirectoryList.SetItemText(nItem, 5, dir.m_bAllowDelete ? "Y" : "N");
		m_DirectoryList.SetItemText(nItem, 6, dir.m_bAllowCreateDirectory ? "Y" : "N");

		m_DirectoryList.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnEditDir										*/
/* Description   : Edit directory entry properties.					*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnEditDir() 
{
	// get selected user
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

	int nUserIndex = m_UsersList.GetItemData(nSelIndex);

    // get index of selected directory item
	nSelIndex = m_DirectoryList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nSelIndex == -1)
        return;

	int nDirIndex = m_DirectoryList.GetItemData(nSelIndex);
	
	CDirectoryDlg dlg;

	dlg.m_strTitle = "Edit Directory";

	dlg.m_bAllowCreateDirectory = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowCreateDirectory;
	dlg.m_bAllowDelete = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowDelete;
	dlg.m_bAllowDownload = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowDownload;
	dlg.m_bAllowRename = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowRename;
	dlg.m_bAllowUpload = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowUpload;
	dlg.m_strPath = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_strDir;
	dlg.m_strAlias = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_strAlias;
	dlg.m_bIsHomeDir = m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bIsHomeDir;

	if (!dlg.m_strAlias.IsEmpty())
		dlg.m_bVirtualDir = TRUE;

	if (dlg.DoModal() == IDOK)
	{
		m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowCreateDirectory = dlg.m_bAllowCreateDirectory;
		m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowDelete = dlg.m_bAllowDelete;
		m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowDownload = dlg.m_bAllowDownload;
		m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowRename = dlg.m_bAllowRename;
		m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_bAllowUpload = dlg.m_bAllowUpload;
		m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_strDir = dlg.m_strPath;
		if (dlg.m_bVirtualDir)
			m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_strAlias = dlg.m_strAlias;
		else
			m_UserArray[nUserIndex].m_DirectoryArray[nDirIndex].m_strAlias = "";

		m_DirectoryList.SetItemText(nDirIndex, 0, dlg.m_strPath);
		m_DirectoryList.SetItemText(nDirIndex, 1, dlg.m_strAlias);
		m_DirectoryList.SetItemText(nDirIndex, 2, dlg.m_bAllowDownload ? "Y" : "N");
		m_DirectoryList.SetItemText(nDirIndex, 3, dlg.m_bAllowUpload ? "Y" : "N");
		m_DirectoryList.SetItemText(nDirIndex, 4, dlg.m_bAllowRename ? "Y" : "N");
		m_DirectoryList.SetItemText(nDirIndex, 5, dlg.m_bAllowDelete ? "Y" : "N");
		m_DirectoryList.SetItemText(nDirIndex, 6, dlg.m_bAllowCreateDirectory ? "Y" : "N");

		OnSelchangeUserlist();
	}
}


/********************************************************************/
/*																	*/
/* Function name : OnDelDir											*/
/* Description   : Delete directory entry from user account.		*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnDelDir() 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

    // get index of selected user item
	int nUserIndex = m_UsersList.GetItemData(nSelIndex);
	
    // get index of selected directory item
	nSelIndex = m_DirectoryList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nSelIndex == -1)
        return;

	int nDirIndex = m_DirectoryList.GetItemData(nSelIndex);
	
	// delete item from list
	m_DirectoryList.DeleteItem(nSelIndex);

	m_UserArray[nUserIndex].m_DirectoryArray.RemoveAt(nDirIndex);

	for (int i=0; i < m_DirectoryList.GetItemCount(); i++)
	{
		int nItemData = m_DirectoryList.GetItemData(i);
		if (nItemData > nDirIndex)
		{
			m_DirectoryList.SetItemData(i, nItemData-1);
		}
	}
	UpdateDialogControls(this, FALSE);
}


/********************************************************************/
/*																	*/
/* Function name : OnSetHome										*/
/* Description   : Make selected directory the home directory.		*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnSetHome() 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

	int nUserIndex = m_UsersList.GetItemData(nSelIndex);

    // get index of selected directory item
	POSITION pos;
	pos = m_DirectoryList.GetFirstSelectedItemPosition();
	if (!pos)
		return;
	
	int nDirIndex = m_DirectoryList.GetNextSelectedItem(pos);
	
	for (int i=0; i<m_UserArray[nUserIndex].m_DirectoryArray.GetSize(); i++)
	{
		LVITEM lvi;
		ZeroMemory(&lvi, sizeof (LV_ITEM));
		lvi.mask = LVIF_IMAGE|LVIF_PARAM;
		lvi.iItem = i;
		m_DirectoryList.GetItem(&lvi);
		if (i == nDirIndex)
		{
			// set bIsHome flag for selected directory
			lvi.iImage = 1;
			m_UserArray[nUserIndex].m_DirectoryArray[lvi.lParam].m_bIsHomeDir = TRUE;
		}
		else
		{
			// clear old bIsHomeDir flag	
			lvi.iImage = 0;
			m_UserArray[nUserIndex].m_DirectoryArray[lvi.lParam].m_bIsHomeDir = FALSE;
		}
		m_DirectoryList.SetItem(&lvi);
	}
}


void CUserAccountsDlg::OnUpdateControls(CCmdUI* pCmdUI) 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	
	pCmdUI->Enable(nSelIndex != -1 && !IsDlgButtonChecked(IDC_DISABLE_ACCOUNT));
}

void CUserAccountsDlg::OnUpdateDisableAccount(CCmdUI* pCmdUI) 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	pCmdUI->Enable(nSelIndex != -1);
}



CString CUserAccountsDlg::GetAttributes(CDirectory &dir)
{
	CString strResult;

	strResult.Format("%c%c%c%c%c", dir.m_bAllowDownload ? 'Y':'-',
								 dir.m_bAllowUpload ? 'Y':'-',
								 dir.m_bAllowRename ? 'Y':'-',
								 dir.m_bAllowDelete ? 'Y':'-',
								 dir.m_bAllowCreateDirectory ? 'Y':'-');
	return strResult;
}


/********************************************************************/
/*																	*/
/* Function name : AutoSizeColumns									*/
/* Description   : Make all columns fit nicely.						*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::AutoSizeColumns() 
{
	// Call this after your the control is filled
	m_DirectoryList.SetRedraw(FALSE);
	int mincol = 0;
    int maxcol = m_DirectoryList.GetHeaderCtrl()->GetItemCount()-1;
	int col;
    for (col = mincol; col <= maxcol; col++) 
	{
		m_DirectoryList.SetColumnWidth(col,LVSCW_AUTOSIZE);
        int wc1 = m_DirectoryList.GetColumnWidth(col);
        m_DirectoryList.SetColumnWidth(col,LVSCW_AUTOSIZE_USEHEADER);
        int wc2 = m_DirectoryList.GetColumnWidth(col);
        // 10 is minumim column width
		int wc = max(10, max(wc1,wc2));
        m_DirectoryList.SetColumnWidth(col,wc);
     }
     m_DirectoryList.SetRedraw(TRUE);
}


/********************************************************************/
/*																	*/
/* Function name : OnDisableAccount									*/
/* Description   : Disable account has been clicked					*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnDisableAccount() 
{
	UpdateDialogControls(this, FALSE);
}


/********************************************************************/
/*																	*/
/* Function name : OnWizard											*/
/* Description   : Launch New Account Wizard						*/
/*																	*/
/********************************************************************/
void CUserAccountsDlg::OnWizard() 
{
	CBitmap bmpHeader, bmpWatermark;

	VERIFY(bmpHeader.LoadBitmap(IDB_BANNER));
	VERIFY(bmpWatermark.LoadBitmap(IDB_WATERMARK));
	
	// show windows 2000-like wizard
	CWizardSheet wizSheet("New Account Wizard", this, 0, bmpWatermark, NULL, bmpHeader);
	wizSheet.m_psh.hInstance = ::GetModuleHandle(NULL);
	if (wizSheet.DoModal() == ID_WIZFINISH)
	{
		int nIndex = -1;
		int nItem = -1;
		
		// existing account ?
		for (int i=0; i<m_UsersList.GetItemCount(); i++)
		{
			CString strName;
			strName = m_UsersList.GetItemText(i, 0);
			if (strName.CompareNoCase(wizSheet.m_Page1.m_strAccountName) == 0)
			{
				nItem = i;
				nIndex = m_UsersList.GetItemData(i);
				break;
			}
		}

		// add new account
		if (nIndex == -1)
		{
			CUser user;

			int nItem = m_UsersList.InsertItem(0, wizSheet.m_Page1.m_strAccountName, 2);
			nIndex = m_UserArray.Add(user);
			m_UsersList.SetItemData(nItem, nIndex);
		}		
		else
		{
			// clear old bIsHomeDir flag	
			for (int i=0; i<m_UserArray[nIndex].m_DirectoryArray.GetSize(); i++)
			{
				m_UserArray[nIndex].m_DirectoryArray[i].m_bIsHomeDir = FALSE;
			}
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
		
		if (m_nPreviousIndex == nItem)
			m_nPreviousIndex = -1;

		// select updated account
		m_UsersList.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED); 
		OnSelchangeUserlist();	
	}	
}
	
