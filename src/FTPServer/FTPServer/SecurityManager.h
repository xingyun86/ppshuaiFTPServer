
// SecurityManager.h: interface for the CSecurityManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECURITYMANAGER_H__1236B96A_01BF_4866_9297_2A1D4B38C9D2__INCLUDED_)
#define AFX_SECURITYMANAGER_H__1236B96A_01BF_4866_9297_2A1D4B38C9D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSecurityManager  
{
public:
	BOOL IsIPAddressBlocked(LPCTSTR lpszIPAddress);
	BOOL IsIPAddressNonBlocked(LPCTSTR lpszIPAddress);
	void UpdateBlockedList(CStringArray &strArray);
	void UpdateNonBlockedList(CStringArray &strArray);
	void GetNonBlockedList(CStringArray &strArray);
	void GetBlockedList(CStringArray &strArray);
	CSecurityManager();
	virtual ~CSecurityManager();

	BOOL Serialize(BOOL bStoring);
protected:
	CStringArray m_NonBlockedList;
	CStringArray m_BlockedList;
	CString m_strFilename;

	CCriticalSection m_CriticalSection;
};

#endif // !defined(AFX_SECURITYMANAGER_H__1236B96A_01BF_4866_9297_2A1D4B38C9D2__INCLUDED_)
