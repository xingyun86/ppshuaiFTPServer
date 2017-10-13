
#if !defined(AFX_DIRECTORYDLG_H__2B2B3B91_220F_11D6_AB35_00D0B70C3D79__INCLUDED_)
#define AFX_DIRECTORYDLG_H__2B2B3B91_220F_11D6_AB35_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectoryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirectoryDlg dialog

class CDirectoryDlg : public CDialog
{
// Construction
public:
	BOOL m_bIsHomeDir;
	CDirectoryDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_strTitle;
// Dialog Data
	//{{AFX_DATA(CDirectoryDlg)
	enum { IDD = IDD_ADD_DIRECTORY };
	CString	m_strPath;
	BOOL	m_bVirtualDir;
	CString	m_strAlias;
	BOOL	m_bAllowCreateDirectory;
	BOOL	m_bAllowDelete;
	BOOL	m_bAllowDownload;
	BOOL	m_bAllowRename;
	BOOL	m_bAllowUpload;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDirectoryDlg)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnVirtualdir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTORYDLG_H__2B2B3B91_220F_11D6_AB35_00D0B70C3D79__INCLUDED_)
