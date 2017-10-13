
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D3E3BA89_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_)
#define AFX_STDAFX_H__D3E3BA89_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>
#include <afxsock.h>		// MFC socket extensions

#include <afxtempl.h>
#include <shlobj.h>
#include "shfolder.h"


#define FTPSTAT_DOWNLOADSUCCEEDED	2
#define FTPSTAT_UPLOADSUCCEEDED		3
#define FTPSTAT_DOWNLOADFAILED		4
#define FTPSTAT_UPLOADFAILED		5

#define WM_THREADSTART	WM_USER+200
#define WM_THREADCLOSE	WM_USER+201
#define WM_THREADMSG	WM_USER+202
#define WM_ADDTRACELINE WM_USER+203

extern CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags);
extern void DoEvents();
extern void GetAppDir(CString& strAppDir);
extern BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout);

PFNSHGETFOLDERPATHA GetFuncPtr_SHGetFolderPathA();
extern HRESULT CreateStartMenuShortcut(LPSTR lpszShortcutFile, LPSTR lpszDescription, LPTSTR lpszRelativeFolder);
extern void RemoveStartMenuShortcut(LPSTR lpszDescription, LPTSTR lpszRelativeFolder);
extern CString GetShortcutTarget(LPCTSTR lpszFilename);
void AutoSizeColumns(CListCtrl *pListCtrl);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D3E3BA89_EEFA_11D5_AB1C_00D0B70C3D79__INCLUDED_)
