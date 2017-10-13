
/****************************************************************/
/*																*/
/*  DirectoryDlg.cpp											*/
/*																*/
/*  Implementation of the CDirectoryDlg class.					*/
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
#include "DirectoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDirectoryDlg::CDirectoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirectoryDlg)
	m_strPath = _T("");
	m_bVirtualDir = FALSE;
	m_strAlias = _T("");
	m_bAllowCreateDirectory = FALSE;
	m_bAllowDelete = FALSE;
	m_bAllowDownload = FALSE;
	m_bAllowRename = FALSE;
	m_bAllowUpload = FALSE;
	//}}AFX_DATA_INIT
}


void CDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectoryDlg)
	DDX_Text(pDX, IDC_PATH, m_strPath);
	DDX_Check(pDX, IDC_VIRTUALDIR, m_bVirtualDir);
	DDX_Text(pDX, IDC_ALIAS, m_strAlias);
	DDX_Check(pDX, IDC_CREATE_DIR, m_bAllowCreateDirectory);
	DDX_Check(pDX, IDC_DELETE, m_bAllowDelete);
	DDX_Check(pDX, IDC_DOWNLOAD, m_bAllowDownload);
	DDX_Check(pDX, IDC_RENAME, m_bAllowRename);
	DDX_Check(pDX, IDC_UPLOAD, m_bAllowUpload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirectoryDlg, CDialog)
	//{{AFX_MSG_MAP(CDirectoryDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_VIRTUALDIR, OnVirtualdir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDirectoryDlg::OnBrowse() 
{
	CString strDir = BrowseForFolder(m_hWnd, "Select a directory:", BIF_RETURNONLYFSDIRS);
	if (!strDir.IsEmpty())
	{
		m_strPath = strDir;
		UpdateData(FALSE);
	}
}

BOOL CDirectoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);

	if (m_bIsHomeDir)
	{
		GetDlgItem(IDC_VIRTUALDIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_ALIAS)->EnableWindow(FALSE);
		GetDlgItem(IDC_ALIAS)->SetWindowText("/");
	}
	else
	{
		OnVirtualdir();
	}

	return TRUE; 
}

void CDirectoryDlg::OnVirtualdir() 
{
	UpdateData();
	if (m_bVirtualDir)
		GetDlgItem(IDC_ALIAS)->EnableWindow();
	else
		GetDlgItem(IDC_ALIAS)->EnableWindow(FALSE);
}
