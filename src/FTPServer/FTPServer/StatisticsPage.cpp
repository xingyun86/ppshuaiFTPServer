
// StatisticsPage.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "StatisticsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsPage dialog


CStatisticsPage::CStatisticsPage() : CDialogResize(CStatisticsPage::IDD)
{
	//{{AFX_DATA_INIT(CStatisticsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStatisticsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsPage)
	DDX_Control(pDX, IDC_STATISTICS, m_StatisticsCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsPage, CDialogResize)
	//{{AFX_MSG_MAP(CStatisticsPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CStatisticsPage)
	DLGRESIZE_CONTROL(IDC_STATISTICS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
END_DLGRESIZE_MAP()


BOOL CStatisticsPage::OnInitDialog() 
{
	CDialogResize::OnInitDialog();
	
	InitResizing(FALSE, FALSE, WS_CLIPCHILDREN);
	
	m_StatisticsCtrl.InsertColumn(0, "Statistic");		
	m_StatisticsCtrl.InsertColumn(1, "Value");	

	m_StatisticsCtrl.InsertItem(0, "Total Connections");
	m_StatisticsCtrl.SetItemText(0, 1, "0");
	m_StatisticsCtrl.InsertItem(1, "Current Connections");
	m_StatisticsCtrl.SetItemText(1, 1, "0");
	m_StatisticsCtrl.InsertItem(2, "Files Downloaded");
	m_StatisticsCtrl.SetItemText(2, 1, "0");
	m_StatisticsCtrl.InsertItem(3, "Files Uploaded");
	m_StatisticsCtrl.SetItemText(3, 1, "0");
	m_StatisticsCtrl.InsertItem(4, "Failed Downloads");
	m_StatisticsCtrl.SetItemText(4, 1, "0");
	m_StatisticsCtrl.InsertItem(5, "Failed Uploads");
	m_StatisticsCtrl.SetItemText(5, 1, "0");
	m_StatisticsCtrl.InsertItem(6, "Total kilobytes received");
	m_StatisticsCtrl.SetItemText(6, 1, "0");
	m_StatisticsCtrl.InsertItem(7, "Total kilobytes sent");
	m_StatisticsCtrl.SetItemText(7, 1, "0");
	
	DWORD dwStyle = m_StatisticsCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
    m_StatisticsCtrl.SetExtendedStyle(dwStyle);

	AutoSizeColumns(&m_StatisticsCtrl);
	return TRUE;
}


void CStatisticsPage::SetValue(int nType, int nValue)
{
	CString strValue;
	strValue.Format("%d", nValue);
	m_StatisticsCtrl.SetItemText(nType, 1, strValue);
}

void CStatisticsPage::SetValue(int nType, LPCTSTR lpszValue)
{
	m_StatisticsCtrl.SetItemText(nType, 1, lpszValue);
}
