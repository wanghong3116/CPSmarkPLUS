#include "stdafx.h"
#include "SettingObject.h"

SettingObject::SettingObject()
{

}

SettingObject::SettingObject(const TCHAR* szMainClassName, const TCHAR* szMainWindowName, const TCHAR* szStartArgs)
{
	_tcscpy_s(m_szMainClassName, _ARRAYSIZE(m_szMainClassName), szMainClassName);
	_tcscpy_s(m_szMainWindowName, _ARRAYSIZE(m_szMainWindowName), szMainWindowName);
	_tcscpy_s(m_szStartArgs, _ARRAYSIZE(m_szStartArgs), szStartArgs);
}

SettingObject::~SettingObject()
{

}

int SettingObject::StartApp()
{
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	TCHAR szCMD[_MAX_PATH * 3] = { 0 };

	_stprintf_s(szCMD, _T("\"%s\" %s"), m_szExeFP, m_szStartArgs);
	if (FALSE == CreateProcess(NULL, szCMD, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &m_pi))
	{
		//MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
		return -2;
	}

	return 0;
}

bool SettingObject::IsFindMainWindowAndVisible()
{
	struct T_WndFind
	{
		HWND*	phMainWnd;
		DWORD	dwProcessID;
		const TCHAR* szClassName;
		const TCHAR* szWndName;
	};
	HWND hMainWnd = NULL;
	DWORD dwExStyle = NULL;

	if (0 == *m_szMainClassName && 0 == *m_szMainWindowName)
	{
		return false;
	}

	T_WndFind wf = { &hMainWnd,  m_pi.dwProcessId, m_szMainClassName, m_szMainWindowName };
	EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
		TCHAR szClassName[_MAX_PATH] = { 0 };
		TCHAR szWindowName[_MAX_PATH] = { 0 };
		T_WndFind* pwf = (T_WndFind*)lParam;

		GetClassName(hwnd, szClassName, _ARRAYSIZE(szClassName));
		GetWindowText(hwnd, szWindowName, _ARRAYSIZE(szWindowName));
		if (NULL != _tcsstr(szClassName, pwf->szClassName) && NULL != _tcsstr(szWindowName, pwf->szWndName))
		{
			DWORD dwWndID = 0;

			GetWindowThreadProcessId(hwnd, &dwWndID);
			if (dwWndID == pwf->dwProcessID)
			{
				//printf("找到主窗口窗口句柄为%x classname=[%s] windowName=[%s]\r\n", hwnd, szClassName, szWindowName);
				//Sleep(1000000);
				*(pwf->phMainWnd) = hwnd;
				return FALSE;
			}
		}
		return TRUE;
	}
	, (LPARAM)&wf);

	if (NULL == hMainWnd)
	{
		return false;
	}
	if (!IsWindowVisible(hMainWnd))
	{
		return false;
	}
	dwExStyle = GetWindowLong(hMainWnd, GWL_EXSTYLE);
	if (dwExStyle & WS_EX_LAYERED)
	{
		return false;
	}
	
	return true;
}
