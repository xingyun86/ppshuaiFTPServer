//Download by http://www.NewXing.com

#include "stdafx.h"
#include "resource.h"
#include "TabCtrlEx.h"

#include "TabOne.h"
#include "TabTwo.h"
#include "TabThree.h"
#include "TabFour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTabCtrlEx::CTabCtrlEx()
{
	m_tabPages[0]=new CTabOne;
	m_tabPages[1]=new CTabTwo;
	m_tabPages[2]=new CTabThree;
	m_tabPages[3]=new CTabFour;

	m_nNumberOfPages = 4;

	SetBkColor(RGB(192, 192, 192));
	SetTextColor(RGB(0, 0, 0));
}


CTabCtrlEx::~CTabCtrlEx()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++)
	{
		delete m_tabPages[nCount];
	}
}


void CTabCtrlEx::Init()
{
	InsertItem(0, "Service");
	InsertItem(1, "Dag");
	InsertItem(2, "Details");
	InsertItem(3, "Connections");

	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_TAB_ONE, this);
	m_tabPages[1]->Create(IDD_TAB_TWO, this);
	m_tabPages[2]->Create(IDD_TAB_THREE, this);
	m_tabPages[3]->Create(IDD_FAB_FOUR, this);
	
	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);

	SetRectangle();
}


void CTabCtrlEx::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++)
	{
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx message handlers

void CTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus())
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
}


void CTabCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);

	CRect rect;
	RECT rcTab;

	GetWindowRect(rect);
	ScreenToClient(rect);
	
	GetItemRect(0, &rcTab);

	int dx = 2 * GetSystemMetrics(SM_CXEDGE);
	int dy = 2 * GetSystemMetrics(SM_CYEDGE);

	for(int nCount=0; nCount < m_nNumberOfPages; nCount++)
	{
		m_tabPages[nCount]->SetWindowPos(NULL, 0, 0, rect.Width() - (2 * dx), rect.Height() - (2 * dy) - (rcTab.bottom - rcTab.top), SWP_NOMOVE | SWP_NOZORDER);;
	}
}


