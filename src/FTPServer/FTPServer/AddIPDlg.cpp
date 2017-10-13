
/****************************************************************/
/*																*/
/*  AddIPDlg.cpp												*/
/*																*/
/*  Implementation of the CAddIPDlg class.						*/
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
#include "ftpserver.h"
#include "AddIPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAddIPDlg::CAddIPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddIPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddIPDlg)
	m_strIPaddress = _T("");
	//}}AFX_DATA_INIT
}


void CAddIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddIPDlg)
	DDX_Text(pDX, IDC_IPADDRESS, m_strIPaddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddIPDlg, CDialog)
	//{{AFX_MSG_MAP(CAddIPDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/
/* Description   : Initialize dialog								*/
/*																	*/
/********************************************************************/
BOOL CAddIPDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);	

	// ip address can be 255.255.255.255 = 15 chars max.
	((CEdit *)GetDlgItem(IDC_IPADDRESS))->SetLimitText(15);

	CenterWindow();
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : PreTranslateMessage								*/		
/* Description   : Only accept numeric characters, decimal point	*/
/*				   separators, backspace and tabs.					*/
/*																	*/
/********************************************************************/
BOOL CAddIPDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_CHAR && pMsg->hwnd == ::GetDlgItem(m_hWnd, IDC_IPADDRESS))
	{
		TCHAR nChar = (TCHAR)pMsg->wParam;
		if ((IsCharAlphaNumeric(nChar) && !IsCharAlpha(nChar)) ||
			nChar == '.' || nChar == '\b' || nChar == '\t')
		{
			return CDialog::PreTranslateMessage(pMsg);
		}
		else
		{
			// eat character
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

