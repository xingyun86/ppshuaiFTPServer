
#if !defined(AFX_SECURITYPAGE_H__898CB245_9C3E_4693_8555_20DBC1664830__INCLUDED_)
#define AFX_SECURITYPAGE_H__898CB245_9C3E_4693_8555_20DBC1664830__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogResize.h"


class CSecurityPage : public CDialogResize
{
// Construction
public:
	void AddIPToBlockList(LPCTSTR lpszIP);
	CSecurityPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSecurityPage)
	enum { IDD = IDD_PROPPAGE_SECURITY };
	CListBox	m_BlockedList;
	CListBox	m_NonBlockedList;
	BOOL	m_bBlockAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecurityPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateSecurityData(int nType);

	// Generated message map functions
	//{{AFX_MSG(CSecurityPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBlockAll();
	afx_msg void OnDestroy();
	afx_msg void OnAddBlock();
	afx_msg void OnEditBlock();
	afx_msg void OnAddNonblock();
	afx_msg void OnEditNonblock();
	afx_msg void OnRemoveBlock();
	afx_msg void OnRemoveNonblock();
	afx_msg void OnDblclkBlockedlist();
	afx_msg void OnDblclkNonblockedlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DLGRESIZE_MAP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURITYPAGE_H__898CB245_9C3E_4693_8555_20DBC1664830__INCLUDED_)
