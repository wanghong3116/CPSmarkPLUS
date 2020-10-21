#include "stdafx.h"
#include "OutlookSetting.h"
#include "OfficeRegisterWindow.h"

OutlookSetting::OutlookSetting()
	:SettingObject(_T("rctrl_renwnd32"), _T("Outlook"), _T(""))
{
}

OutlookSetting::~OutlookSetting()
{
}

int OutlookSetting::Init()
{
	CoInitialize(NULL);
	
	MyPlatform::GetAppPathFromRegAppPath(m_szExeFP, _ARRAYSIZE(m_szExeFP), _T("OUTLOOK.EXE"));
	if (0 != _taccess(m_szExeFP, 0))
	{
		return -1;
	}
// 	m_threadDisplay.swap(std::thread([](OutlookSetting* pOSetting)->void {
// 
// 		pOSetting->StartAppThread();
// 		
// 		
// 	}, this));
	//Sleep(1000 * 20);
	return 0;
}

int OutlookSetting::Set()
{
	const int c_nNextID = 0x3024;
	const int c_nOverID = 0x3025;
	const int c_nAccountNoID = 0xB;
	const int c_nAccountYESID = 0xA;
	const int c_nNoAccUseID = 0x1E;
	const int c_nMaxRunTime = 1000 * 60;
	HWND hWndWelcome = NULL;
	HWND hAccountWnd = NULL;
	HWND hNoAccWnd = NULL;
	RECT rectNoAccUseButton = { 0 };
	POINT pointClick = { 0 };
	int nFuncResult = 0;
	int nRunTime = 0;
	
	Init();

	Destroy();
	Sleep(1000 * 2);

	StartApp();
	Sleep(1000 * 2);

	while (nRunTime < c_nMaxRunTime && !IsFindMainWindowAndVisible())
	{
		Sleep(1000);
		nRunTime += 1000;
		if (NULL == (hWndWelcome = FindWindow(_T("#32770"), _T("欢迎使用 Microsoft Outlook 2016")))
			&& NULL == (hWndWelcome = FindWindow(_T("#32770"), _T("Microsoft Outlook 账户设置")))
			&& NULL == (hWndWelcome = FindWindow(_T("#32770"), _T("取消电子邮件帐户设置")))
			)
		{
			continue;
		}
		printf("找到welcome=[%x]\r\n", hWndWelcome);

		std::vector<HWND> vhChild;

		EnumChildWindows(hWndWelcome, [](_In_ HWND   hwnd, _In_ LPARAM lParam)->BOOL {
			std::vector<HWND>* pvhChild = (std::vector<HWND>*)lParam;
			TCHAR szClassName[_MAX_PATH] = { 0 };

			GetClassName(hwnd, szClassName, _ARRAYSIZE(szClassName));
			printf("Child wnd=[%s]\r\n", szClassName);
			if (0 == _tcsicmp(_T("#32770"), szClassName))
			{
				pvhChild->push_back(hwnd);
			}

			return true;

		}, (LPARAM)&vhChild);

		for (size_t nWndIndex = 0; nWndIndex < vhChild.size(); nWndIndex++)
		{
			TCHAR szText[_MAX_PATH] = { 0 };

			GetWindowText(vhChild[nWndIndex], szText, _ARRAYSIZE(szText));
			_tprintf(_T(" Child wind [%s] visible=[%d]\r\n"), szText, ::IsWindowVisible(vhChild[nWndIndex]));
			if (!::IsWindowVisible(vhChild[nWndIndex]))
			{
				continue;
			}

			if (0 == _tcsicmp(_T("Microsoft Outlook 账户设置"), szText))
			{
				printf("无账号设置\r\n");
				hAccountWnd = vhChild[nWndIndex];
				Sleep(1000);
				nRunTime += 1000;
				CheckDlgButton(hAccountWnd, c_nAccountNoID, BST_CHECKED);
				CheckDlgButton(hAccountWnd, c_nAccountYESID, BST_UNCHECKED);
				Sleep(1000);
				nRunTime += 1000;

				Sleep(1000);
				nRunTime += 1000;
				/// 下一步
				Sleep(1000);
				nRunTime += 1000;
				SendMessage(hWndWelcome, WM_COMMAND, MAKEWPARAM(c_nNextID, BN_CLICKED), (LPARAM)GetDlgItem(hWndWelcome, c_nNextID));
				Sleep(1000);
				nRunTime += 1000;
			}
			else if (0 == _tcsicmp(_T("取消电子邮件帐户设置"), szText))
			{
				printf("取消电子邮件帐户设置\r\n");
				hNoAccWnd = vhChild[nWndIndex];
				for (int nIndex = 0; nIndex < 3; nIndex++)
				{
					Sleep(1000);
					nRunTime += 1000;
					SetWindowPos(hWndWelcome, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					Sleep(1000);
					nRunTime += 1000;
					if (!IsDlgButtonChecked(hNoAccWnd, c_nNoAccUseID))
					{
						GetWindowRect(::GetDlgItem(hNoAccWnd, c_nNoAccUseID), &rectNoAccUseButton);
						pointClick.x = (rectNoAccUseButton.left + rectNoAccUseButton.right) / 2;
						pointClick.y = (rectNoAccUseButton.top + rectNoAccUseButton.bottom) / 2;
						SetCursorPos(pointClick.x, pointClick.y);
						Sleep(1000);
						nRunTime += 1000;
						//doLButton(pointClick.x, pointClick.y);
						mouse_event(MOUSEEVENTF_LEFTDOWN, pointClick.x, pointClick.y, 0, 0);
						Sleep(500);
						nRunTime += 500;
						mouse_event(MOUSEEVENTF_LEFTUP, pointClick.x, pointClick.y, 0, 0);
						Sleep(500);
						nRunTime += 500;
					}

					CheckDlgButton(hNoAccWnd, c_nNoAccUseID, BST_CHECKED);
					//SendMessage(hNoAccWnd, WM_COMMAND, MAKEWPARAM(c_nNoAccUseID, BN_CLICKED), (LPARAM)GetDlgItem(hNoAccWnd, c_nNoAccUseID));
					Sleep(1000);
					nRunTime += 1000;
				}

				/// 结束
				Sleep(1000);
				nRunTime += 1000;
				SendMessage(hWndWelcome, WM_COMMAND, MAKEWPARAM(c_nOverID, BN_CLICKED), (LPARAM)GetDlgItem(hWndWelcome, c_nOverID));
				Sleep(1000);
				nRunTime += 1000;
			}
			else if (0 == _tcsicmp(_T("欢迎使用 Microsoft Outlook 2016"), szText))
			{
				printf("下一步\r\n");
				Sleep(1000);
				nRunTime += 1000;
				SendMessage(hWndWelcome, WM_COMMAND, MAKEWPARAM(c_nNextID, BN_CLICKED), (LPARAM)GetDlgItem(hWndWelcome, c_nNextID));
				Sleep(1000);
				nRunTime += 1000;
			}
		}
	}
	 
	Destroy();
	Sleep(1000 * 2);
	 
	
	return nFuncResult;
}

int OutlookSetting::Destroy()
{
	Sleep(1000);
	MyPlatform::KillProcessFromName(_T("OUTLOOK.EXE"));
	CoUninitialize();
	return 0;
}
// 
// int OutlookSetting::StartAppThread()
// {
// 	for (int nIndex = 0; nIndex < 3; nIndex++)
// 	{
// 		Outlook::_ApplicationPtr pApp;
// 
// 		Destroy();
// 		Sleep(1000 * 2);
// 
// 		CoInitialize(NULL);
// 		if (FAILED(pApp.CreateInstance(__uuidof(Outlook::Application))))
// 		{
// 			printf("没有正确安装Outlook\r\n");
// 			continue;
// 		}
// 		Sleep(1000);
// 		printf("启动成功,准备显示！\r\n");
// 
// 		try
// 		{
// 			Outlook::MAPIFolderPtr oInbox;
// 			printf("GetNamespace\r\n");
// 			Outlook::_NameSpacePtr oMAPI = pApp->GetNamespace("MAPI");
// 			printf("GetDefaultFolder\r\n");
// 			oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
// 			// 			printf("Display开始\r\n");
// 			// 			oInbox->Display();
// 			// 			printf("Display结束\r\n");
// 
// 		}
// 		catch (_com_error &err)
// 		{
// 			printf("Outlook显示失败 %s\r\n", (const char*)err.Description());
// 			Sleep(1000 * 10);
// 			continue;
// 		}
// 
// 		break;
// 	}
// 	printf("启动结束\r\n");
// 	CoUninitialize();
// 	return 0;
// }
