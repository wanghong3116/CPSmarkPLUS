
#include "stdafx.h"
#include "InformationTrace.h"
#include "PlatformHeader.h"

namespace MyPlatform
{
 
#define MAX_LOG_FILE							20
#define EXT_LOG								L".log"

	tstring CInformationTrace::m_BaseLogFileName = _T("");
	bool FileTimeCompW(wstring strElem1, wstring strElem2)
	{
		return _wtoi64(wcsrchr(strElem1.c_str(), L'\\') + 1) < _wtoi64(wcsrchr(strElem2.c_str(), L'\\') + 1);
	}

	CInformationTrace * CInformationTrace::m_pInforTrace = NULL;

	int CInformationTrace::IsInTestMode = 0;

	CInformationTrace* CInformationTrace::GetInformationTracePtr()
	{
		if (NULL == m_pInforTrace)
		{
			m_pInforTrace = new CInformationTrace();
		}

		return m_pInforTrace;
	}

	CInformationTrace::CInformationTrace()
	{
		m_pMutex = new CThreadMutex();

		memset(m_szLogFile, 0, sizeof(m_szLogFile));
		memset(m_szLogDir, 0, sizeof(m_szLogDir));
		wcscpy_s(m_szLogExt, _ARRAYSIZE(m_szLogExt), EXT_LOG);

		m_nLimitNum = 20;
		m_bOpenConsole = false;


		GetFullPathUnderExeW(MyPlatform::ResultsFolder.c_str(), m_szLogDir, _MAX_PATH);
	}

	CInformationTrace::~CInformationTrace()
	{
		if (NULL != m_pMutex)
		{
			delete m_pMutex;
		}

	}


	int CInformationTrace::resetLogFilePath()
	{
		SYSTEMTIME sysTime = { 0 };
		vector<wstring> vstrDirAllFiles;

		::GetLocalTime(&sysTime);
		CreateFullDirectoryW(m_szLogDir);
		tstring projectDir = m_szLogDir;
		projectDir.append(_T("\\")).append(m_BaseLogFileName.c_str());
		CreateFullDirectoryW(projectDir.c_str());
		swprintf_s(m_szLogFile, _ARRAYSIZE(m_szLogFile), L"%s\\%s\\%s%s", m_szLogDir,
			m_BaseLogFileName.c_str(), m_BaseLogFileName.c_str(), m_szLogExt);
	 
		/*
		if (m_nLimitNum <= 0)
		{
			return PLATFORM_FUNC_NOERROR;
		}
		m_nLimitNum -= 1;
		GetAllFilesUnderDirW(m_szLogDir, &vstrDirAllFiles);
		if (m_nLimitNum > (int)vstrDirAllFiles.size())
		{
			return PLATFORM_FUNC_NOERROR;
		}
		sort(vstrDirAllFiles.begin(), vstrDirAllFiles.end(), FileTimeCompW);
		int nDeleteNum = (int)vstrDirAllFiles.size() - m_nLimitNum - 1;
		for (int nIdx = 0; nIdx < nDeleteNum;  nIdx++)
		{
			if (wcsstr(vstrDirAllFiles[nIdx].c_str(), m_BaseLogFileName.c_str()) == NULL) 
				DeleteFileW(vstrDirAllFiles[nIdx].c_str());
		}
		*/
		return PLATFORM_FUNC_NOERROR;
	}

	int CInformationTrace::TraceA(int nLevel, char *szFormat, ...)
	{
		CInformationTrace *pInforTrace = GetInformationTracePtr();

		if (NULL == pInforTrace)
		{
			return -1;
		}

		va_list vl = NULL;
		va_start(vl, szFormat);

		pInforTrace->traceAArgPtr(nLevel, szFormat, vl);

		va_end(vl);

		return PLATFORM_FUNC_NOERROR;
	}

	int CInformationTrace::TraceW(int nLevel, wchar_t *szFormat, ...)
	{
		CInformationTrace *pInforTrace = GetInformationTracePtr();

		if (NULL == pInforTrace)
		{
			return -1;
		}

		va_list vl = NULL;
		va_start(vl, szFormat);

		pInforTrace->traceWArgPtr(nLevel, szFormat, vl);

		va_end(vl);

		return PLATFORM_FUNC_NOERROR;
	}

	int CInformationTrace::traceA(int nLevel, char *szFormat, ...)
	{
		va_list vl = NULL;
		va_start(vl, szFormat);

		traceAArgPtr(nLevel, szFormat, vl);

		va_end(vl);

		return PLATFORM_FUNC_NOERROR;
	}

	int CInformationTrace::traceW(int nLevel, wchar_t *szFormat, ...)
	{
		va_list vl = NULL;
		va_start(vl, szFormat);

		traceWArgPtr(nLevel, szFormat, vl);

		va_end(vl);

		return PLATFORM_FUNC_NOERROR;
	}

	int CInformationTrace::traceAArgPtr(int nLevel, char *szFormat, va_list argPtr)
	{
		int nLen = 0;
		char* szTrace = NULL;

		try
		{
			if ((nLen = _vscprintf(szFormat, argPtr)) <= 0)
			{
				throw - 1;
			}
			nLen += 200;
			szTrace = (char*)calloc(nLen, sizeof(char));
			if (NULL == szTrace)
			{
				throw - 2;
			}

			vsprintf_s(szTrace, nLen, szFormat, argPtr);
			combineMsg(nLevel, CharToWchar(szTrace).c_str());
			throw PLATFORM_FUNC_NOERROR;
		}
		catch (int nRet)
		{
			FREE(szTrace);
			return nRet;
		}
	}

	int CInformationTrace::traceWArgPtr(int nLevel, wchar_t *szFormat, va_list argPtr)
	{
		int nLen = 0;
		wchar_t* szTrace = NULL;

		try
		{
			if ((nLen = _vscwprintf(szFormat, argPtr)) <= 0)
			{
				throw - 1;
			}
			nLen += 200;
			szTrace = (wchar_t*)calloc(nLen, sizeof(wchar_t));
			if (NULL == szTrace)
			{
				throw - 2;
			}

			vswprintf_s(szTrace, nLen, szFormat, argPtr);
			combineMsg(nLevel, szTrace);
			throw PLATFORM_FUNC_NOERROR;
		}
		catch (int nRet)
		{
			FREE(szTrace);
			return nRet;
		}
	}

	int CInformationTrace::combineMsg(int nLevel, const wchar_t *szTrace)
	{
		wchar_t *szMsg = NULL;
		size_t nLen = 0;
		SYSTEMTIME sysTime = { 0 };

		try
		{
			nLen = wcslen(szTrace);
			nLen += 500;
			if (NULL == (szMsg = (wchar_t *)calloc(nLen, sizeof(wchar_t))))
			{
				throw - 1;
			}

			switch (nLevel)
			{
			case L0:
				swprintf_s(szMsg, nLen, L"%s", L"error: ");
				break;
			case L1:
				swprintf_s(szMsg, nLen, L"%s", L"warning: ");
				break;
			case L2:
			default:
				swprintf_s(szMsg, nLen, L"%s", L"info: ");
				break;
			}

			GetLocalTime(&sysTime);
			swprintf_s(wcsrchr(szMsg, '\0'), nLen - wcslen(szMsg), L"%04d-%02d-%02d %02d:%02d:%02d||  ",
				sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			wcscat_s(szMsg, nLen - wcslen(szMsg), szTrace);
			wcscat_s(szMsg, nLen - wcslen(szMsg), L"\r\n");

			writeMsg(szMsg);
			throw PLATFORM_FUNC_NOERROR;
		}
		catch (int nRet)
		{
			FREE(szMsg);
			return nRet;
		}

	}

	int CInformationTrace::writeMsg(const wchar_t *szMsg)
	{
		CMyMutexControl control(m_pMutex->GetAddress());

		SYSTEMTIME sysTime = { 0 };
		FILE* fp = NULL;
		int nFileSize = 0;

		GetFileSizeW(m_szLogFile, &nFileSize);
		if (nFileSize > 5000000)
		{
			memset(m_szLogFile, 0, sizeof(m_szLogFile));
			nFileSize = 0;
		}

		if (0 != _waccess(m_szLogFile, 0)
			|| 0 == nFileSize)
		{
			resetLogFilePath();
			wchar_t cHeader = 0xfeff;
			_wfopen_s(&fp, m_szLogFile, L"a+");
			if (NULL == fp)
			{
				return -1;
			}
			fwrite(&cHeader, sizeof(wchar_t), 1, fp);
			FCLOSE(fp);

			if (m_bOpenConsole)
			{
				AllocConsole();

				freopen_s(&fp, "CONOUT$", "w+t", stdout);
				freopen_s(&fp, "CONIN$", "r+t", stdin);
			}
		}

		_wfopen_s(&fp, m_szLogFile, L"ab+");
		if (NULL == fp)
		{
			return -2;
		}
		fwrite(szMsg, sizeof(wchar_t), wcslen(szMsg), fp);
		FCLOSE(fp);

		if (m_bOpenConsole)
		{
			_putws(szMsg);
		}

		return PLATFORM_FUNC_NOERROR;

	}
}

