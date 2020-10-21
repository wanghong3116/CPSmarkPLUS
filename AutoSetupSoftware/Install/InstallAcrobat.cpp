#include "stdafx.h"
#include "InstallAcrobat.h"

InstallAcrobat::InstallAcrobat()
	: InstallObject::InstallObject(_T("Software\\Adobe Acrobat\\Adobe Acrobat\\Setup.exe"))
{
}

InstallAcrobat::~InstallAcrobat()
{

}

int InstallAcrobat::Install()
{
	__super::Install();
	if (0 == StartSetup())
		WaitSetupOver();
	 MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	return 0;
}

int InstallAcrobat::StartSetup()
{
	const int c_nMaxWaitTime = 60 * 1000;
	HWND  hSetUpWnd = NULL;
	DWORD dwTime = 0;

	while (dwTime < c_nMaxWaitTime)
	{
		 if (NULL != (hSetUpWnd = FindSetUpWindow()))
		{
			break;
		}
		Sleep(100);
		dwTime += 100;
	}
	if (NULL == hSetUpWnd)
	{
		return -1;
	}
	 
	Sleep(1000);
	SendMessage(hSetUpWnd, WM_COMMAND, MAKEWPARAM(0x4A12, BN_CLICKED), (LPARAM)GetDlgItem(hSetUpWnd, 0x4A12));
	Sleep(1000 * 2);
	return 0;
}

int InstallAcrobat::WaitSetupOver()
{
	const int c_nMaxWaitTime = 60 * 1000 * 5;
	const int c_nOverID = 0x4997;

	DWORD dwTime = 0;
	HWND hOver = NULL;
	HWND  hSetUpWnd = NULL;
	while (dwTime < c_nMaxWaitTime)
	{
		hSetUpWnd = FindSetUpWindow();
		hOver = ::GetDlgItem(hSetUpWnd, c_nOverID);
		if (NULL != hOver  && IsWindowVisible(hOver))
		{
			printf("found hOver...\r\n");
			break;
		}

		Sleep(1000);
		dwTime += 1000;
 	}
	Sleep(1000);
	printf("send EN_SETFOCUS...\r\n");
	SendMessage(hSetUpWnd, WM_COMMAND, MAKEWPARAM(c_nOverID, EN_SETFOCUS), (LPARAM)GetDlgItem(hSetUpWnd, c_nOverID));
	Sleep(1000); 
	printf("send BN_CLICKED...\r\n");
	SendMessage(hSetUpWnd, WM_COMMAND, MAKEWPARAM(c_nOverID, BN_CLICKED), (LPARAM)GetDlgItem(hSetUpWnd, c_nOverID));
 	Sleep(1000);

	return 0;
}

HWND InstallAcrobat::FindSetUpWindow()
{
	const TCHAR* c_szWindoName = _T("Adobe Acrobat DC (Continuous) - ÉèÖÃ");
	const TCHAR* c_szClassName = _T("MsiDialogCloseClass");
	HWND hSetupWnd =  ::FindWindow(c_szClassName, c_szWindoName);
	SetWindowPos(hSetupWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return hSetupWnd;
}
 