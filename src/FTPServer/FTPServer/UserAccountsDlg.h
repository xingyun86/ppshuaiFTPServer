
#if !defined(AFX_USERACCOUNTSDLG_H__2B2B3B91_220F_11D6_AB35_00D0B70C3D79__INCLUDED_)
#define AFX_USERACCOUNTSDLG_H__2B2B3B91_220F_11D6_AB35_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogResize.h"
#include "UserManager.h"

class CUserAccountsDlg : public CDialogResize
{
public:
	CUserAccountsDlg(CWnd* pParent = NULL);
	~CUserAccountsDlg();

	CString m_strUserName;

	CArray<CUser, CUser&> m_UserArray;

	//{{AFX_DATA(CUserAccountsDlg)
	enum { IDD = IDD_USERACCOUNTS };
	CListCtrl	m_UsersList;
	CListCtrl	m_DirectoryList;
	CString	m_strPassword;
	BOOL	m_bDisableAccount;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CUserAccountsDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;
	void AutoSizeColumns();
	CString GetAttributes(CDirectory &directory);
	CImageList m_ImageList;
	int m_nPreviousIndex;
	
	//{{AFX_MSG(CUserAccountsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeUserlist();
	virtual void OnOK();
	afx_msg void OnDblclkDirlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddDir();
	afx_msg void OnEditDir();
	afx_msg void OnDelDir();
	afx_msg void OnSetHome();
	afx_msg void OnBrowse();
	afx_msg void OnAddUser();
	afx_msg void OnEditUser();
	afx_msg void OnDelUser();
	afx_msg void OnUpdateControls(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableAccount(CCmdUI* pCmdUI) ;
	afx_msg void OnClickUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDisableAccount();
	afx_msg void OnWizard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DLGRESIZE_MAP;
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_USERACCOUNTSDLG_H__2B2B3B91_220F_11D6_AB35_00D0B70C3D79__INCLUDED_)
