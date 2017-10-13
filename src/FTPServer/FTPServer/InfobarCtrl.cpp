
/****************************************************************/
/*																*/
/*  InfobarCtrl.cpp												*/
/*																*/
/*  Implementation of the CInfobarCtrl class.					*/
/*	This class imitates the outlook infobar.					*/
/*																*/
/*  Programmed by xingyun86							*/
/*  Copyright @2017					*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "InfobarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CInfobarCtrl::CInfobarCtrl()
{
	m_bMouseInButton = FALSE;
	m_bMouseDown = FALSE;
}

CInfobarCtrl::~CInfobarCtrl()
{
}


BEGIN_MESSAGE_MAP(CInfobarCtrl, CStatic)
	//{{AFX_MSG_MAP(CInfobarCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : OnPaint											*/
/* Description   : This is where all the painting of our control	*/
/*				   takes place.										*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* pOldBitmap;

	CRect rcClient;
	GetClientRect(&rcClient);

	// to avoid flicker, establish a memory dc, draw to it and then BitBlt it to the client
	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	pOldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);

	// paint background
	memDC.FillSolidRect(rcClient, GetSysColor(COLOR_3DSHADOW));

	// select font
	CFont* pOldFont = (CFont*)memDC.SelectObject(&m_TextFont);
	COLORREF oldTextColor;
	int nBkMode = memDC.SetBkMode(TRANSPARENT);

	// set text color to white
	oldTextColor = memDC.SetTextColor(RGB(255,255,255));

	CRect rcItem = m_rcButton;
	if (rcItem.right >= rcClient.right)
		rcItem.right = rcClient.right-1;

	if (m_bMouseInButton)
	{
		if (m_bMouseDown)
			memDC.Draw3dRect(rcItem, RGB(0,0,0), RGB(255,255,255));
		else
			memDC.Draw3dRect(rcItem, RGB(255,255,255), RGB(0,0,0));
	}

	rcItem.DeflateRect(5, 0);
	rcItem.right -= 12;
	// draw text
	::DrawTextEx(memDC.m_hDC,
                m_strText.GetBuffer (0),
                m_strText.GetLength(),
                &rcItem,
                DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_PATH_ELLIPSIS,
                NULL);

	// draw a arrow
	rcItem.right += 12;
	rcItem.left = rcItem.right-8;
	rcItem.top += rcItem.Height() / 2 - 1;
	CPoint pt[3];
	pt[0] = rcItem.TopLeft();
	pt[1] = CPoint(rcItem.right, rcItem.top);
	pt[2] = CPoint(rcItem.left + rcItem.Width()/2, rcItem.top + rcItem.Width()/2);
	memDC.SelectStockObject(WHITE_BRUSH);
	memDC.SelectStockObject(NULL_PEN);
	memDC.Polygon(pt, 3);

	// finally send the result to the display
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	// restore old values
	memDC.SelectObject(pOldFont);		
	memDC.SetTextColor(oldTextColor);
	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBitmap);
}


/********************************************************************/
/*																	*/
/* Function name : OnLButtonDown									*/
/* Description   : Handle WM_LBUTTONDOWN message					*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bMouseInButton)
	{
		if (!m_bMouseDown)
			m_bMouseDown = TRUE;

		Invalidate();
	}	
	CStatic::OnLButtonDown(nFlags, point);
}


/********************************************************************/
/*																	*/
/* Function name : OnLButtonUp										*/
/* Description   : Handle WM_LBUTTONUP message						*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bMouseInButton)
	{
		int bOldMode = m_bMouseInButton;
		if (m_rcButton.PtInRect(point))
			m_bMouseInButton = TRUE;
		else 
			m_bMouseInButton = FALSE;
		
		if (m_bMouseDown)
		{
			m_bMouseDown = FALSE;

			if (m_bMouseInButton == bOldMode)
			{
				Invalidate();
				
				// do something ...
				if (m_bMouseInButton)
				{
					Invalidate();

					// load and display popup menu
					CMenu menu;
					menu.LoadMenu(IDR_INFOBARMENU);
					CMenu* pPopup = menu.GetSubMenu(0);
					
					CRect rect;
					GetWindowRect(rect);
				
				    // show and track the menu
					DWORD dwID = pPopup->TrackPopupMenu((TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
                                                  rect.left, rect.bottom, this);
					GetParent()->SendMessage(WM_COMMAND, dwID);
				}
			}
			else
			{
				m_bMouseInButton = FALSE;
				Invalidate();
			}
		}
	}	
	CStatic::OnLButtonUp(nFlags, point);
}


/********************************************************************/
/*																	*/
/* Function name : OnMouseMove										*/
/* Description   : Handle WM_MOUSEMOVE message						*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
		m_bMouseDown = TRUE;
	else
		m_bMouseDown = FALSE;

	BOOL bOldState = m_bMouseInButton;

	if (m_rcButton.PtInRect(point))
		m_bMouseInButton = TRUE;
	else 
		m_bMouseInButton = FALSE;

	if (m_bMouseInButton != bOldState)
	{
		Invalidate(FALSE);

		KillTimer(1);
		SetTimer(1,10,NULL);
	}
	
	CStatic::OnMouseMove(nFlags, point);
}


/********************************************************************/
/*																	*/
/* Function name : OnTimer											*/
/* Description   : Handle WM_TIMER message -> process timer			*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		CPoint pt;
		if (!GetCursorPos(&pt))
			return;
		ScreenToClient(&pt);
		
		CRect rcClient;
		GetClientRect(&rcClient);
		rcClient.bottom = rcClient.top + 22;

		if (!rcClient.PtInRect(pt))
		{
			KillTimer(1);

			m_bMouseInButton = FALSE;
			m_bMouseDown = FALSE;

			Invalidate();
		}
	}
	CStatic::OnTimer(nIDEvent);
}


/********************************************************************/
/*																	*/
/* Function name : SetText											*/
/* Description   : Set text of the control							*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::SetText(LPCTSTR lpszText)
{
	m_strText = lpszText;

	CClientDC dc(this);
	CFont* pFont = (CFont*)dc.SelectObject(&m_TextFont);

	CRect rcClient;
	GetClientRect(&rcClient);

	// calculate new button rect
	CSize size = dc.GetTextExtent(m_strText);
	size.cx += 22;
	size.cy = rcClient.Height()-2;
	m_rcButton = CRect(CPoint(1, 1), size);

	dc.SelectObject(pFont);

	Invalidate();
}


/********************************************************************/
/*																	*/
/* Function name : PreSubclassWindow								*/
/* Description   : Initialize control variables						*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::PreSubclassWindow() 
{
	m_TextFont.CreateFont(16, 0,0,0,FW_BOLD, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	
	CStatic::PreSubclassWindow();
}
