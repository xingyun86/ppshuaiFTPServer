
#if !defined(AFX_INFOBARCTRL_H__4CCD6BDB_9694_47EC_A643_42A2CEBF62DF__INCLUDED_)
#define AFX_INFOBARCTRL_H__4CCD6BDB_9694_47EC_A643_42A2CEBF62DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfobarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfobarCtrl window

class CInfobarCtrl : public CStatic
{
// Construction
public:
	CInfobarCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfobarCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetText(LPCTSTR lpszText);
	virtual ~CInfobarCtrl();

	// Generated message map functions
protected:
	CFont m_TextFont;
	CRect m_rcButton;
	BOOL m_bMouseDown;
	int m_bMouseInButton;
	CString m_strText;
	//{{AFX_MSG(CInfobarCtrl)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOBARCTRL_H__4CCD6BDB_9694_47EC_A643_42A2CEBF62DF__INCLUDED_)
