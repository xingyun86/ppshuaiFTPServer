
/****************************************************************/
/*																*/
/*  UserManager.cpp												*/
/*																*/
/*  Implementation of the CUserManager class.					*/
/*																*/
/*  Programmed by xingyun86							*/
/*  Copyright @2017					*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "FTPserverApp.h"
#include "UserManager.h"

extern CFTPServerApp theApp;

IMPLEMENT_SERIAL(CDirectory, CObject, 1)

CDirectory::CDirectory()
{
}

CDirectory::~CDirectory()
{
}

void CDirectory::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 'store' data
		ar << m_strDir;
		ar << m_strAlias;
		ar << m_bAllowDownload;
		ar << m_bAllowUpload;
		ar << m_bAllowRename;
		ar << m_bAllowDelete;
		ar << m_bAllowCreateDirectory;
		ar << m_bIsHomeDir;
	}
	else
	{
		// 'load' data
		ar >> m_strDir;
		ar >> m_strAlias;
		ar >> m_bAllowDownload;
		ar >> m_bAllowUpload;
		ar >> m_bAllowRename;
		ar >> m_bAllowDelete;
		ar >> m_bAllowCreateDirectory;
		ar >> m_bIsHomeDir;
	}
}


template <> void AFXAPI SerializeElements <CDirectory> (CArchive& ar, CDirectory* pNewDirectories, int nCount)
{
    for (int i = 0; i < nCount; i++, pNewDirectories++)
    {
        // Serialize each CDirectory object
        pNewDirectories->Serialize(ar);
    }
}


/* Copy-constructor */
CDirectory::CDirectory(const CDirectory &dir)
{
	m_strDir = dir.m_strDir;
	m_strAlias = dir.m_strAlias;
	m_bAllowDownload = dir.m_bAllowDownload;
	m_bAllowUpload = dir.m_bAllowUpload;
	m_bAllowRename = dir.m_bAllowRename;
	m_bAllowDelete = dir.m_bAllowDelete;
	m_bAllowCreateDirectory = dir.m_bAllowCreateDirectory;
	m_bIsHomeDir = dir.m_bIsHomeDir;
}

/* = operator definition */
CDirectory& CDirectory::operator=(const CDirectory &dir)
{
	if (&dir != this)
	{
		m_strDir = dir.m_strDir;
		m_strAlias = dir.m_strAlias;
		m_bAllowDownload = dir.m_bAllowDownload;
		m_bAllowUpload = dir.m_bAllowUpload;
		m_bAllowRename = dir.m_bAllowRename;
		m_bAllowDelete = dir.m_bAllowDelete;
		m_bAllowCreateDirectory = dir.m_bAllowCreateDirectory;
		m_bIsHomeDir = dir.m_bIsHomeDir;
	}
	return *this;
}


IMPLEMENT_SERIAL(CUser, CObject, 1)

CUser::CUser()
{
	m_bAccountDisabled = FALSE;
}

CUser::~CUser()
{
}

void CUser::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 'store' data
		ar << m_strName;
		ar << m_strPassword;
		ar << m_bAccountDisabled;
	}
	else
	{
		// 'load' data
		ar >> m_strName;
		ar >> m_strPassword;
		ar >> m_bAccountDisabled;
	}
	// serialize directories
	m_DirectoryArray.Serialize(ar);
}


/* Copy-constructor */
CUser::CUser(const CUser &user)
{
	m_strName = user.m_strName;
	m_strPassword = user.m_strPassword;
	m_bAccountDisabled = user.m_bAccountDisabled;
	for (int i=0; i < user.m_DirectoryArray.GetSize(); i++)
		m_DirectoryArray.Add((CDirectory &)user.m_DirectoryArray[i]);
}

/* = operator definition */
CUser& CUser::operator=(const CUser &user)
{
	if (&user != this)
	{
		m_strName = user.m_strName;
		m_strPassword = user.m_strPassword;
		m_bAccountDisabled = user.m_bAccountDisabled;
		for (int i=0; i < user.m_DirectoryArray.GetSize(); i++)
			m_DirectoryArray.Add((CDirectory &)user.m_DirectoryArray[i]);
	}
	return *this;
}



CUserManager::CUserManager()
{
	GetAppDir(m_strFilename);
	m_strFilename += "users.dat";
}

CUserManager::~CUserManager()
{

}


/********************************************************************/
/*																	*/
/* Function name : Serialize										*/
/* Description   : Call this function to store/load the user data	*/
/*																	*/
/********************************************************************/
BOOL CUserManager::Serialize(BOOL bStoring)
{
	static const TCHAR* lpszSignature = _T("Pablo Software Solutions - StoreObject");

	CFile file;

	if (file.Open(m_strFilename, bStoring ? CFile::modeWrite|CFile::modeCreate : CFile::modeRead))
	{
		TRY
		{
			CString str; 
			CArchive ar(&file, bStoring ? CArchive::store : CArchive::load);
			
			if (bStoring)
			{
				// save signature
				ar << CString(lpszSignature);

				// Save the changed user details
				for (int i=0; i < m_UserArray.GetSize(); i++)
				{
					m_UserArray[i].Serialize(ar);
				}

				ar.Flush();
			}
			else
			{
				// load signature
				ar >> str;
				// if this the file we are looking for ?
				if (str.Compare(lpszSignature) == 0)
				{
					int nCount=0;

					while(!ar.IsBufferEmpty())
					{
						CUser user;

						// get user data
						user.Serialize(ar);
						
						// add user to array
						m_UserArray.Add(user);
					}
				}
			}
			ar.Close();
			file.Close();
		}
		CATCH_ALL(e)
		{
			// catch all exceptions that might happen ...
			return FALSE;
		}
		END_CATCH_ALL
	}
	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : CheckUser										*/
/* Description   : Lookup a specific user and return it's data		*/
/*																	*/
/********************************************************************/
BOOL CUserManager::CheckUser(LPCTSTR lpszUser, LPCTSTR lpszPassword, CUser &user)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_UserArray.GetSize(); i++)
	{
		// check username case-insensitive
		if (!m_UserArray[i].m_strName.CompareNoCase(lpszUser))
		{
			if ((!m_UserArray[i].m_strPassword.Compare(lpszPassword) || m_UserArray[i].m_strPassword.IsEmpty()) && !m_UserArray[i].m_bAccountDisabled)
			{
				user = m_UserArray[i];
				m_CriticalSection.Unlock();
				return TRUE;
			}
			else
			{
				m_CriticalSection.Unlock();
				return FALSE;
			}
		}
	}
	m_CriticalSection.Unlock();
	return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : ConvertPathToLocal								*/
/* Description   : Convert relative path to local path				*/
/*																	*/
/********************************************************************/
int CUserManager::ConvertPathToLocal(LPCTSTR lpszDirectory, LPCTSTR lpszUser, CDirectory &directory, BOOL &bIsRootDir)
{
	CUser user;
	
	if (!GetUser(lpszUser, user))
	{
		// user not valid
		return 1;
	}

	CStringList partList;
	CString strSub;
	int i = 0;
	int nCount=0;

	// split path in parts
	while(AfxExtractSubString(strSub, lpszDirectory, nCount++, '/'))
	{
		if (!strSub.IsEmpty())
			partList.AddTail(strSub);
	}
	
	// get home directory
	for (i=0; i<user.m_DirectoryArray.GetSize(); i++)
	{
		if (user.m_DirectoryArray[i].m_bIsHomeDir)
			break;
	}

	if (i == user.m_DirectoryArray.GetSize())
	{
		// no home directory found
		return 1;
	}
	
	CString strLocalPath;
	CString strHomeDir = user.m_DirectoryArray[i].m_strDir;

	while(!partList.IsEmpty())
	{
		CString strPart = partList.GetHead();
		partList.RemoveHead();

		// does directory exist ?
		if (strPart != ".." && FileExists(strHomeDir + "\\" + strPart, TRUE))
		{
			strHomeDir += "\\" + strPart;
		}
		else
		// does file exist ?
		if (FileExists(strHomeDir + "\\" + strPart, FALSE))
		{
			strHomeDir += "\\" + strPart;
		}
		else
		{
			BOOL bFound = FALSE;
			// maybe it's a virtual directory
			for (int i=0; i<user.m_DirectoryArray.GetSize(); i++)
			{
				if ((!user.m_DirectoryArray[i].m_bIsHomeDir) && (user.m_DirectoryArray[i].m_strAlias.CompareNoCase(strPart) == 0))
				{
					bFound = TRUE;
					strHomeDir = user.m_DirectoryArray[i].m_strDir; 
					break;
				}
			}
			if (!bFound)
			{
				// directory not found
				return 2;
			}
		}
	}
	strLocalPath = strHomeDir;

	// Check user rights for this directory
	bIsRootDir = FALSE;

	while(strHomeDir != "")
	{
		int i = 0;
		for (i=0; i<user.m_DirectoryArray.GetSize(); i++)
		{
			CString strPath1 = strHomeDir;
			strPath1.TrimRight("\\");
			strPath1.MakeLower();
			CString strPath2 = user.m_DirectoryArray[i].m_strDir;
			strPath2.TrimRight("\\");
			strPath2.MakeLower();

			if (strPath1 == strPath2)
			{
				if (strHomeDir == strLocalPath)
				{
					bIsRootDir = TRUE;
				}
				break;
			}
		}

		if (i == user.m_DirectoryArray.GetSize())
		{
			int nPos = strHomeDir.ReverseFind('\\');
			if (nPos != -1)
			{
				strHomeDir = strHomeDir.Left(nPos);
			}
			else
			{
				// not a valid home directory
				return 1;
			}
			continue;
		}
		directory = user.m_DirectoryArray[i];
		strLocalPath.TrimRight('\\');
		directory.m_strDir = strLocalPath;
		if (!bIsRootDir)
			directory.m_bIsHomeDir = FALSE;
		// successfully converted directory
		return 0;
		
	}
	// error converting directory
	return 2;
}


/********************************************************************/
/*																	*/
/* Function name : ChangeDirectory									*/
/* Description   : Change to specified directory					*/
/*																	*/
/********************************************************************/
int CUserManager::ChangeDirectory(LPCTSTR lpszUser, CString &strCurrentdir, CString &strChangeTo)
{
	// make unix style
	strChangeTo.Replace("\\","/");
	while(strChangeTo.Replace("//","/"));
	strChangeTo.TrimRight("/");

	// now looks something like this: 
	// ""				= root
	// "/mydir/apps"	= absolute path
	// "mydir/apps"		= relative path

	if (strChangeTo == "")
	{
		// goto root
		strChangeTo = "/";
	}
	else
	{
		// first character '/' ?
		if (strChangeTo.Left(1) != "/")
		{ 
			// client specified a path relative to their current path
			strCurrentdir.TrimRight("/");
			strChangeTo = strCurrentdir + "/" + strChangeTo;
		}
	}
	
	// goto parent directory
	if (strChangeTo.Right(2) == "..")
	{
		strChangeTo.TrimRight("..");
		strChangeTo.TrimRight("/");
		int nPos = strChangeTo.ReverseFind('/');
		if (nPos != -1)
			strChangeTo = strChangeTo.Left(nPos);

		if (strChangeTo == "")
		{
			// goto root
			strChangeTo = "/";
		}
	}

	// get local path
	CDirectory directory;
	BOOL bIsRootDir;
	int nResult = ConvertPathToLocal(strChangeTo, lpszUser, directory, bIsRootDir);
	
	if (nResult)
		return nResult;

	if (!directory.m_bAllowDownload)
		return 1;
	
	// is it really a directory ?
	if (!FileExists(directory.m_strDir, TRUE))
		return 2;

	// everything went successfully
	strCurrentdir = strChangeTo; 
	return 0;
}


/********************************************************************/
/*																	*/
/* Function name : GetDirectoryList									*/
/* Description   : Get directory listing for specfied directory		*/
/*																	*/
/********************************************************************/
int CUserManager::GetDirectoryList(LPCTSTR lpszUser, LPCTSTR lpszDirectory, CString &strResult)
{
	CUser user;
	
	// clear list
	strResult = "";

	if (!GetUser(lpszUser, user))
		return 1;

	CDirectory directory;
	BOOL bIsRootDir;
	int nResult = ConvertPathToLocal(lpszDirectory, lpszUser, directory, bIsRootDir);
	if (nResult)
		return nResult;

	CFileFind find;
	BOOL bFound = FALSE;

	// quick way to check if it's a directory
	if ((GetFileAttributes(directory.m_strDir) & FILE_ATTRIBUTE_DIRECTORY) == 
		FILE_ATTRIBUTE_DIRECTORY)
	{
		bFound = find.FindFile(directory.m_strDir + "\\*.*");
	}
	else
	{
		// it's a file
		bFound = find.FindFile(directory.m_strDir);
	}

	while (bFound)
	{
		bFound = find.FindNextFile();
		
		// skip "." and ".." 
		if (find.IsDots())
			continue;

		// permissions
		if (find.IsDirectory())
			strResult += "drwx------";
		else
			strResult += "-rwx------";

		// groups
		strResult += " 1 user group ";
		// file size
		CString strLength;
		strLength.Format("%d", find.GetLength());
		CString strFiller = "              ";
		strResult += strFiller.Left(strFiller.GetLength() - strLength.GetLength());
		strResult += strLength;
		// file date
		strResult += GetFileDate(find);
		// file name
		strResult += find.GetFileName();
		// end of line
		strResult += "\r\n";
	}
	
	// if it's the root dir also show virtual directories
	if (directory.m_bIsHomeDir)
	{
		for (int i=0; i<user.m_DirectoryArray.GetSize(); i++)
		{
			if (!user.m_DirectoryArray.GetAt(i).m_bIsHomeDir && user.m_DirectoryArray.GetAt(i).m_strAlias != "")
			{
				bFound = find.FindFile(user.m_DirectoryArray.GetAt(i).m_strDir);
				if (bFound)
				{
					find.FindNextFile();

					strResult += "drwx------";
					// groups
					strResult += " 1 user group ";
					strResult += "             0";
					// file date
					strResult += GetFileDate(find);
					// file name
					strResult += user.m_DirectoryArray.GetAt(i).m_strAlias;
					// end of line
					strResult += "\r\n"; 
				}
			}
		}
	}

	return 0;
}


/********************************************************************/
/*																	*/
/* Function name : FileExists										*/
/* Description   : Check if file or directory exists				*/
/*																	*/
/********************************************************************/
BOOL CUserManager::FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck)
{
	// A quick'n'easy way to see if a file exists.
	DWORD dwAttributes = GetFileAttributes(lpszFileName);
    if (dwAttributes == 0xFFFFFFFF)
        return FALSE;

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		if (bIsDirCheck)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (!bIsDirCheck)
			return TRUE;
		else
			return FALSE;
	}
/*
    HANDLE hFindFile;
    WIN32_FIND_DATA findData;

	if (strFileName.Right(1) == "\\")
	{
		strFileName += "*.*";
	}

    hFindFile = FindFirstFile(strFileName, &findData);
    if (hFindFile == INVALID_HANDLE_VALUE)
	{
		FindClose(hFindFile);
		return FALSE;
	}
     
    FindClose(hFindFile);
	return TRUE; */
}


/********************************************************************/
/*																	*/
/* Function name : GetFileName										*/
/* Description   : Get file name									*/
/*																	*/
/********************************************************************/
int CUserManager::GetFileName(LPCTSTR lpszUser, CString strFilename, CString strCurrentdir, int nOption, CString &strResult)
{
	// make unix style
	strFilename.Replace("\\", "/");
	while(strFilename.Replace("//", "/"));
	strFilename.TrimRight("/");
	
	if (strFilename == "")
	{
		// no file name
		return 2;
	}

	// append filename to directory
	CString strDirectory = strCurrentdir;

	// client has specified complete path 
	int nPos = strFilename.ReverseFind('/');
	if (nPos != -1)
	{
		strDirectory = strFilename.Left(nPos);
		if (strDirectory == "")
			strDirectory = "/";
		strFilename = strFilename.Mid(nPos+1);
	}

	// get local path
	CDirectory directory;
	BOOL bIsRootDir;
	int nResult = ConvertPathToLocal(strDirectory, lpszUser, directory, bIsRootDir);

	// we have a complete path !
	strResult = directory.m_strDir + "\\" + strFilename;

	if (nResult)
		return nResult;

	if ((nOption != FTP_UPLOAD) && !FileExists(strResult, FALSE))
	{
		// file does not exist
		return 2;
	}

	// check file access rights
	if (((!directory.m_bAllowDownload) && (nOption == FTP_DOWNLOAD)) ||
		((!directory.m_bAllowDelete) && (nOption == FTP_DELETE)) ||
		((!directory.m_bAllowUpload) && (nOption == FTP_UPLOAD)) ||
		((!directory.m_bAllowRename) && (nOption == FTP_RENAME)))
	{
		return 1;
	}
	
	// return relative path
	if (nOption == FTP_LIST)
	{
		strResult = strCurrentdir + "/" + strFilename;
	}
	return 0;
}


/********************************************************************/
/*																	*/
/* Function name : GetDirectory										*/
/* Description   :													*/
/*																	*/
/********************************************************************/
int CUserManager::GetDirectory(LPCTSTR lpszUser, CString strDirectory, CString strCurrentdir, int nOption, CString &strResult)
{
	// make unix compatible
	strDirectory.Replace("\\","/");
	while(strDirectory.Replace("//","/"));
	strDirectory.TrimRight("/");
	
	if (strDirectory == "")
	{
		// no directory
		return 2;
	}
	else
	{
		// first character '/' ?
		if (strDirectory.Left(1) != "/")
		{ 
			// client specified a path relative to their current path
			strCurrentdir.TrimRight("/");
			strDirectory = strCurrentdir + "/" + strDirectory;
		}
	}

	// split part into 2 parts
	int nPos = strDirectory.ReverseFind('/');
	if (nPos == -1)
		return 2;

	// get left part of directory
	CString strNode = strDirectory.Left(nPos);
	// root ?
	if (strNode == "")
		strNode = "/";
	// get right part of directory
	strDirectory = strDirectory.Mid(nPos+1);

	CDirectory directory;
	BOOL bIsRootDir;

	int nResult;
	do
	{
		// does parent directory exist ?
		nResult = ConvertPathToLocal(strNode, lpszUser, directory, bIsRootDir);
		if ((nResult == 2) && (nOption == FTP_CREATE_DIR)) 
		{ 
			// directory could not be found, maybe one level higher
			int nPos = strNode.ReverseFind('/');
			// no more levels
			if (nPos == -1) 
				return nResult;
			strDirectory = strNode.Mid(nPos+1) + "/" + strDirectory;
			strNode = strNode.Left(nPos);
			continue;
		}
		else 
		if (nResult) 
		{
			// not enough rights
			return nResult;
		}
		
		strNode = directory.m_strDir;
		
		// check directory access rights
		if ((!directory.m_bAllowDelete && nOption == FTP_DELETE) ||
			(!directory.m_bAllowRename && nOption == FTP_RENAME) ||
			(!directory.m_bAllowCreateDirectory && nOption == FTP_CREATE_DIR))
		{
			return 1;
		}
		break;
	} while (strNode != "/");
	
	strDirectory.Replace("/","\\");
	strResult = strNode + "\\" + strDirectory;
		
	// check if directory exists
	if (!FileExists(strResult))
		return 2;

	// function successfull
	return nResult;
}


/********************************************************************/
/*																	*/
/* Function name : GetUser											*/
/* Description   : Get user object for specified username			*/
/*																	*/
/********************************************************************/
BOOL CUserManager::GetUser(LPCTSTR lpszUser, CUser &user)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_UserArray.GetSize(); i++)
	{
		if (!m_UserArray[i].m_strName.CompareNoCase(lpszUser))
		{
			user = m_UserArray[i];
			m_CriticalSection.Unlock();
			return TRUE;
		}
	}
	m_CriticalSection.Unlock();
	return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : GetUserList										*/
/* Description   : Make copy of user array							*/
/*																	*/
/********************************************************************/
void CUserManager::GetUserList(CArray<CUser, CUser&>&array)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_UserArray.GetSize();i++)
	{
		array.Add(m_UserArray[i]);
	}
	m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : UpdateUserList									*/
/* Description   : Update user array								*/
/*																	*/
/********************************************************************/
void CUserManager::UpdateUserList(CArray<CUser, CUser&>&array)
{
	m_CriticalSection.Lock();
	m_UserArray.RemoveAll();
	for (int i=0; i<array.GetSize();i++)
	{
		m_UserArray.Add(array[i]);
	}
	m_CriticalSection.Unlock();
	Serialize(TRUE);
}


/********************************************************************/
/*																	*/
/* Function name : GetFileDate										*/
/* Description   : return file date in unix style					*/
/*																	*/
/********************************************************************/
CString CUserManager::GetFileDate(CFileFind &find)
{
	CString strResult;

	CTime time = CTime::GetCurrentTime();

	find.GetLastWriteTime(time);

	CTimeSpan timeSpan = CTime::GetCurrentTime() - time;

	if (timeSpan.GetDays() > 356)
	{
		strResult = time.Format(" %b %d %Y ");
	}
	else
	{
		strResult.Format(" %s %02d:%02d ", time.Format("%b %d"), time.GetHour(), time.GetMinute());
	}

	return strResult;
}
