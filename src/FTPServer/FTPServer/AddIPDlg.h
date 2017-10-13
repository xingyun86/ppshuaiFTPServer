
#if !defined(AFX_ADDIPDLG_H__5DE6C4D7_BFBC_4184_8ACE_E371F57DE55E__INCLUDED_)
#define AFX_ADDIPDLG_H__5DE6C4D7_BFBC_4184_8ACE_E371F57DE55E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddIPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddIPDlg dialog

class CAddIPDlg : public CDialog
{
// Construction
public:
	CString m_strTitle;
	CAddIPDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddIPDlg)
	enum { IDD = IDD_ADD_IP };
	CString	m_strIPaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddIPDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddIPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDIPDLG_H__5DE6C4D7_BFBC_4184_8ACE_E371F57DE55E__INCLUDED_)
