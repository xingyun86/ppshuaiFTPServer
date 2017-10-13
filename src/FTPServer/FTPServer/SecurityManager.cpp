
/****************************************************************/
/*																*/
/*  SecurityManager.cpp											*/
/*																*/
/*  Implementation of the CSecurityManager class.				*/
/*																*/
/*  Programmed by xingyun86							*/
/*  Copyright @2017					*/
/*	http://www.ppsbbs.tech								*/
/*																*/
/*  Last updated: 10 july 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "ftpserver.h"
#include "SecurityManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSecurityManager::CSecurityManager()
{
	GetAppDir(m_strFilename);
	m_strFilename += "security.dat";
}

CSecurityManager::~CSecurityManager()
{

}


/********************************************************************/
/*																	*/
/* Function name : Serialize										*/
/* Description   : Call this function to store/load security data	*/
/*																	*/
/********************************************************************/
BOOL CSecurityManager::Serialize(BOOL bStoring)
{
	static const TCHAR* lpszSignature = _T("Pablo Software Solutions - Security");

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
				m_BlockedList.Serialize(ar);
				m_NonBlockedList.Serialize(ar);

				ar.Flush();
			}
			else
			{
				// load signature
				ar >> str;
				// if this the file we are looking for ?
				if (str.Compare(lpszSignature) == 0)
				{
					m_BlockedList.Serialize(ar);
					m_NonBlockedList.Serialize(ar);
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
/* Function name : GetBlockedList									*/
/* Description   : Get list of blocked IP addresses					*/
/*																	*/
/********************************************************************/
void CSecurityManager::GetBlockedList(CStringArray &strArray)
{
	m_CriticalSection.Lock();
	strArray.RemoveAll();
	strArray.Copy(m_BlockedList);
	m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : GetNonBlockedList								*/
/* Description   : Get list of IP addresses	that are not blocked.	*/
/*																	*/
/********************************************************************/
void CSecurityManager::GetNonBlockedList(CStringArray &strArray)
{
	m_CriticalSection.Lock();
	strArray.RemoveAll();
	strArray.Copy(m_NonBlockedList);
	m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : UpdateBlockedList								*/
/* Description   : Update list of IP addresses that are blocked.	*/
/*																	*/
/********************************************************************/
void CSecurityManager::UpdateBlockedList(CStringArray &strArray)
{
	m_CriticalSection.Lock();
	m_BlockedList.RemoveAll();
	m_BlockedList.Copy(strArray);
	Serialize(TRUE);
	m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : UpdateNonBlockedList								*/
/* Description   : Update list of IP addresses that are not blocked.*/
/*																	*/
/********************************************************************/
void CSecurityManager::UpdateNonBlockedList(CStringArray &strArray)
{
	m_CriticalSection.Lock();
	m_NonBlockedList.RemoveAll();
	m_NonBlockedList.Copy(strArray);
	Serialize(TRUE);
	m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : IsIPAddressBlocked								*/
/* Description   : Check if specified IP is blocked.				*/
/*																	*/
/********************************************************************/
BOOL CSecurityManager::IsIPAddressBlocked(LPCTSTR lpszIPAddress)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_BlockedList.GetSize(); i++)
	{
		if (m_BlockedList[i].CompareNoCase(lpszIPAddress) == 0)
		{
			m_CriticalSection.Unlock();
			return TRUE;
		}
	}
	m_CriticalSection.Unlock();
	return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : IsIPAddressNonBlocked							*/
/* Description   : Check if specified IP is non-blocked.			*/
/*																	*/
/********************************************************************/
BOOL CSecurityManager::IsIPAddressNonBlocked(LPCTSTR lpszIPAddress)
{
	m_CriticalSection.Lock();
	for (int i=0; i<m_NonBlockedList.GetSize(); i++)
	{
		if (m_NonBlockedList[i].CompareNoCase(lpszIPAddress) == 0)
		{
			m_CriticalSection.Unlock();
			return TRUE;
		}
	}
	m_CriticalSection.Unlock();
	return FALSE;
}
