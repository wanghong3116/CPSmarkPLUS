#include "stdafx.h"
#include "MarkObject.h"
#include "ZipMark.h"

using namespace MyPlatform;

CZipMark::CZipMark()
{
	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("7Z:压缩解压缩测试"));
	m_eMarkType = E_MARK_WINRAR;
	m_eMarkClassify = E_MARK_CLASSIFY_STORAGE;
}

CZipMark::~CZipMark()
{

}

int CZipMark::init()
{
	MyPlatform::GetFullPathUnderExe(_T("7-Zip\\7z.exe"), m_sz7zFP);
	if (0 != _taccess(m_sz7zFP, 0))
	{
		m_strLastErrMsg = _T("没有找到压缩程序");
		return -1;
	}

	MyPlatform::GetFullPathUnderExe(_T("7-Zip\\zipMeasure.zip"), m_szSourceFP);
	if (0 != _taccess(m_szSourceFP, 0))
	{
		m_strLastErrMsg = _T("没有找到被压缩的文件");
		return -2;
	}

	return BENCH_MARK_OK;
}

int CZipMark::run()
{
	int nRet = BENCH_MARK_OK;
	m_nRunTime = MyPlatform::GetTimeStampMillisecond();
	nRet = WzipDemo7Z();
	m_nRunTime = MyPlatform::GetTimeStampMillisecond() - m_nRunTime;
	return nRet;
}

int CZipMark::destroy()
{

	return BENCH_MARK_OK;
}

typedef double Double;

int CZipMark::WzipDemo7Z()
{
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	TCHAR szZipTempFile[_MAX_PATH] = { 0 };
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	SHFILEOPSTRUCT shfo = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("7-Zip\\zipMeasure"), szZipDir);
	MyPlatform::GetFullPathUnderExe(_T("7-Zip\\zipMeasure_temp.zip"), szZipTempFile);
	szZipDir[_tcslen(szZipDir) + 1] = '\0';
	shfo.wFunc = FO_DELETE;
	shfo.pFrom = szZipDir;
	shfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	/// 解压
	{
		
		MyPlatform::tstringstream streamParam;

		std::auto_ptr<TCHAR> szParam;
		streamParam << _T("\"") << m_sz7zFP << _T("\" x ")
			<< _T("\"") << m_szSourceFP << _T("\" ")
			<< _T("-aoa ")
			<< _T("-o\"") << szZipDir << _T("\"");

		SHFileOperation(&shfo);
		CreateDirectory(szZipDir, NULL);
		MyPlatform::tstring strCmd = streamParam.str();
		szParam.reset((TCHAR *)calloc(strCmd.size() + 1, sizeof(TCHAR)));
		if (NULL == szParam.get())
		{
			m_strLastErrMsg.Format(_T("解压中开辟内存失败"));
			return -1;
		}
		_tcscpy_s(szParam.get(), strCmd.size() + 1, strCmd.c_str());

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		//LOGTT(TAG, TEXT("解压文件"));
		if (!CreateProcess(NULL, szParam.get(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			m_strLastErrMsg.Format(_T("解压中创建解压进程失败"));
			return -2;
		}
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	/// 压缩
	{
		//ILog *lpLog = GetController()->GetTipsLog();
		//LOGTT(TAG, TEXT("压缩文件"));
		MyPlatform::tstringstream streamParam;
		std::auto_ptr<TCHAR> szParam;

		DeleteFile(szZipTempFile);
		streamParam << _T("\"") << m_sz7zFP << _T("\" a -tzip ")
			<< _T("\"") << szZipTempFile << _T("\" ")
			<< _T("\"") << szZipDir << _T("\\");
		MyPlatform::tstring strCmd = streamParam.str();
		szParam.reset((TCHAR *)calloc(strCmd.size() + 1, sizeof(TCHAR)));
		if (NULL == szParam.get())
		{
			m_strLastErrMsg.Format(_T("压缩中开辟内存失败"));
			return -3;
		}
		_tcscpy_s(szParam.get(), strCmd.size() + 1, strCmd.c_str());

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		//LOGTT(TAG, TEXT("解压文件"));
		if (!CreateProcess(NULL, szParam.get(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			m_strLastErrMsg.Format(_T("压缩中创建压缩进程失败"));
			return -4;
		}
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		SHFileOperation(&shfo);
		DeleteFile(szZipTempFile);
	}

	return BENCH_MARK_OK;
}
