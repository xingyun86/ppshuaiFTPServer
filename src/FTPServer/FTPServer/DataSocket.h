
#if !defined(AFX_DATASOCKET_H__38ADA982_DD96_4607_B7D2_982011F162FE__INCLUDED_)
#define AFX_DATASOCKET_H__38ADA982_DD96_4607_B7D2_982011F162FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConnectSocket;

#define XFERMODE_IDLE	0
#define XFERMODE_LIST	1
#define XFERMODE_SEND	2
#define XFERMODE_RECEIVE 3
#define XFERMODE_ERROR	4

class CDataSocket : public CAsyncSocket
{
public:
	CDataSocket(CConnectSocket *pSocket, int nTransferType = 0);
	virtual ~CDataSocket();
public:
	void SetTransferType(int nType);
	void SetData(LPCTSTR lpszData);
	CFile m_File;
	int GetStatus();
	//{{AFX_VIRTUAL(CDataSocket)
	public:
	virtual void OnSend(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDataSocket)
	//}}AFX_MSG

protected:
	BOOL m_bPassiveMode;
	BOOL m_bConnected;
	int Receive();
	BOOL PrepareReceiveFile(LPCTSTR lpszFilename);
	BOOL PrepareSendFile(LPCTSTR lpszFilename);
	DWORD m_nTotalBytesTransfered;
	DWORD m_nTotalBytesReceive;
	DWORD m_nTotalBytesSend;
	int m_nTransferType;
	CString m_strData;
	int m_nStatus;
	CConnectSocket *m_pConnectSocket;
};


//{{AFX_INSERT_LOCATION}}

#endif // AFX_DATASOCKET_H__38ADA982_DD96_4607_B7D2_982011F162FE__INCLUDED_
