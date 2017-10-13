//Download by http://www.NewXing.com
#if !defined(AFX_TABCTRLEX_H__F3E8650F_019C_479F_9E0F_60FE1181F49F__INCLUDED_)
#define AFX_TABCTRLEX_H__F3E8650F_019C_479F_9E0F_60FE1181F49F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx window

class CTabCtrlEx : public CTabCtrl
{
// Construction
public:
	CTabCtrlEx();
	CDialog *m_tabPages[4];
	int m_tabCurrent;
	int m_nNumberOfPages;

// Operations
public:
	void Init();
	void SetRectangle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetServerInfo(int nItem, LPCTSTR lpszAddress);
	void SetTextColor(COLORREF clrText);
	void SetBkColor(COLORREF crBkgnd);
	virtual ~CTabCtrlEx();

	// Generated message map functions
protected:
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;

	CBrush m_brBkgnd;
	CBrush m_brText;

	//{{AFX_MSG(CTabCtrlEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRLEX_H__F3E8650F_019C_479F_9E0F_60FE1181F49F__INCLUDED_)
