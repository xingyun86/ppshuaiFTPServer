
// ConnectSocket.h : header file

#if !defined(AFX_CONNECTS_H__B7C54BD3_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
#define AFX_CONNECTS_H__B7C54BD3_A555_11D0_8996_00AA00B92B2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDataSocket;

class CConnectSocket : public CSocket
{
	enum // Token ID's
	{
		TOK_USER, TOK_PASS, TOK_CWD, TOK_PWD, 
		TOK_PORT, TOK_PASV, TOK_TYPE, TOK_LIST,
		TOK_REST, TOK_CDUP, TOK_RETR, TOK_STOR,
		TOK_SIZE, TOK_DELE, TOK_RMD, TOK_MKD,
		TOK_RNFR, TOK_RNTO, TOK_ABOR, TOK_SYST, TOK_NOOP, 
		TOK_BYE, TOK_QUIT, TOK_ERROR,
	};

public:
	struct CTransferStatus
	{
		int m_bPassiveMode;
		int m_nRest;
		int m_nRemotePort;
		CString m_strRemoteHost;
		CDataSocket *m_pDataSocket;
	} m_TransferStatus;

	struct CFTPCommand
	{
		int m_nTokenID;
		char *m_pszName;
		BOOL m_bHasArguments;
	};

// Attributes
public:
	BOOL HasConnectionDropped(void);
	BOOL SendResponse(CString strData);
	void FireStatusMessage(LPCTSTR lpszStatus, int nType);
	BOOL GetRxCommand(CString &command, CString &args);
	void DestroyDataSocket();
// Operations
public:
	CConnectSocket();
	virtual ~CConnectSocket();

	void ParseCommand();

// Overrides
public:
	CWinThread* m_pThread;

	BOOL m_bLoggedon;
	CString m_strUserName;
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CConnectSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	void Shutdown(int nReason);
	CStringList m_strCommands;
	void GetRxLine();
	BOOL m_bRenameFile;
	CString m_strRenameFile;
	CString m_RxBuffer;
	CString m_strCurrentDir;

	char *GetResponseMessage(int nResponse);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTS_H__B7C54BD3_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
