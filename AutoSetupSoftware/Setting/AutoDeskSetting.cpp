#include "stdafx.h"
#include "AutoDeskSetting.h"

AutoDeskSetting::AutoDeskSetting()
{

}

AutoDeskSetting::AutoDeskSetting(const TCHAR* szMainClassName, const TCHAR* szMainWindowName, const TCHAR* szStartArgs)
	:  SettingObject(szMainClassName, szMainWindowName, szStartArgs)
{
// 	_tcscpy_s(m_szMainClassName, _ARRAYSIZE(m_szMainClassName), szMainClassName);
// 	_tcscpy_s(m_szMainWindowName, _ARRAYSIZE(m_szMainWindowName), szMainWindowName);
// 	_tcscpy_s(m_szStartArgs, _ARRAYSIZE(m_szStartArgs), szStartArgs);
}

AutoDeskSetting::~AutoDeskSetting()
{

}

// int AutoDeskSetting::StartApp()
// {
// 	STARTUPINFO si = { sizeof(STARTUPINFO) };
// 	TCHAR szCMD[_MAX_PATH * 3] = { 0 };
// 
// 	_stprintf_s(szCMD, _T("\"%s\" %s"), m_szExeFP, m_szStartArgs);
// 	if (FALSE == CreateProcess(NULL, szCMD, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &m_pi))
// 	{
// 		//MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
// 		return -2;
// 	}
// 
// 	return 0;
// }

int AutoDeskSetting::ClickSerialNumber()
{
	struct T_WndFind
	{
		HWND*	phMainWnd;
		DWORD	dwProcessID;
	};
	HWND hSNWnd = NULL;
	DWORD dwTime = 0;

	T_WndFind wf = { &hSNWnd,  m_pi.dwProcessId };
	EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
		const TCHAR* c_szParitcalWindoName = _T("_LGSDialog");
		TCHAR szClassName[_MAX_PATH] = { 0 };

		GetClassName(hwnd, szClassName, _ARRAYSIZE(szClassName));
		if (NULL != _tcsstr(szClassName, c_szParitcalWindoName))
		{
			DWORD dwWndID = 0;

			T_WndFind* pwf = (T_WndFind*)lParam;

			GetWindowThreadProcessId(hwnd, &dwWndID);
			if (dwWndID == pwf->dwProcessID)
			{
				*(pwf->phMainWnd) = hwnd;
				return FALSE;
			}
		}
		return TRUE;
	}
	, (LPARAM)&wf);

	if (NULL == hSNWnd)
	{
		return -1;
	}
	printf("ClickSerialNumber::找到Button\r\n");
	RECT rectWindow;
	POINT pointClick = { 0 };
	SetWindowPos(hSNWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	GetWindowRect(hSNWnd, &rectWindow);
	pointClick.x = rectWindow.left + 322;
	pointClick.y = rectWindow.top + 252;
	SetCursorPos(pointClick.x, pointClick.y);
	Sleep(1000);
	doLButton(pointClick.x, pointClick.y);
	Sleep(1000);

	return 0;
}

int AutoDeskSetting::ClickPermit()
{
	const TCHAR* c_szWndName = _T("Autodesk 许可");
	const TCHAR* c_szClassName = _T("Qt5QWindowIcon");
	HWND hPermit = NULL;

	if (NULL == (hPermit = FindWindow(c_szClassName, c_szWndName)))
	{
		//printf("无法找到许可界面\r\n");
		return -1;
	}
	printf("ClickPermit::找到许可界面\r\n");
	RECT rectWindow;
	POINT pointClick = { 0 };
	SetWindowPos(hPermit, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	GetWindowRect(hPermit, &rectWindow);
	pointClick.x = rectWindow.left + 720;
	pointClick.y = rectWindow.top + 650;
	SetCursorPos(pointClick.x, pointClick.y);
	Sleep(1000);
	doLButton(pointClick.x, pointClick.y);
	Sleep(1000);
	return 0;
}

int AutoDeskSetting::ClickRun()
{
	const TCHAR* c_szWndName = _T("Autodesk 许可");
	const TCHAR* c_szClassName = _T("Qt5QWindowIcon");
	HWND hPermit = NULL;

	if (NULL == (hPermit = FindWindow(c_szClassName, c_szWndName)))
	{
		//printf("无法找到许可界面\r\n");
		return -1;
	}
	printf("ClickRun::找到许可界面\r\n");
	RECT rectWindow;
	POINT pointClick = { 0 };
	SetWindowPos(hPermit, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	GetWindowRect(hPermit, &rectWindow);
	pointClick.x = rectWindow.left + 115;
	pointClick.y = rectWindow.top + 625;
	SetCursorPos(pointClick.x, pointClick.y);
	Sleep(1000);
	doLButton(pointClick.x, pointClick.y);
	Sleep(1000);
	return 0;
}
 
 int AutoDeskSetting::ClickSelectStyle()
 {
 	const TCHAR* c_szNextButtonName = _T("继续");
 	const int c_nNextButtonID = 0x1;
 	const DWORD c_dwMaxFindWndTime = 1000 * 60;
 	struct T_WndFind
 	{
 		HWND*	phMainWnd;
 		const TCHAR* c_szNextButtonName;
 		int c_nNextButtonID;
 	};
 	HWND hStyleWnd = NULL;
 	DWORD dwTime = 0;
 
 	while (dwTime < c_dwMaxFindWndTime)
 	{
 		T_WndFind wf = { &hStyleWnd,  c_szNextButtonName, c_nNextButtonID };
 		EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
 			const TCHAR* c_szParitcalWindoName = _T("#32770");
 			TCHAR szClassName[_MAX_PATH] = { 0 };
 
 			GetClassName(hwnd, szClassName, _ARRAYSIZE(szClassName));
 			if (0 == _tcscmp(szClassName, c_szParitcalWindoName))
 			{
 				DWORD dwWndID = 0;
 				TCHAR szText[_MAX_PATH] = { 0 };
 				T_WndFind* pwf = (T_WndFind*)lParam;
 
 				GetDlgItemText(hwnd, pwf->c_nNextButtonID, szText, _ARRAYSIZE(szText));
 				GetWindowThreadProcessId(hwnd, &dwWndID);
 				if (NULL != GetDlgItem(hwnd, pwf->c_nNextButtonID) && 0 == _tcscmp(pwf->c_szNextButtonName, szText))
 				{
 					*(pwf->phMainWnd) = hwnd;
 					return FALSE;
 				}
 			}
 			return TRUE;
 		}
 		, (LPARAM)&wf);
 
 		if (NULL != hStyleWnd)
 		{
 			break;
 		}
 
 		Sleep(1000);
 		dwTime += 1000;
 	}
 
 	if (NULL == hStyleWnd)
 	{
 		return -1;
 	}
	printf("Found 继续button\r\n");
 	::SendMessage(hStyleWnd, WM_COMMAND, MAKEWPARAM(c_nNextButtonID, BN_CLICKED), (LPARAM)::GetDlgItem(hStyleWnd, c_nNextButtonID));
 	Sleep(1000 * 5);
 	return 0;
 }

int AutoDeskSetting::ClickDataCollect()
{
	const int c_nNextButtonID = 0x1;
	HWND hData = NULL;

	if (NULL == (hData = FindWindow(_T("#32770"), _T("数据收集和使用"))))
	{
		return -1;
	}
	printf("Found 数据收集和使用\r\n");
	::SendMessage(hData, WM_COMMAND, MAKEWPARAM(c_nNextButtonID, BN_CLICKED), (LPARAM)::GetDlgItem(hData, c_nNextButtonID));
	Sleep(1000 * 10);
	return 0;
}
// 
// bool AutoDeskSetting::IsFindMainWindowAndVisible()
// {
// 	struct T_WndFind
// 	{
// 		HWND*	phMainWnd;
// 		DWORD	dwProcessID;
// 		const TCHAR* szClassName;
// 		const TCHAR* szWndName;
// 	};
// 	HWND hMainWnd = NULL;
// 	DWORD dwExStyle = NULL;
// 
// 	if (0 == *m_szMainClassName && 0 == *m_szMainWindowName)
// 	{
// 		return false;
// 	}
// 
// 	T_WndFind wf = { &hMainWnd,  m_pi.dwProcessId, m_szMainClassName, m_szMainWindowName };
// 	EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
// 		TCHAR szClassName[_MAX_PATH] = { 0 };
// 		TCHAR szWindowName[_MAX_PATH] = { 0 };
// 		T_WndFind* pwf = (T_WndFind*)lParam;
// 
// 		GetClassName(hwnd, szClassName, _ARRAYSIZE(szClassName));
// 		GetWindowText(hwnd, szWindowName, _ARRAYSIZE(szWindowName));
// 		if (NULL != _tcsstr(szClassName, pwf->szClassName) && NULL != _tcsstr(szWindowName, pwf->szWndName))
// 		{
// 			DWORD dwWndID = 0;
// 
// 			GetWindowThreadProcessId(hwnd, &dwWndID);
// 			if (dwWndID == pwf->dwProcessID)
// 			{
// 				//printf("找到主窗口窗口句柄为%x classname=[%s] windowName=[%s]\r\n", hwnd, szClassName, szWindowName);
// 				//Sleep(1000000);
// 				*(pwf->phMainWnd) = hwnd;
// 				return FALSE;
// 			}
// 		}
// 		return TRUE;
// 	}
// 	, (LPARAM)&wf);
// 
// 	if (NULL == hMainWnd)
// 	{
// 		return false;
// 	}
// 	if (!IsWindowVisible(hMainWnd))
// 	{
// 		return false;
// 	}
// 	dwExStyle = GetWindowLong(hMainWnd, GWL_EXSTYLE);
// 	if (dwExStyle & WS_EX_LAYERED)
// 	{
// 		return false;
// 	}
// 	
// 	return true;
// }
