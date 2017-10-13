
// stdafx.cpp : source file that includes just the standard includes
//	FTPServer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


/********************************************************************/
/*																	*/
/* Function name : BrowseForFolder									*/		
/* Description   : Browse for folder using SHBrowseForFolder.		*/
/*																	*/
/********************************************************************/
CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags)
{
	// We're going to use the shell to display a 
	// "Choose Directory" dialog box for the user.
	CString strResult = "";
  
	LPMALLOC lpMalloc;
	
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
	{
		// failed to get allocator  
		return strResult; 
	}

	char szBuffer[_MAX_PATH];
	char szDisplayName[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hWnd;
	// set root at Desktop
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = lpszTitle;
	browseInfo.ulFlags = nFlags;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0;
	
	LPITEMIDLIST lpItemIDList;

	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path the user chose.
			if (szBuffer[0] == '\0')
			{
				// SHGetPathFromIDList failed, or SHBrowseForFolder failed.
				AfxMessageBox("Failed to get directory", MB_ICONSTOP|MB_OK);
				return strResult;
			}
     
			// We have a path in szBuffer!
			strResult = szBuffer;
			return strResult;
		}
		else
		{
			// The thing referred to by lpItemIDList 
			// might not have been a file system object.
			// For whatever reason, SHGetPathFromIDList didn't work!
			AfxMessageBox("Failed to get directory", MB_ICONSTOP|MB_OK);
			return strResult; // strResult is empty 
		}
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();      
	}
	return strResult;
}


void DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}


/********************************************************************/
/*																	*/
/* Function name : GetAppDir										*/		
/* Description   : Get application directory.						*/
/*																	*/
/********************************************************************/
void GetAppDir(CString& strAppDir)
{
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	// Get application's full path.
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// Break full path into seperate components.
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	// Store application's drive and path
	strAppDir.Format(_T("%s%s"), szDrive, szDir);
}


PFNSHGETFOLDERPATHA GetFuncPtr_SHGetFolderPathA()
{
	static HMODULE hMod = NULL;
	PFNSHGETFOLDERPATHA pSHGetFolderPath = NULL;

	// Load SHFolder.dll only once
	if (!hMod)
		hMod = LoadLibrary("SHFolder.dll");

	// Obtain a pointer to the SHGetFolderPathA function
	if (hMod)
		pSHGetFolderPath = (PFNSHGETFOLDERPATHA)GetProcAddress(hMod, "SHGetFolderPathA");

	return pSHGetFolderPath;
} 


/********************************************************************/
/*																	*/
/* Function name : CreateStartMenuShortcut							*/		
/* Description   : Create a shortcut using the IShellLink interface.*/
/*																	*/
/********************************************************************/
HRESULT CreateStartMenuShortcut(LPSTR lpszShortcutFile, LPSTR lpszDescription, LPTSTR lpszRelativeFolder)
{
	HRESULT hr;
	PFNSHGETFOLDERPATHA pSHGetFolderPath = NULL;
	TCHAR lpszLink[MAX_PATH];
	BOOL bFound = FALSE;

	pSHGetFolderPath = GetFuncPtr_SHGetFolderPathA();

	// Find the current user's Start Menu Programs folder
	if (pSHGetFolderPath)
		bFound = SUCCEEDED(pSHGetFolderPath(NULL, CSIDL_PROGRAMS, NULL, 0, lpszLink));

	if (bFound)
	{
		// Proceed to create the shortcut
		IShellLink *pIShellLink = NULL;
		IPersistFile *ppf = NULL;
		WCHAR pLinkUnicode[MAX_PATH];

		CoInitialize(NULL);

		// Get a pointer to the IShellLink interface.
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (void **)&pIShellLink);

		if (SUCCEEDED(hr))
		{
			// Get a pointer to IPersistFile interface for saving shortcut
			hr = pIShellLink->QueryInterface(IID_IPersistFile, (void **)&ppf);

			if (SUCCEEDED(hr))
			{   
	            hr = pIShellLink->SetPath(lpszShortcutFile);
		        hr = pIShellLink->SetDescription(lpszDescription);

				if (SUCCEEDED(hr))
				{
					// Add the target folder to the Start Menu Programs path
					lstrcat(lpszLink, "\\");
					lstrcat(lpszLink, lpszRelativeFolder);
					lstrcat(lpszLink, "\\");
	
					// Create the directory if it does not exist
					CreateDirectory(lpszLink,NULL);

					// Add the file name for the shortcut
					lstrcat(lpszLink, lpszDescription);
					lstrcat(lpszLink, ".lnk");

					// Convert string to Unicode, and call IPersistFile::Save()
					MultiByteToWideChar(CP_ACP, 0, lpszLink, -1, pLinkUnicode, MAX_PATH);
					hr = ppf->Save(pLinkUnicode, TRUE);
				}
				ppf->Release();
			}
			pIShellLink->Release();
		}
		CoUninitialize();
	}
	return hr;
} 


/********************************************************************/
/*																	*/
/* Function name : RemoveStartMenuShortcut							*/		
/* Description   : Create a shortcut using the IShellLink interface.*/
/*																	*/
/********************************************************************/
void RemoveStartMenuShortcut(LPSTR lpszDescription, LPTSTR lpszRelativeFolder)
{
	PFNSHGETFOLDERPATHA pSHGetFolderPath = NULL;
	TCHAR lpszLink[MAX_PATH];
	BOOL bFound = FALSE;

	pSHGetFolderPath = GetFuncPtr_SHGetFolderPathA();

	// Find the current user's Start Menu Programs folder
	if (pSHGetFolderPath)
		bFound = SUCCEEDED(pSHGetFolderPath(NULL, CSIDL_PROGRAMS, NULL, 0, lpszLink));

	if (bFound)
	{
		// Add the target folder to the Start Menu Programs path
		lstrcat(lpszLink, "\\");
		lstrcat(lpszLink, lpszRelativeFolder);
		lstrcat(lpszLink, "\\");

		// Add the file name for the shortcut
		lstrcat(lpszLink, lpszDescription);
		lstrcat(lpszLink, ".lnk");

		DeleteFile(lpszLink);
	}
} 


/********************************************************************/
/*																	*/
/* Function name : GetShortcutTarget								*/		
/* Description   : Get target filename from shortcut.				*/
/*																	*/
/********************************************************************/
CString GetShortcutTarget(LPCTSTR lpszFilename)
{
	CoInitialize(0);
	
	CString strResult;
	HRESULT hResult;
	IShellLink *psl;
	char szPath[MAX_PATH];
	WIN32_FIND_DATA findData;

	// Get a pointer to the IShellLink interface.
	hResult = CoCreateInstance (CLSID_ShellLink, NULL,	CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);
	if (SUCCEEDED(hResult))
	{
		IPersistFile *ppf;

		// Get a pointer to the IPersistFile interface.
		hResult = psl->QueryInterface (IID_IPersistFile, (void **)&ppf);

		if (SUCCEEDED(hResult))
		{
			WORD wsz [MAX_PATH]; // buffer for Unicode string

			// Ensure that the string consists of Unicode characters.
			MultiByteToWideChar (CP_ACP, 0, lpszFilename, -1, (LPWSTR)wsz, MAX_PATH);
			
			// Load the shortcut.
			hResult = ppf->Load ((LPCOLESTR)wsz, STGM_READ);
			
			if (SUCCEEDED (hResult))
			{
				// Resolve the shortcut.
				hResult = psl->Resolve (0, SLR_ANY_MATCH|SLR_NO_UI);
				if (SUCCEEDED (hResult))
				{
					lstrcpy (szPath, lpszFilename);
					// Get the path to the shortcut target.
					hResult = psl->GetPath(szPath, MAX_PATH, (WIN32_FIND_DATA *)&findData, 0); 
					strResult = szPath;
				}
			}
			// Release the pointer to IPersistFile.
			ppf->Release ();
		}
		// Release the pointer to IShellLink.
		psl->Release ();
	}
	CoUninitialize();
	
	if (SUCCEEDED(hResult))
		return strResult;
	else
		return "";
}


// Pump messages while waiting for event
BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout)
{   
	DWORD dwRet;

	while (1)
	{
		// wait for event or message, if it's a message, process it and return to waiting state
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, nTimeout, QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
		{
			TRACE0("WaitWithMessageLoop() event triggered.\n");
			return TRUE;      
		}   
		else
		if (dwRet == WAIT_OBJECT_0 + 1)
		{
			// process window messages
			AfxGetApp()->PumpMessage();
		}  
		else
		if (dwRet == WAIT_TIMEOUT)
		{
			// timed out !
			return FALSE;
		}
		else
		{
			// WAIT_ABANDONED_0 ...
			return TRUE;
		}
	}
}



/********************************************************************/
/*																	*/
/* Function name : AutoSizeColumns									*/
/* Description   : 													*/
/*																	*/
/********************************************************************/
void AutoSizeColumns(CListCtrl *pListCtrl) 
{
	// Call this after your the control is filled
	pListCtrl->SetRedraw(FALSE);
	int mincol = 0;
    int maxcol = pListCtrl->GetHeaderCtrl()->GetItemCount()-1;
    for (int col = mincol; col <= maxcol; col++) 
	{
		pListCtrl->SetColumnWidth(col, LVSCW_AUTOSIZE);
        int wc1 = pListCtrl->GetColumnWidth(col);
        pListCtrl->SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
        int wc2 = pListCtrl->GetColumnWidth(col);
        // 10 is minumim column width
		int wc = max(10, max(wc1,wc2));
        pListCtrl->SetColumnWidth(col,wc);
     }
     pListCtrl->SetRedraw(TRUE);
}
