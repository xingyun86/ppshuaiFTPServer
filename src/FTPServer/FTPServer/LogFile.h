
#if !defined(AFX_LOGFILE_H__7B62ED00_C40F_11D2_8415_444553540000__INCLUDED_)
#define AFX_LOGFILE_H__7B62ED00_C40F_11D2_8415_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogFile.h : header file
//
//#include <windows.h>
//#include "stdio.h"
//#include <iomanip.h>

#ifndef _FSTREAM_

#include <fstream>
using namespace std;

#endif

/////////////////////////////////////////////////////////////////////////////
// CLogFile window

class CLogFile
{
public:
	void Date();
	void Clear();
	void SetLogLevel(int nLevel);
	int m_nLevel;
	int m_nCurrentLevel;
    char m_FileName[256];
	char m_szLogPath[MAX_PATH];
	void Time();
	CLogFile();
	void Open();
	void Open(LPCTSTR lpszFileName);
	void Close();

	~CLogFile();

	CLogFile& operator <<(long				lVal);
	CLogFile& operator <<(const		TCHAR*	str);
	CLogFile& operator <<(TCHAR				tch);
	CLogFile& operator <<(int				nVal);
	CLogFile& operator <<(unsigned	long	ulVal);
	CLogFile& operator <<(double			dVal);
	CLogFile& operator <<(unsigned	int		unVal);

	CLogFile& operator <<(CLogFile& (*_f)(CLogFile&));
	void  Write(TCHAR* formatString, ...);	
	
/*	friend CLogFile& setlevel(CLogFile& os, int level) 
	{
        m_nCurrentLevel = level;
        return os;
    }
*/

	friend CLogFile& error_lvl(CLogFile& os) 
	{
		os.m_nCurrentLevel = 1;
		return os;
	}

	friend CLogFile& warning_lvl(CLogFile& os) 
	{
		os.m_nCurrentLevel = 2;
		return os;
	}
	
	friend CLogFile& trace_lvl(CLogFile& os) 
	{
		os.m_nCurrentLevel = 3;
		return os;
	}

private:
	ofstream	*m_File;
	void		Output( const TCHAR* data);

    WORD m_OldDate;
};

// manipulator for writing time to logfile
CLogFile& time(CLogFile& os);

// manipulator for writing date to logfile
CLogFile& date(CLogFile& os);

// manipulator for writing end of line to logfile
CLogFile& endl(CLogFile& os);

// manipulator for settings log level to error
CLogFile& error_lvl(CLogFile& os);
// manipulator for settings log level to trace
CLogFile& trace_lvl(CLogFile& os);
// manipulator for settings log level to warning
CLogFile& warning_lvl(CLogFile& os);

#endif // !defined(AFX_LOGFILE_H__7B62ED00_C40F_11D2_8415_444553540000__INCLUDED_)
