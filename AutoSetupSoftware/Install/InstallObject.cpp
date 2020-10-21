#include "stdafx.h"
#include "InstallObject.h"

InstallObject::InstallObject()
{

}

InstallObject::InstallObject(const TCHAR* szSetupFP)
{
	MyPlatform::GetFullPathUnderExe(szSetupFP, m_szSetupFilePath, _ARRAYSIZE(m_szSetupFilePath));
}

InstallObject::~InstallObject()
{

}

int InstallObject::Install()
{
	TCHAR szCmd[_MAX_PATH * 5] = { 0 };

	if (0 != _taccess(m_szSetupFilePath, 0))
	{
		return -1;
	}

	_stprintf_s(szCmd, _ARRAYSIZE(szCmd), _T("\"%s\""), m_szSetupFilePath);
	if (0 != runProcess(m_szSetupFilePath, NULL, m_hProcess, m_hThread))
	{
		return -2;
	}
	//WaitForSingleObject(m_hThread, INFINITE);

	return 0;
}

int InstallObject::WaitInstallOver()
{
	if (NULL == m_hThread)
	{
		return -1;
	}
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;
	CloseHandle(m_hProcess);
	m_hProcess = NULL;
	return 0;
}

int InstallObject::runProcess(TCHAR *szCmd, const TCHAR* szRunDir, HANDLE& hProcess, HANDLE& hThread)
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szRunDir, &si, &pi))
	{
		MyPlatform::MyTrace(L0, _T("InstallObject::runProcess CreateProcess failed, errno=[%d]"), GetLastError());
		return -1;
	}

	hProcess = pi.hProcess;
	hThread = pi.hThread;
// 	WaitForSingleObject(pi.hThread, INFINITE);
// 	CloseHandle(pi.hThread);
// 	CloseHandle(pi.hProcess);

	return 0;
}

