
#if !defined(AFX_STATISTICSPAGE_H__8435DE26_16BC_4C60_BE40_4D45DF1826F9__INCLUDED_)
#define AFX_STATISTICSPAGE_H__8435DE26_16BC_4C60_BE40_4D45DF1826F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogResize.h"
#include "resource.h"

class CStatisticsPage : public CDialogResize
{
// Construction
public:
	void SetValue(int nType, int nValue);
	void SetValue(int nType, LPCTSTR lpszValue);
	CStatisticsPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatisticsPage)
	enum { IDD = IDD_PROPPAGE_STATISTICS };
	CListCtrl	m_StatisticsCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatisticsPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DLGRESIZE_MAP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSPAGE_H__8435DE26_16BC_4C60_BE40_4D45DF1826F9__INCLUDED_)
