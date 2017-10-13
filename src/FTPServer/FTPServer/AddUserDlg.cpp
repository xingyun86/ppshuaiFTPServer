
/****************************************************************/
/*																*/
/*  AddUserDlg.cpp												*/
/*																*/
/*  Implementation of the CAddUserDlg class.					*/
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
#include "AddUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CAddUserDlg::CAddUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddUserDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CAddUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddUserDlg)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAddUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAddUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);

	CenterWindow();
	return TRUE;
}


