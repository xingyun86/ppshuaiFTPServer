
#if !defined(AFX_TRACEPAGE_H__749D54AC_2C20_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_TRACEPAGE_H__749D54AC_2C20_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracePage.h : header file
//
#include "ColorListBox.h"

class CTracePage : public CDialog
{
	DECLARE_DYNCREATE(CTracePage)

// Construction
public:
	CTracePage();
	~CTracePage();

	void AddTraceLine(int nLevel, LPCTSTR pstrFormat, ...);
// Dialog Data
	//{{AFX_DATA(CTracePage)
	enum { IDD = IDD_PROPPAGE_TRACE };
	CColorListBox	m_TraceList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTracePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTracePage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTraceClear();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	LRESULT OnAddTraceLine(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	CObList m_LogQueue;
	CCriticalSection m_QueueLock;

	class CLogMsg : public CObject
	{
	public:
		CLogMsg() {};
		virtual ~CLogMsg() 
		{
		};
	
		SYSTEMTIME m_sysTime;
		int		m_nLevel;
		CString m_strText;
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEPAGE_H__749D54AC_2C20_11D6_AB38_00D0B70C3D79__INCLUDED_)
