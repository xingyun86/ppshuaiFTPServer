
/****************************************************************/
/*																*/
/*  TracePage.cpp												*/
/*																*/
/*  Implementation of the CTracePage class.						*/
/*	This class is a part of the FTP Server Application			*/
/*																*/
/*  Programmed by xingyun86							*/
/*  Copyright @2017					*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "FTPServerApp.h"
#include "TracePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTracePage, CDialog)

CTracePage::CTracePage() : CDialog(CTracePage::IDD)
{
	//{{AFX_DATA_INIT(CTracePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTracePage::~CTracePage()
{
	while(!m_LogQueue.IsEmpty())
	{
		CLogMsg *pLogMsg = (CLogMsg *)m_LogQueue.RemoveHead();
		delete pLogMsg;
	}
}

void CTracePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTracePage)
	DDX_Control(pDX, IDC_TRACE, m_TraceList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTracePage, CDialog)
	//{{AFX_MSG_MAP(CTracePage)
	ON_WM_SIZE()
	ON_COMMAND(ID_TRACE_CLEAR, OnTraceClear)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDTRACELINE, OnAddTraceLine)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTracePage message handlers

void CTracePage::AddTraceLine(int nLevel, LPCTSTR pstrFormat, ...)
{
	CString str;

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);

	try
	{
		// create new message
		CLogMsg *pLogMsg = new CLogMsg;

		GetLocalTime(&pLogMsg->m_sysTime);
		pLogMsg->m_nLevel = nLevel;
		pLogMsg->m_strText = str;

		m_QueueLock.Lock();
		m_LogQueue.AddTail(pLogMsg);
		m_QueueLock.Unlock();
				
		// schedule log action
		PostMessage(WM_ADDTRACELINE);
	}
	catch(...)
	{
	
	}
}


LRESULT CTracePage::OnAddTraceLine(WPARAM, LPARAM)
{
	CLogMsg *pLogMsg;
	
	try
	{
		// get first message in the queue
//		m_QueueLock.Lock();
		pLogMsg = (CLogMsg *)m_LogQueue.RemoveHead();
//		m_QueueLock.Unlock();

		switch(pLogMsg->m_nLevel)
		{
			case 1:
				m_TraceList.AddString(pLogMsg->m_strText, RGB(0,130,0));
				break;
			case 2:
				m_TraceList.AddString(pLogMsg->m_strText, RGB(0,0,255));
				break;
			case 3:
				m_TraceList.AddString(pLogMsg->m_strText, RGB(255,0,0));
				break;
			default:
				m_TraceList.AddString(pLogMsg->m_strText, RGB(0,0,0));
				break;

		}
/*		CString strDateTime;
		strDateTime.Format("%02d/%02d/%02d %02d:%02d:%02d.%03d ", 
							pLogMsg->m_sysTime.wDay, pLogMsg->m_sysTime.wMonth, pLogMsg->m_sysTime.wYear,
							pLogMsg->m_sysTime.wHour, pLogMsg->m_sysTime.wMinute, pLogMsg->m_sysTime.wSecond, 
							pLogMsg->m_sysTime.wMilliseconds);

		switch(pLogMsg->m_nLevel)
		{
			case 1:
				m_LogFile << error_lvl << strDateTime << pLogMsg->m_strText << endl;
				break;
			case 2:
				m_LogFile << warning_lvl << strDateTime << pLogMsg->m_strText << endl;
				break;
			case 3:
			default:
				m_LogFile << trace_lvl << strDateTime << pLogMsg->m_strText << endl;
				break;
		} */
		delete pLogMsg;
	}
	catch(...)
	{
		// something is wrong...
	}
	return TRUE;
}

void CTracePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_TRACE)))
	{
		CRect rect;
		GetClientRect(rect);
//		rect.DeflateRect(5,5,10,10);
		m_TraceList.MoveWindow(rect);
	}
}

void CTracePage::OnTraceClear() 
{
	m_TraceList.ResetContent();	
}

void CTracePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_TRACE_MENU));
	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);		
}
