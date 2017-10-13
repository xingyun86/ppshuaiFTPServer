
#if !defined(AFX_WIZARDPAGES_H__0A0FEEEB_C884_4EB9_85E6_843E2DEB225E__INCLUDED_)
#define AFX_WIZARDPAGES_H__0A0FEEEB_C884_4EB9_85E6_843E2DEB225E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardPages.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardPage1 dialog

class CWizardPage1 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardPage1)

// Construction
public:
	CWizardPage1();
	~CWizardPage1();

// Dialog Data
	//{{AFX_DATA(CWizardPage1)
	enum { IDD = IDD_WIZARD_PAGE1 };
	CString	m_strAccountName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage1)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CWizardPage2 dialog

class CWizardPage2 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardPage2)

// Construction
public:
	CWizardPage2();
	~CWizardPage2();

// Dialog Data
	//{{AFX_DATA(CWizardPage2)
	enum { IDD = IDD_WIZARD_PAGE2 };
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage2)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CWizardPage3 dialog

class CWizardPage3 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardPage3)

// Construction
public:
	CWizardPage3();
	~CWizardPage3();

// Dialog Data
	//{{AFX_DATA(CWizardPage3)
	enum { IDD = IDD_WIZARD_PAGE3 };
	CString	m_strHomeDirectory;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage3)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage3)
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



/////////////////////////////////////////////////////////////////////////////
// CWizardPage4 dialog

class CWizardPage4 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardPage4)

// Construction
public:
	CWizardPage4();
	~CWizardPage4();

// Dialog Data
	//{{AFX_DATA(CWizardPage4)
	enum { IDD = IDD_WIZARD_PAGE4 };
	BOOL	m_bAllowCreateDirectory;
	BOOL	m_bAllowDelete;
	BOOL	m_bAllowDownload;
	BOOL	m_bAllowRename;
	BOOL	m_bAllowUpload;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPage4)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPage4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CWizardFinish dialog

class CWizardFinish : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardFinish)

// Construction
public:
	CWizardFinish();
	~CWizardFinish();

// Dialog Data
	//{{AFX_DATA(CWizardFinish)
	enum { IDD = IDD_WIZARD_FINISH };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardFinish)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardFinish)
	afx_msg void OnConnectnow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

class CWizardSheet : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CWizardSheet)

// Construction
public:
	CWizardSheet(UINT nIDCaption, CWnd* pParentWnd = NULL,
			UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
			HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);
	CWizardSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL,
			UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
			HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);
/*	CWizardSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CWizardSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
*/
// Attributes
public:
	void InitPages();
// Operations
public:
	CWizardPage1 m_Page1;
	CWizardPage2 m_Page2;
	CWizardPage3 m_Page3;
	CWizardPage4 m_Page4;
	CWizardFinish m_PageFinish;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWizardSheet();

	// Generated message map functions
protected:
	
	//{{AFX_MSG(CWizardSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDPAGES_H__0A0FEEEB_C884_4EB9_85E6_843E2DEB225E__INCLUDED_)
