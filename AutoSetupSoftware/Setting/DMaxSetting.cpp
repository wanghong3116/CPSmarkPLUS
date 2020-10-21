#include "stdafx.h"
#include "DMaxSetting.h"

DMaxSetting::DMaxSetting()
	: AutoDeskSetting(_T("Qt5QWindowIcon"), _T("Autodesk 3ds Max 2018"), _T("/Language=CHS"))
{

}

DMaxSetting::~DMaxSetting()
{

}

int DMaxSetting::Init()
{
	TCHAR szDMaxFP[_MAX_PATH] = { 0 };
	DWORD dwType = 0, dwSize = 0;
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	dwSize = sizeof(szDMaxFP);
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Classes\\CLSID\\{5985293F-A250-11d1-BCC1-0060B01A6606}\\LocalServer"), _T(""), RRF_RT_REG_SZ | 0x00010000, &dwType, szDMaxFP, &dwSize);
	if (0 != _taccess(szDMaxFP, 0))
	{
		return -1;
	}
	_tcscpy_s(m_szExeFP, _ARRAYSIZE(m_szExeFP), szDMaxFP);

	return 0;
}
// 
// int DMaxSetting::ClickSerialNumber()
// {
// 	const DWORD c_dwMaxFindWndTime = 1000 * 60 * 3;
// 	struct T_WndFind
// 	{
// 		HWND*	phMainWnd;
// 		DWORD	dwProcessID;
// 	};
// 	HWND hSNWnd = NULL;
// 	DWORD dwTime = 0;
// 
// 	while (dwTime < c_dwMaxFindWndTime)
// 	{
// 		T_WndFind wf = { &hSNWnd,  pi.dwProcessId };
// 		EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
// 			const TCHAR* c_szParitcalWindoName = _T("_LGSDialog");
// 			TCHAR szClassName[_MAX_PATH] = { 0 };
// 
// 			GetClassName(hwnd, szClassName, _ARRAYSIZE(szClassName));
// 			if (NULL != _tcsstr(szClassName, c_szParitcalWindoName))
// 			{
// 				DWORD dwWndID = 0;
// 
// 				T_WndFind* pwf = (T_WndFind*)lParam;
// 
// 				GetWindowThreadProcessId(hwnd, &dwWndID);
// 				if (dwWndID == pwf->dwProcessID)
// 				{
// 					*(pwf->phMainWnd) = hwnd;
// 					return FALSE;
// 				}
// 			}
// 			return TRUE;
// 		}
// 		, (LPARAM)&wf);
// 
// 		if (NULL != hSNWnd)
// 		{
// 			break;
// 		}
// 
// 		Sleep(1000);
// 		dwTime += 1000;
// 	}
// 
// 	if (NULL == hSNWnd)
// 	{
// 		return -1;
// 	}
// 
// 	RECT rectWindow;
// 	POINT pointClick = { 0 };
// 	SetWindowPos(hSNWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// 	GetWindowRect(hSNWnd, &rectWindow);
// 	pointClick.x = rectWindow.left + 322;
// 	pointClick.y = rectWindow.top + 252;
// 	SetCursorPos(pointClick.x, pointClick.y);
// 	Sleep(1000);
// 	doLButton(pointClick.x, pointClick.y);
// 	Sleep(1000);
// 
// 	return 0;
// }
// 
// int DMaxSetting::ClickPermit()
// {
// 	const TCHAR* c_szWndName = _T("Autodesk 许可");
// 	const TCHAR* c_szClassName = _T("Qt5QWindowIcon");
// 	HWND hPermit = NULL;
// 
// 	if (NULL == (hPermit = MyPlatform::FindWindowWithTime(c_szClassName, c_szWndName, 1000 * 60 * 3)))
// 	{
// 		printf("无法找到许可界面\r\n");
// 		return -1;
// 	}
// 	RECT rectWindow;
// 	POINT pointClick = { 0 };
// 	SetWindowPos(hPermit, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// 	GetWindowRect(hPermit, &rectWindow);
// 	pointClick.x = rectWindow.left + 720;
// 	pointClick.y = rectWindow.top + 650;
// 	SetCursorPos(pointClick.x, pointClick.y);
// 	Sleep(1000);
// 	doLButton(pointClick.x, pointClick.y);
// 	Sleep(1000);
// 	return 0;
// }
// 
// int DMaxSetting::ClickRun()
// {
// 	const TCHAR* c_szWndName = _T("Autodesk 许可");
// 	const TCHAR* c_szClassName = _T("Qt5QWindowIcon");
// 	HWND hPermit = NULL;
// 
// 	if (NULL == (hPermit = MyPlatform::FindWindowWithTime(c_szClassName, c_szWndName, 1000 * 60 * 3)))
// 	{
// 		printf("无法找到许可界面\r\n");
// 		return -1;
// 	}
// 	RECT rectWindow;
// 	POINT pointClick = { 0 };
// 	SetWindowPos(hPermit, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// 	GetWindowRect(hPermit, &rectWindow);
// 	pointClick.x = rectWindow.left + 115;
// 	pointClick.y = rectWindow.top + 625;
// 	SetCursorPos(pointClick.x, pointClick.y);
// 	Sleep(1000);
// 	doLButton(pointClick.x, pointClick.y);
// 	Sleep(1000);
// 	return 0;
// }

//int DMaxSetting::ClickSelectStyle()
//{
//	const TCHAR* c_szNextButtonName = _T("继续");
//	const int c_nNextButtonID = 0x1;
//	struct T_WndFind
//	{
//		HWND*	phMainWnd;
//		DWORD	dwProcessID;
//		const TCHAR* c_szNextButtonName;
//		int c_nNextButtonID;
//	};
//	HWND hStyleWnd = NULL;
//	std::vector<HWND> vhWnd;
//	MyPlatform::FindWindowWithClassNameAndWindowName( _T("Qt5QWindowIcon"), _T("#32770"), true, &vhWnd);
//	if (vhWnd.size() != 0 && NULL != (hStyleWnd = vhWnd[0]))
//	{
//		::SendMessage(hStyleWnd, WM_COMMAND, MAKEWPARAM(c_nNextButtonID, BN_CLICKED), (LPARAM)::GetDlgItem(hStyleWnd, c_nNextButtonID));
//		Sleep(1000);
//		return 0;
//	}
//	else
//	{
//		return -1;
//	}
//}
// 
// int DMaxSetting::ClickDataCollect()
// {
// 	const int c_nNextButtonID = 0x1;
// 	HWND hData = NULL;
// 
// 	if (NULL == (hData = MyPlatform::FindWindowWithTime(_T("#32770"), _T("数据收集和使用"), 1000 * 60 * 3)))
// 	{
// 		return -1;
// 	}
// 
// 	::SendMessage(hData, WM_COMMAND, MAKEWPARAM(c_nNextButtonID, BN_CLICKED), (LPARAM)::GetDlgItem(hData, c_nNextButtonID));
// 	Sleep(10000000);
// 	return 0;
// }

int DMaxSetting::Set()
{
	const int c_nMaxTime = 1000 * 60;
	int nRunTime = 0;

	if (0 != Init())
	{
		printf("无法启动3dmax\r\n");
		return -1;
	}

	 StartApp();

	const int c_nMaxWaitMainWndTime = (int)(1000 * 60);
	int nWaitTime = 0;
	while (nRunTime < c_nMaxTime && nWaitTime < c_nMaxWaitMainWndTime)
	{
		if (IsFindMainWindowAndVisible())
		{
			nWaitTime += 1000;
		}
		else
		{
			nWaitTime = 0;
		}
		Sleep(1000);
		ClickSerialNumber();
		Sleep(1000);
		ClickRun();
		Sleep(2000);
		HWND hWelcome = NULL;
		if (NULL != (hWelcome = ::FindWindow(_T("Qt5QWindow"), _T("3dsmax"))))
		{
			printf("Found Welcome page.\r\n");
			SendMessage(hWelcome, WM_CLOSE, 0, 0);
		}
		Sleep(2000);
		ClickDataCollect();
		Sleep(2000);
		ClickSelectStyle();
		Sleep(1000);
		nRunTime += 10000;
	}
	Sleep(1000 * 2);
	Destroy();
	Sleep(1000 * 2);
 
	return 0;
}

int DMaxSetting::Destroy()
{
	Sleep(1000);
	MyPlatform::KillProcessFromName(_T("3dsmax.exe"));
	MyPlatform::KillProcessFromName(_T("LMU.exe"));
	Sleep(1000);
	return 0;
}
