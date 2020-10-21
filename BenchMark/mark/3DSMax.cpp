#include "stdafx.h"
#include "MarkObject.h"
#include "3DSMax.h"

C3DSMaxMark::C3DSMaxMark()
{
	//_tcscpy_s(m_szDescript, _T("3dsmax:3D场景渲染"));
}

C3DSMaxMark::~C3DSMaxMark()
{
	LogInfo(_T("Invoke ~C3DSMaxMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int C3DSMaxMark::init()
{
	DWORD dwType = 0, dwSize = 0;

	dwSize = sizeof(m_sz3DSMaxFP);
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Classes\\CLSID\\{5985293F-A250-11d1-BCC1-0060B01A6606}\\LocalServer"), _T(""), RRF_RT_REG_SZ | 0x00010000, &dwType, m_sz3DSMaxFP, &dwSize);
	if (0 != _taccess(m_sz3DSMaxFP, 0))
	{
		g_ini.GetString(CFG_MARK, CFG_MARK_3DSMAX_FILE_PATH, m_sz3DSMaxFP, _ARRAYSIZE(m_sz3DSMaxFP));
		if (0 != _taccess(m_sz3DSMaxFP, 0))
		{
			LogError({ _T("3DSMax没有安装") });
			return-1;
		}
	}
	
	if (0 != _taccess(m_szSourceMaterialFP, 0))
	{
		LogError({ _T("素材文件不存在") });
		return -2;
	}

	if (BENCH_MARK_OK != wirteScript())
	{
		LogError({ _T("脚本写入错误") });
		return -3;
	}

	return BENCH_MARK_OK;
}

int C3DSMaxMark::run()
{
	const DWORD c_dwMaxRenderTime = 1000 * 60 * 15;
	CString strCmd;
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	DWORD dwRunTime = 0;
	int nRunResult = BENCH_MARK_OK;
	HWND hRegister = NULL, hWelcome = NULL;
	POINT pointClick = { 0 };
	
	m_nRunTime = MyPlatform::GetTimeStampMillisecond();


	LogInfo(_T("3dMAX正在渲染。。。"));
	strCmd.Format(_T("%s /Language=CHS -U MAXScript \"%s\""), m_sz3DSMaxFP, m_szScriptFP);
	if (FALSE == CreateProcess(NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
		return -1;
	}

	int count = 0;
	while (true)
	{
		dwRunTime += 1000;
		if (dwRunTime >= c_dwMaxRenderTime)
		{
			LogError({ _T("3DSMax 渲染超时！\r\n") });
			nRunResult = -2;
			break;
		}
		if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hThread, 1000))
		{
			break;
		}
		
		if (NULL != (hWelcome = ::FindWindow(_T("Qt5QWindow"), _T("3dsmax"))))
		{
			SendMessage(hWelcome, WM_CLOSE, 0, 0);
		}
		
		if (NULL == (hRegister = FindWindow(_T("Qt5QWindowIcon"), _T("Autodesk 许可"))))
		{
			continue;
		}
		else
		{
			RECT rectWindow;
			SetWindowPos(hRegister, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			GetWindowRect(hRegister, &rectWindow);
			pointClick.x = rectWindow.left + 111;
			pointClick.y = rectWindow.top + 625;
			SetCursorPos(pointClick.x, pointClick.y);
			dwRunTime += 1000;
			SleepAndRecord(500);
			doLButton(pointClick.x, pointClick.y);
			SleepAndRecord(500);
			dwRunTime += 1000;
		}	
	}

	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	DeleteFile(m_szScriptFP);

	m_nRunTime = MyPlatform::GetTimeStampMillisecond() - m_nRunTime;
	return nRunResult;
}

int C3DSMaxMark::destroy()
{
	LogInfo(_T("Invoke C3DSMaxMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	MyPlatform::KillProcessFromName(_T("3dsmax.exe"));
	SleepAndRecord(500);
	return BENCH_MARK_OK;
}
