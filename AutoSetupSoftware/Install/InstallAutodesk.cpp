#include "stdafx.h"
#include "InstallAutodesk.h"

InstallAutodesk::InstallAutodesk(const TCHAR* szSetupFP, const TCHAR* szSetupWindowName)
	: InstallObject::InstallObject(szSetupFP)
{
	_tcscpy_s(m_szSetUpWindowName, szSetupWindowName);
}

InstallAutodesk::~InstallAutodesk()
{

}

int InstallAutodesk::Install()
{
	if (0 != __super::Install())
	{
		return -1;
	}
	
	if (NULL == (m_hSetupWindow = FindSetupWindow(1000 * 60 * 3)))
	{
		return -2;
	}
	StartPage();
	Sleep(1000 * 10);

	if (NULL == (m_hSetupWindow = FindSetupWindow(1000 * 60 * 3)))
	{
		return -2;
	}
	AcceptPage();
	Sleep(1000 * 10);

	if (NULL == (m_hSetupWindow = FindSetupWindow(1000 * 60 * 3)))
	{
		return -2;
	}
	SetupPage();
	Sleep(1000 * 10);

	OverPage();

	return 0;
}

HWND InstallAutodesk::FindSetupWindow(int nFindMaxTime)
{
	const TCHAR* c_szClassName = _T("#32770");

	DWORD dwTime = 0;

	HWND hSetUpWnd = NULL;
	while (dwTime < nFindMaxTime)
	{
		if (NULL != (hSetUpWnd = ::FindWindow(c_szClassName, m_szSetUpWindowName)))
		{
			break;
		}
		Sleep(100);
		dwTime += 100;
	}
	if (NULL == hSetUpWnd)
	{
		return NULL;
	}

	return hSetUpWnd;

}

int InstallAutodesk::StartPage()
{
	const int c_nSetupID = 0x439;

	RECT rectWindow = { 0 };
	POINT pointClick;
	HWND hSetupButton = NULL;

	hSetupButton = GetDlgItem(m_hSetupWindow, c_nSetupID);
	GetWindowRect(hSetupButton, &rectWindow);
	pointClick.x = rectWindow.left + (rectWindow.left + rectWindow.right) / 2;
	pointClick.y = rectWindow.top + (rectWindow.top + rectWindow.bottom) / 2;
	Sleep(1000);
	SendMessage(hSetupButton, WM_LBUTTONDOWN, 0, MAKELPARAM(pointClick.x, pointClick.y));
	SendMessage(hSetupButton, WM_LBUTTONUP, 0, MAKELPARAM(pointClick.x, pointClick.y));
	Sleep(1000);

	return 0;
}

int InstallAutodesk::AcceptPage()
{
	const int c_nAcceptID = 0x477;
	const int c_nNextID = 0x1;

	Sleep(1000);
	SendMessage(m_hSetupWindow, WM_COMMAND, MAKEWPARAM(c_nAcceptID, BN_CLICKED), (LPARAM)GetDlgItem(m_hSetupWindow, c_nAcceptID));
	Sleep(1000);

	Sleep(1000);
	SendMessage(m_hSetupWindow, WM_COMMAND, MAKEWPARAM(c_nNextID, BN_CLICKED), (LPARAM)GetDlgItem(m_hSetupWindow, c_nNextID));
	Sleep(1000);

	return 0;
}

int InstallAutodesk::SetupPage()
{
	const int c_nNextID = 0x1;

	Sleep(1000);
	SendMessage(m_hSetupWindow, WM_COMMAND, MAKEWPARAM(c_nNextID, BN_CLICKED), (LPARAM)GetDlgItem(m_hSetupWindow, c_nNextID));
	Sleep(1000);

	return 0;
}

int InstallAutodesk::OverPage()
{
	const int c_nMaxWaitTime = 15 * 60 * 1000;
	const TCHAR* c_szOverButtonNameRestart = _T("立即启动");
	const TCHAR* c_szOverButtonNameOK = _T("完成");
	const int c_nOverButtonID = 0x1;

	DWORD dwTime = 0;
	HWND hOverButton = NULL;
	TCHAR szButtonText[_MAX_PATH] = { 0 };

	while (dwTime < c_nMaxWaitTime)
	{
		Sleep(1000);
		dwTime += 1000;
		m_hSetupWindow = FindSetupWindow(1000);
		dwTime += 1000;

		hOverButton = GetDlgItem(m_hSetupWindow, c_nOverButtonID);
		GetDlgItemText(m_hSetupWindow, c_nOverButtonID, szButtonText, _ARRAYSIZE(szButtonText));
		if (NULL != hOverButton && (0 == _tcscmp(szButtonText, c_szOverButtonNameRestart) || 0 == _tcscmp(szButtonText, c_szOverButtonNameOK)))
		{
			break;
		}
	}
	if (NULL == hOverButton)
	{
		return -1;
	}

	Sleep(1000);
	SendMessage(m_hSetupWindow, WM_COMMAND, MAKEWPARAM(c_nOverButtonID, BN_CLICKED), (LPARAM)GetDlgItem(m_hSetupWindow, c_nOverButtonID));
	Sleep(1000);

	return 0;
}

