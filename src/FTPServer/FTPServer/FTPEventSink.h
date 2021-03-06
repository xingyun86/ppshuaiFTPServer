
// FTPEventSink.h: interface for the CFTPEventSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPEVENTSINK_H__D8B6FF57_D431_11D5_AB13_00D0B70C3D79__INCLUDED_)
#define AFX_FTPEVENTSINK_H__D8B6FF57_D431_11D5_AB13_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFTPEventSink  
{
public:
	CFTPEventSink();
	virtual ~CFTPEventSink();

	virtual void OnFTPUserConnected(DWORD nThreadID, LPCTSTR lpszUser, LPCSTR lpszAddress) {};
	virtual void OnFTPUserDisconnected(DWORD nThreadID, LPCTSTR lpszUser) {};
	virtual void OnFTPStatusChange(int nType, LPCTSTR lpszText) {};
	virtual void OnFTPReceivedBytesChange(int nBytes) {};
	virtual void OnFTPSentBytesChange(int nBytes) {};
	virtual void OnFTPStatisticChange(int nType, int nValue) {};
};

#endif // !defined(AFX_FTPEVENTSINK_H__D8B6FF57_D431_11D5_AB13_00D0B70C3D79__INCLUDED_)
