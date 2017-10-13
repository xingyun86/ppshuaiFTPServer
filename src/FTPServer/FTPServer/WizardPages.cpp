
/****************************************************************/
/*																*/
/*  WizardPages.cpp												*/
/*																*/
/*  Implementation of the New User Wizard.						*/
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
#include "WizardPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CWizardSheet, CPropertySheetEx)

CWizardSheet::CWizardSheet(UINT nIDCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(nIDCaption, pParentWnd, iSelectPage,
				  hbmWatermark, hpalWatermark, hbmHeader)
{
	// add all the pages of the wizard
	InitPages();

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;
	m_psh.dwFlags &= ~PSH_HASHELP; 
}

CWizardSheet::CWizardSheet(LPCTSTR pszCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(pszCaption, pParentWnd, iSelectPage,
					  hbmWatermark, hpalWatermark, hbmHeader)

{
	// add all the pages of the wizard
	InitPages();

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;
	m_psh.dwFlags &= ~PSH_HASHELP; 
}


CWizardSheet::~CWizardSheet()
{
}


BEGIN_MESSAGE_MAP(CWizardSheet, CPropertySheetEx)
	//{{AFX_MSG_MAP(CWizardSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : InitPages										*/
/* Description   : Add pages to wizard.								*/
/*																	*/
/********************************************************************/
void CWizardSheet::InitPages()
{
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_PageFinish);
}



IMPLEMENT_DYNCREATE(CWizardPage1, CPropertyPageEx)

CWizardPage1::CWizardPage1() : CPropertyPageEx(CWizardPage1::IDD, 0, IDS_HEADERTITLE1, NULL)
{
	//{{AFX_DATA_INIT(CWizardPage1)
	m_strAccountName = _T("");
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP; 
}

CWizardPage1::~CWizardPage1()
{
}

void CWizardPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage1)
	DDX_Text(pDX, IDC_ACCOUNTNAME, m_strAccountName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage1, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizardPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnSetActive										*/
/* Description   : Called when this becomes the active page.		*/
/*																	*/
/********************************************************************/
BOOL CWizardPage1::OnSetActive() 
{
	CPropertySheetEx* parent = (CPropertySheetEx*)GetParent();
	parent->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPageEx::OnSetActive();
}


/********************************************************************/
/*																	*/
/* Function name : OnWizardNext										*/
/* Description   : Called when Next button has been pressed.		*/
/*																	*/
/********************************************************************/
LRESULT CWizardPage1::OnWizardNext() 
{
	CEdit *editBox = (CEdit *)GetDlgItem(IDC_ACCOUNTNAME);
	if (editBox->GetWindowTextLength() == 0)
	{
		MessageBox("You must enter an account name.", "Account Wizard",  MB_OK | MB_ICONEXCLAMATION);
		editBox->SetFocus();
		return -1;
	}	
	return CPropertyPageEx::OnWizardNext();
}


IMPLEMENT_DYNCREATE(CWizardPage2, CPropertyPageEx)

CWizardPage2::CWizardPage2() : CPropertyPageEx(CWizardPage2::IDD, 0, IDS_HEADERTITLE2, NULL)
{
	//{{AFX_DATA_INIT(CWizardPage2)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP; 
}

CWizardPage2::~CWizardPage2()
{
}

void CWizardPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage2)
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage2, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizardPage2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnSetActive										*/
/* Description   : Called when this becomes the active page.		*/
/*																	*/
/********************************************************************/
BOOL CWizardPage2::OnSetActive() 
{
	CPropertySheetEx* parent = (CPropertySheetEx*)GetParent();
	parent->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	
	return CPropertyPageEx::OnSetActive();
}


/********************************************************************/
/*																	*/
/* Function name : OnWizardNext										*/
/* Description   : Called when Next button has been pressed.		*/
/*																	*/
/********************************************************************/
LRESULT CWizardPage2::OnWizardNext() 
{
	CString strPassword, strConfirmPassword;

	GetDlgItemText(IDC_PASSWORD, strPassword);
	GetDlgItemText(IDC_CONFIRMPASSWORD, strConfirmPassword);

	if (strPassword != strConfirmPassword)
	{
		MessageBox("The passwords do not match!\r\nPlease make sure 'Password' and 'Confirm Password' are the same.", "Connection Wizard",  MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		return -1;
	}	
	return CPropertyPageEx::OnWizardNext();
}


IMPLEMENT_DYNCREATE(CWizardPage3, CPropertyPageEx)

CWizardPage3::CWizardPage3() : CPropertyPageEx(CWizardPage3::IDD, 0, IDS_HEADERTITLE3, NULL)
{
	//{{AFX_DATA_INIT(CWizardPage3)
	m_strHomeDirectory = _T("");
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP; 
}

CWizardPage3::~CWizardPage3()
{
}

void CWizardPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage3)
	DDX_Text(pDX, IDC_HOMEDIRECTORY, m_strHomeDirectory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage3, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizardPage3)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnSetActive										*/
/* Description   : Called when this becomes the active page.		*/
/*																	*/
/********************************************************************/
BOOL CWizardPage3::OnSetActive() 
{
	CPropertySheetEx* parent = (CPropertySheetEx*)GetParent();
	parent->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);		
	return CPropertyPageEx::OnSetActive();
}


/********************************************************************/
/*																	*/
/* Function name : OnBrowse											*/
/* Description   : Browse for folder.								*/
/*																	*/
/********************************************************************/
void CWizardPage3::OnBrowse() 
{
	CString strDir = BrowseForFolder(m_hWnd, "Select a directory:", BIF_RETURNONLYFSDIRS);
	if (!strDir.IsEmpty())
	{
		GetDlgItem(IDC_HOMEDIRECTORY)->SetWindowText(strDir);
	}	
}


/********************************************************************/
/*																	*/
/* Function name : OnWizardNext										*/
/* Description   : Called when Next button has been pressed.		*/
/*																	*/
/********************************************************************/
LRESULT CWizardPage3::OnWizardNext() 
{
	CString strHomeDirectory;
	if (GetDlgItemText(IDC_HOMEDIRECTORY, strHomeDirectory) == 0)
	{
		MessageBox("You must specify a home directory.", "Account Wizard",  MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_HOMEDIRECTORY)->SetFocus();
		return -1;
	}	

	// check if it's a valid directory
	if (GetFileAttributes(strHomeDirectory) == 0xFFFFFFFF)
	{
		MessageBox("The directory you entered is not valid.", "Account Wizard",  MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_HOMEDIRECTORY)->SetFocus();
		return -1;
	}
	return CPropertyPageEx::OnWizardNext();
}


IMPLEMENT_DYNCREATE(CWizardPage4, CPropertyPageEx)

CWizardPage4::CWizardPage4() : CPropertyPageEx(CWizardPage4::IDD, 0, IDS_HEADERTITLE4, NULL)
{
	//{{AFX_DATA_INIT(CWizardPage4)
	m_bAllowCreateDirectory = FALSE;
	m_bAllowDelete = FALSE;
	m_bAllowDownload = FALSE;
	m_bAllowRename = FALSE;
	m_bAllowUpload = FALSE;
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP; 
}

CWizardPage4::~CWizardPage4()
{
}

void CWizardPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage4)
	DDX_Check(pDX, IDC_CREATE_DIR, m_bAllowCreateDirectory);
	DDX_Check(pDX, IDC_DELETE, m_bAllowDelete);
	DDX_Check(pDX, IDC_DOWNLOAD, m_bAllowDownload);
	DDX_Check(pDX, IDC_RENAME, m_bAllowRename);
	DDX_Check(pDX, IDC_UPLOAD, m_bAllowUpload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage4, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizardPage4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/
/* Description   : Initialize dialog.								*/
/*																	*/
/********************************************************************/
BOOL CWizardPage4::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : OnSetActive										*/
/* Description   : Called when this becomes the active page.		*/
/*																	*/
/********************************************************************/
BOOL CWizardPage4::OnSetActive() 
{
	CPropertySheetEx* parent = (CPropertySheetEx*)GetParent();
	parent->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT);	
	return CPropertyPageEx::OnSetActive();
}



IMPLEMENT_DYNCREATE(CWizardFinish, CPropertyPageEx)

CWizardFinish::CWizardFinish() : CPropertyPageEx(CWizardFinish::IDD)
{
	//{{AFX_DATA_INIT(CWizardFinish)
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HIDEHEADER;
	m_psp.dwFlags &= ~PSP_HASHELP; 
}

CWizardFinish::~CWizardFinish()
{
}

void CWizardFinish::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardFinish)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardFinish, CPropertyPageEx)
	//{{AFX_MSG_MAP(CWizardFinish)
	ON_BN_CLICKED(IDC_CONNECTNOW, OnConnectnow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnSetActive										*/
/* Description   : Called when this becomes the active page.		*/
/*																	*/
/********************************************************************/
BOOL CWizardFinish::OnSetActive() 
{
	CPropertySheetEx* parent = (CPropertySheetEx*)GetParent();
	parent->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);	
	return CPropertyPageEx::OnSetActive();
}


/********************************************************************/
/*																	*/
/* Function name : OnConnectnow										*/
/* Description   :													*/
/*																	*/
/********************************************************************/
void CWizardFinish::OnConnectnow() 
{
	UpdateData();	
}


