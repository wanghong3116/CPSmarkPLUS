#include "stdafx.h"
#include "InstallWinrar.h"


InstallWinrar::InstallWinrar()
	: InstallObject::InstallObject(_T("Software\\Winrar_x64.exe"))
{
}

InstallWinrar::~InstallWinrar()
{
}

int InstallWinrar::Install()
{
	__super::Install();
	Sleep(3 * 1000);
  
	const int c_nMaxRunTime = 1000 * 60 * 2;
	HWND hInstall = NULL;
	int nRunTime = 0;
	std::string  szClassName = "#32770";
	std::string szWndName = "WinRAR 5.71";
	 
	for (int nTryIndex = 0; nTryIndex < 4; nTryIndex++)
	{
		 
		while (nRunTime < c_nMaxRunTime)
		{
			Sleep(1000);
			nRunTime += 1000;
			std::vector<HWND> vhWnd;
			if (nTryIndex == 0)
				MyPlatform::FindWindowWithClassNameAndWindowName(_bstr_t(szClassName.c_str()), _bstr_t(szWndName.c_str()), true,  &vhWnd);
			else if (nTryIndex == 1 || nTryIndex == 2)
				MyPlatform::FindWindowWithClassNameAndWindowName(_bstr_t(szClassName.c_str()), _T("WinRAR 简体中文版安装"), true, &vhWnd);
			else if (nTryIndex == 3)
			{
				Sleep(1000);
				MyPlatform::KillProcessFromName(_T("Explorer.EXE"));
				break;
			}
			if ((vhWnd.size() != 0) && (NULL != (hInstall = vhWnd[0])))
			{
				printf("找到welcome=[%x]\r\n", hInstall);
				SendMessage(hInstall, WM_COMMAND, MAKEWPARAM(1, BN_CLICKED), (LPARAM)GetDlgItem(hInstall, 1));
				break;
			}
		}
  	}
	return 0;
 }
