#include "stdafx.h"
#include "PSscriptingsupport.tlh"
#include "MarkObject.h"
#include "md5/AlMD5Verify.h"

CMarkObject::CMarkObject()
{
}

CMarkObject::~CMarkObject()
{
	LogInfo(_T("Invoke ~CMarkObject.."));
	delete m_opids;
	DeleteResourceDir();
}

void CMarkObject::SetMD5PassWd(const std::string& sMD5)
{
	LogInfo(_T("Invoke MarkObject.SetMD5PassWd.."));
	m_sMD5PassWd = sMD5;
}

void CMarkObject::SetMD5PassWd_T(tstring& sMD5)
{
	LogInfo(_T("Invoke MarkObject.SetMD5PassWd_T.."));
	t_m_sMD5PassWd = sMD5;
}

void CMarkObject::SetResourceFilePathUnicode(const MyPlatform::tstring& sResourceFilePathUnicode)
{
	m_sResourceFilePath = sResourceFilePathUnicode;
}

bool CMarkObject::VerifyFile_T()
{
	LogInfo(_T("Invoke MarkObject.VerifyFile_T.."));
	char szMD5[1024] = { 0 };
	
	AlMD5Verify oMD5Verify(MyPlatform::TcharToChar(m_sResourceFilePath.c_str()).c_str());

	//DeleteResourceDir();
	Sleep(1000 * 5);
	if (!oMD5Verify.Verify(szMD5)) 
	{
		LogError({ _T("资源文件生成验证码失败") });
		return false;
	}
	if (MyPlatform::TcharToChar(t_m_sMD5PassWd.c_str()) != std::string(szMD5))
	{
		LogError({ _T("资源文件验证失败,可能被修改\r\n") });
		return false;
	}
	return true;
}

bool CMarkObject::VerifyFile()
{
	LogInfo(_T("Invoke MarkObject.VerifyFile.."));
	char szMD5[1024] = { 0 };
	AlMD5Verify oMD5Verify(MyPlatform::TcharToChar(m_sResourceFilePath.c_str()).c_str());

	//DeleteResourceDir();
	Sleep(1000 * 5);
	if (!oMD5Verify.Verify(szMD5))
	{
		LogError({ _T("资源文件生成验证码失败") });
		return false;
	}
	if (m_sMD5PassWd != std::string(szMD5))
	{
		LogError({ _T("资源文件验证失败,可能被修改\r\n") });
		return false;
	}
	return true;
}

bool CMarkObject::UnzipResourceFile()
{
	LogInfo(_T("Invoke MarkObject.UnzipResourceFile.."));
	//开始解压.
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	TCHAR szWinrarFP[_MAX_PATH] = { 0 }; //winRar path

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("WinRARUse\\WinRAR.exe"), szWinrarFP);
	//MyPlatform::GetFullPathUnderExe(_T("WinRAR.exe"), szWinrarFP); 
	MyPlatform::GetFullPathUnderExe(_T("Resources\\"), szZipDir);
	MyPlatform::CreateFullDirectory(szZipDir);

	/// 解压
	{
		MyPlatform::tstringstream streamParam;

		std::auto_ptr<TCHAR> szParam;
		streamParam << _T("\"") << szWinrarFP << _T("\" x -o+ -mt32 ")
			<< _T("\"") << m_sResourceFilePath << _T("\" ")
			<< _T("\"") << szZipDir << _T("\"");

		CreateDirectory(szZipDir, NULL);
		MyPlatform::tstring strCmd = streamParam.str();
		szParam.reset((TCHAR *)calloc(strCmd.size() + 1, sizeof(TCHAR)));
		if (NULL == szParam.get())
		{
			LogError({ _T("解压中开辟内存失败") });
			return false;
		}
		_tcscpy_s(szParam.get(), strCmd.size() + 1, strCmd.c_str());

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		LogInfo(_T("Winrar:解压文件"));
		if (!CreateProcess(NULL, szParam.get(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			LogError({ _T("解压中创建解压进程失败") });
			return false;
		}
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	Sleep(1000);

	return true;
}

__int64 CMarkObject::getRunningTime()
{
	return __max(0, m_nRunTime - m_nSleepTime);
}

const TCHAR* CMarkObject::getDescript()
{
	return m_szDescript;
}

EMarkType CMarkObject::getMarkType()
{
	return m_eMarkType;
}

EMarkClassify CMarkObject::getMarkClassify()
{
	return m_eMarkClassify;
}
 
int CMarkObject::runProcessAndWait(TCHAR *szCmd, const TCHAR* szRunDir)
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };


	if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szRunDir, &si, &pi))
	{
		MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
		return -1;
	}

	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return BENCH_MARK_OK;
}

int CMarkObject::runProcessAndWaitRedirect(TCHAR *szCmd, const TCHAR* szRunDir)
{
	//PROCESS_INFORMATION pi = { 0 };
	//STARTUPINFO si = { sizeof(STARTUPINFO) };

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	TCHAR szOutputLog[512];

	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\out.txt"), szOutputLog);

	HANDLE h = CreateFile(szOutputLog,
		FILE_APPEND_DATA,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL ret = FALSE;
	DWORD flags = CREATE_NO_WINDOW;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = h;
	si.hStdOutput = h;

	//TCHAR cmd[] = TEXT("Test.exe 30");
	//ret = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);

	//if (ret)
	//{
	//	CloseHandle(pi.hProcess);
	//	CloseHandle(pi.hThread);
	//	return 0;
	//}


	//if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szRunDir, &si, &pi))
	if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, TRUE, flags, NULL, szRunDir, &si, &pi))
	{
		MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
		return -1;
	}

	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(h);

	return BENCH_MARK_OK;
}

void CMarkObject::changePathDoubleSplit(TCHAR *szPath, int nBufLen)
{
	TCHAR szTemp[_MAX_PATH * 2] = { 0 };
	TCHAR* pTempPos = szTemp;
	TCHAR* pPathPos = szPath;

	while (*pPathPos != _T('\0'))
	{
		if (*pPathPos == _T('\\'))
		{
			*pTempPos = *pPathPos;
			pTempPos++;
			*pTempPos = *pPathPos;
		}
		else
		{
			*pTempPos = *pPathPos;
		}
		pTempPos++;
		pPathPos++;
	}
	_tcscpy_s(szPath, nBufLen, szTemp);
}

int CMarkObject::DeleteResourceDir()
{
	LogInfo(_T("Invoke MarkObject.DeleteResourceDir.."));

	LogInfo({ m_sResourceFilePath.c_str() , _T(" ： -- MarkObject.DeleteResourceDir.m_sResourceFilePath: -- : "), ((tstring)m_sResourceFilePath).c_str() });
	MyPlatform::DeleteDir(m_sResourceFilePath.c_str());

	return 0;
}

void CMarkObject::SleepAndRecord(ULONGLONG dwTime)
{
	curTime += dwTime;
	Sleep((DWORD)dwTime);
}

void CMarkObject::LogError(std::vector<std::tstring> errList)
{
	m_workMark->LogError(errList);
}


void CMarkObject::LogError(std::tstring err, std::tstring desc)
{
	m_workMark->LogError(err, desc);
}

void CMarkObject::LogInfo(std::tstring info)
{
	m_workMark->LogInfo({ info });
}

void CMarkObject::LogInfo(std::vector<std::tstring> infoList)
{
	m_workMark->LogInfo(infoList);
}

void CMarkObject::LogInfo(std::tstring info, std::tstring desc)
{
	m_workMark->LogInfo(info, desc);
}

bool CMarkObject::UnzipResourceFile(const MyPlatform::tstring & resourceFilePath, CString & lastError)
{
	MyPlatform::CInformationTrace::TraceW(3, _bstr_t(_T("MarkObject.Invoke UnzipResourceFile[args]..")));
	//开始解压.
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	TCHAR szWinrarFP[_MAX_PATH] = { 0 }; //winRar path

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("WinRARUse\\WinRAR.exe"), szWinrarFP);
	//MyPlatform::GetFullPathUnderExe(_T("WinRAR.exe"), szWinrarFP);
	MyPlatform::GetFullPathUnderExe(_T(""), szZipDir);
	MyPlatform::CreateFullDirectory(szZipDir);

	/// 解压
	{
		MyPlatform::tstringstream streamParam;

		std::auto_ptr<TCHAR> szParam;
		streamParam << _T("\"") << szWinrarFP << _T("\" x -o+ -mt32 ")
			<< _T("\"") << resourceFilePath << _T("\" ")
			<< _T("\"") << szZipDir << _T("\"");

		CreateDirectory(szZipDir, NULL);
		MyPlatform::tstring strCmd = streamParam.str();
		szParam.reset((TCHAR *)calloc(strCmd.size() + 1, sizeof(TCHAR)));
		if (NULL == szParam.get())
		{
			lastError.Format(_T("解压中开辟内存失败"));
			return false;
		}
		_tcscpy_s(szParam.get(), strCmd.size() + 1, strCmd.c_str());

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		//LogInfo(TEXT("解压文件"));
		if (!CreateProcess(NULL, szParam.get(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			lastError.Format(_T("解压中创建解压进程失败"));
			return false;
		}
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	Sleep(1000);

	return true;
}

void CMarkObject::TakeMeasurement(CCounterContent* details, Function_T function)
{
	LogInfo({_T("Invoke TakeMeasurement..counter: "), MyPlatform::Int64ToStringW(++measurement_counter)});
	curTime = 0;
	int op_id = InitializeItemStart(details);
	if (function() != BENCH_MARK_OK)
		throw;
	UpdateItemEnd(op_id);
}

int CMarkObject::InitializeItemStart(CCounterContent* details)
{
	auto opid_new = -1;
	auto iter = std::find_if(std::begin(m_opids->m_opid_map), std::end(m_opids->m_opid_map), [&](const std::pair<int, std::tstring> &pair)
	{
		return (pair.second.compare(details->m_description) == 0);
	});
	if (iter != std::end(m_opids->m_opid_map))
	{
		opid_new = (*iter).first;
		m_content_map.insert(std::pair<int, CCounterContent*>(opid_new, details));
	}

	std::wostringstream stringStream;
	stringStream << "InitializeItemStart" << opid_new << "##" <<
		details->m_description << "##"
		<< details->m_measurement << "##" << details->m_scriptname << "##"
		<< details->m_starttime << "##" << details->m_timestamp << endl;
	LogInfo(stringStream.str());
	return opid_new;
}

void CMarkObject::UpdateItemEnd(int opid)
{
	std::wostringstream stringStream;
	auto iter = std::find_if(std::begin(m_content_map), std::end(m_content_map), [&](const std::pair<int, CCounterContent*> &pair)
	{
		return (pair.first == opid);
	});
	if (iter != std::end(m_content_map))
	{
		auto content = (*iter).second;
		ULONGLONG endTime = MyPlatform::GetTimeStampMillisecond();
		content->m_measurement = endTime - content->m_starttime - curTime;
		if (content->m_measurement >= 0 && content->m_measurement < INT_MAX)
		{
			stringStream << "UpdateItemEnd" << "#opid#" << opid << "#sleeptime#" << curTime << "#startTime#" << content->m_starttime << "#endTime#"
				<< endTime << "#measurement#" << content->m_measurement << endl;
			LogInfo(stringStream.str());
		}
		else
		{
			stringStream << "UpdateItemEnd Error: " << opid << "#sleepTime#" << curTime << "#startTime#" << content->m_starttime << "#endTime#" << endTime << endl;
			LogError({ stringStream.str() });
			throw exception("UpdateItemEnd Recording Error...");
		}
	}
	else
	{
		stringStream << "UpdateItemEnd" << opid << "##" << "Error, couldn't find previously defined opid" << endl;
		LogError({ stringStream.str() });
		throw exception("UpdateItemEnd couldn't find previously defined opid Error...");
	}
}
