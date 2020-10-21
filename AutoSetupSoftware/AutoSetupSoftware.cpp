// AutoSetupSoftware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tlhelp32.h>
#include "Install\InstallPr.h"
#include "Install\InstallAE.h"
#include "Install\InstallPS.h"
#include "Install\InstallAcrobat.h"
#include "Install\InstallAutodesk.h"
#include "Install\InstallCAD.h"
#include "Install\InstallDMax.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Install\InstallWinrar.h"
#include "OutlookSetting.h"
#include "Install\InstallOffice.h"
#include "DMaxSetting.h"
#include "CADSetting.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>
#pragma comment(lib, "IPHLPAPI.lib")
 
BOOL WriteInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data)
{
	DWORD Ret; //use to check status
	HKEY hKey; //key


			   //Open the key
	Ret = RegOpenKeyEx(
		hKeyParent,
		subkey,
		0,
		KEY_WRITE,
		&hKey
	);

	if (Ret == ERROR_SUCCESS)
	{

		//Set the value in key
		if (ERROR_SUCCESS !=
			RegSetValueEx(
				hKey,
				valueName,
				0,
				REG_DWORD,
				reinterpret_cast<BYTE *>(&data),
				sizeof(data)))
		{
			RegCloseKey(hKey);
			return FALSE;
		}

		//close the key
		RegCloseKey(hKey);

		return TRUE;
	}

	return FALSE;
}
BOOL WriteInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, std::string data)
{
	DWORD Ret; //use to check status
	HKEY hKey; //key


			   //Open the key
	Ret = RegOpenKeyEx(
		hKeyParent,
		subkey,
		0,
		KEY_WRITE,
		&hKey
	);

	if (Ret == ERROR_SUCCESS)
	{

		//Set the value in key
		if (ERROR_SUCCESS !=
			RegSetValueEx(
				hKey,
				valueName,
				0,
				REG_SZ,
				reinterpret_cast<BYTE *>(&data),
				sizeof(data)))
		{
			RegCloseKey(hKey);
			return FALSE;
		}

		//close the key
		RegCloseKey(hKey);

		return TRUE;
	}

	return FALSE;
}

int RunProcessAndWait(TCHAR *szCmd, const TCHAR* szRunDir)
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szRunDir, &si, &pi))
	{
		return -1;
	}

	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;
}

int SetupOffice()
{
	TCHAR szOfficeSetupFP[_MAX_PATH]	= { 0 };
	TCHAR szCMD[_MAX_PATH * 2]			= { 0 };
	TCHAR szRunDir[_MAX_PATH]			= { 0 };
	HWND hMainWnd						= NULL;
	HWND hHost							= NULL;
	HWND hNet							= NULL;

	MyPlatform::GetFullPathUnderExe(_T("office2016\\office\\setup32.exe"), szOfficeSetupFP);
	MyPlatform::GetFullPathUnderExe(_T("office2016\\office\\"), szRunDir);
	_stprintf_s(szCMD, _MAX_PATH * 2, _T("\"%s\""), szOfficeSetupFP);
	RunProcessAndWait(szCMD, szRunDir);

	Sleep(3 * 1000);
	hMainWnd = FindWindow(_T("Click2RunSetupUIClass"), _T("‪Microsoft Office‬"));
	hHost = FindWindowEx(hMainWnd, NULL, _T("NetUINativeHWNDHost"), _T(""));
	hNet = FindWindowEx(hHost, NULL, _T("NetUIHWND"), _T(""));
	SendMessage(hNet, WM_LBUTTONDOWN, 0, MAKELPARAM(400, 440));
	SendMessage(hNet, WM_LBUTTONUP, 0, MAKELPARAM(400, 440));
	return 0;
}

int CloseNetAdapter()
{
	PIP_ADAPTER_INFO pIpAdapterInfo = NULL;
	ULONG stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = 0;

	if (NULL == (pIpAdapterInfo = new IP_ADAPTER_INFO()))
	{
		return -1;
	}
	nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize); 
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		if (NULL == (pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize]))
		{
			return -1;
		}
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}

	if (ERROR_SUCCESS == nRel)
	{
		TCHAR szSubKey[_MAX_PATH]	= { 0 };
		HKEY hConnect				= NULL;
		TCHAR szValue[_MAX_PATH]	= { 0 };
		DWORD lValueSize			= _MAX_PATH;
		TCHAR szCMD[_MAX_PATH]		= { 0 };
		DWORD dwtype				= 0;
		PIP_ADAPTER_INFO pIpCurrent = pIpAdapterInfo;

		while (pIpCurrent)
		{
			_stprintf_s(szSubKey, _MAX_PATH, _T("SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\%s\\Connection"),
				MyPlatform::CharToTchar(pIpAdapterInfo->AdapterName).c_str());

			RegOpenKey(HKEY_LOCAL_MACHINE, szSubKey, &hConnect);
			RegQueryValueEx(hConnect, _T("Name"), NULL, &dwtype, (LPBYTE)szValue, &lValueSize);
			RegCloseKey(hConnect);
			_stprintf_s(szCMD, _MAX_PATH, _T("netsh interface set interface \"%s\" DISABLED"), szValue);
			RunProcessAndWait(szCMD, NULL);
			pIpCurrent = pIpCurrent->Next;
		}
	}
	delete pIpAdapterInfo;

	return 0;
}

int FindWndHandleByProcessId(HWND* phWnd, DWORD dwProcessID)
{
	struct T_WndFind
	{
		HWND*	phMainWnd;
		DWORD	dwThreadID;
	};

	T_WndFind wf = { phWnd, dwProcessID };
	EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL {
		if (NULL == GetParent(hwnd))
		{
			DWORD dwWndID = 0;

			T_WndFind* pwf = (T_WndFind*)lParam;

			GetWindowThreadProcessId(hwnd, &dwWndID);
			if (dwWndID == pwf->dwThreadID)
			{
				*(pwf->phMainWnd) = hwnd;
				return FALSE;
			}
		}
		return TRUE;
	}
	, (LPARAM)&wf);

	return 0;
}

int FindIgnoreWndAndContinue(DWORD dwProcessID)
{
	HWND hStart = NULL;
	HWND hButtonIgnore = NULL;

	printf("寻找初始化窗口...\r\n");
	while (true)
	{
		FindWndHandleByProcessId(&hStart, dwProcessID);
		Sleep(100);
		if (NULL == hStart)
		{
			continue;
		}

		Sleep(1000);
		printf("点击初“忽略”，开启初始化...\r\n");
		if (NULL != (hButtonIgnore = GetDlgItem(hStart, 0x3F2)))
		{
			SendMessage(hButtonIgnore, WM_LBUTTONDOWN, 0, MAKELPARAM(50, 10));
			SendMessage(hButtonIgnore, WM_LBUTTONUP, 0, MAKELPARAM(50, 10));
		}
		if (!IsWindowVisible(hButtonIgnore))
		{
			break;
		}
	}


	return 0;
}

int StartPSSetupExe()
{
	TCHAR szSetupFP[_MAX_PATH] = { 0 };
	TCHAR szRunDir[_MAX_PATH] = { 0 };
	TCHAR szCMD[_MAX_PATH * 2] = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	printf("\r\n\r\n准备安装Photoshop\r\n启动安装进程...\r\n");
	MyPlatform::GetFullPathUnderExe(_T("Adobe CS6 Photoshop"), szRunDir, _MAX_PATH);
	MyPlatform::GetFullPathUnderExe(_T("Adobe CS6 Photoshop\\Set-up.exe"), szSetupFP, _MAX_PATH);
	_stprintf_s(szCMD, _ARRAYSIZE(szCMD), _T("\"%s\""), szSetupFP);
	if (FALSE == CreateProcess(NULL, szCMD, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szRunDir, &si, &pi))
	{
		return -1;
	}
	printf("启动安装进程成功");
	FindIgnoreWndAndContinue(pi.dwProcessId);
	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;
}

BOOL FindProcessFromName(const TCHAR *szProcessName)
{
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!Process32First(hSnapShot, &pe))
	{
		return -1;
	}

	while (Process32Next(hSnapShot, &pe))
	{
		TCHAR *szCurFile = pe.szExeFile;
		if (0 == _tcsicmp(szCurFile, szProcessName))
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CopyHwndToImage(HWND hWnd, CImage& image)
{
	CRect rectClient;


	GetClientRect(hWnd, rectClient);
	image.Create(rectClient.Width(), rectClient.Height(), 24, BI_RGB);
	HDC hDC = ::GetDC(hWnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, (HBITMAP)image);
	BitBlt(hMemDC, 0, 0, rectClient.Width(), rectClient.Height(), hDC, 0, 0, SRCCOPY);
	//image.Save(_T("D:\\1.png"));
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hMemDC);
	ReleaseDC(hWnd, hDC);

	return 0;
}

int SelectPSSetupOption()
{
	HWND hMainWnd = NULL;
	HWND hStage = NULL;
	CRect rectClient;
	CPoint pointCursor;

	while (true)
	{
		hMainWnd = FindWindow(_T("_macr_PDAPP_Native_frame_window_CS4"), _T("Adobe Photoshop CS6"));
		if (hMainWnd)
		{
			break;
		}
		Sleep(1);
	}
	Sleep(1000);
	hStage = FindWindowEx(hMainWnd, NULL, _T("com.adobe.ape.stage"), _T(""));
	GetWindowRect(hMainWnd, rectClient);
	SetWindowPos(hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	pointCursor = CPoint(250, 430);
	pointCursor.Offset(rectClient.TopLeft());
	doLButton(pointCursor.x, pointCursor.y);
	Sleep(3 * 1000);

	pointCursor = CPoint(600, 520);
	pointCursor.Offset(rectClient.TopLeft());
	doLButton(pointCursor.x, pointCursor.y);
	Sleep(10 * 1000);

	CImage image;
	COLORREF colorSelect = RGB(51, 51, 51);

	CopyHwndToImage(hStage, image);
	if (colorSelect != image.GetPixel(19, 126))
	{
		pointCursor = CPoint(24, 152);
		pointCursor.Offset(rectClient.TopLeft());
		doLButton(pointCursor.x, pointCursor.y);
		Sleep(1000);
	}
	image.Destroy();

	CopyHwndToImage(hStage, image);
	if (colorSelect == image.GetPixel(19, 158))
	{
		pointCursor = CPoint(24, 184);
		pointCursor.Offset(rectClient.TopLeft());
		doLButton(pointCursor.x, pointCursor.y);
		Sleep(1000);
	}
	image.Destroy();

	pointCursor = CPoint(610, 520);
	pointCursor.Offset(rectClient.TopLeft());
	doLButton(pointCursor.x, pointCursor.y);
	Sleep(30 * 1000);

	while (FindProcessFromName(_T("Setup.exe")))
	{
		Sleep(100);
	}
	Sleep(3 * 1000);
	
	pointCursor = CPoint(609, 522);
	pointCursor.Offset(rectClient.TopLeft());
	doLButton(pointCursor.x, pointCursor.y);
	Sleep(2 * 1000);

	return 0;
}

int SetupPhotoshop()
{
	if (0 != StartPSSetupExe())
	{
		return -1;
	}
	SelectPSSetupOption();

	return 0;
}


int SetupAutoCAD()
{
	TCHAR szSetupFP[_MAX_PATH] = { 0 };
	TCHAR szRunDir[_MAX_PATH] = { 0 };
	TCHAR szCMD[_MAX_PATH * 2] = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	MyPlatform::KillProcessFromName(_T("Setup.exe"));
	Sleep(100);
	printf("\r\n\r\n准备安装AutoCAD\r\n启动安装进程...\r\n");
	MyPlatform::GetFullPathUnderExe(_T("AutoCAD_2014"), szRunDir, _MAX_PATH);
	MyPlatform::GetFullPathUnderExe(_T("AutoCAD_2014\\Setup.exe"), szSetupFP, _MAX_PATH);
	_stprintf_s(szCMD, _ARRAYSIZE(szCMD), _T("\"%s\""), szSetupFP);
	if (FALSE == CreateProcess(NULL, szCMD, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szRunDir, &si, &pi))
	{
		return -1;
	}
	printf("启动安装进程成功");
	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	{
		HWND hMainWnd = NULL;
		HWND hButtonSetup = NULL;

		printf("寻找安装按钮...\r\n");
		while (true)
		{
			hMainWnd = FindWindow(_T("#32770"), _T("Autodesk® AutoCAD® 2014"));
			Sleep(100);
			if (NULL == hMainWnd)
			{
				continue;
			}
			if (NULL != (hButtonSetup = GetDlgItem(hMainWnd, 0x439)))
			{
				break;;
			}
		}

		while (true)
		{
			Sleep(500);
			SendMessage(hButtonSetup, WM_LBUTTONDOWN, 0, MAKELPARAM(200, 60));
			SendMessage(hButtonSetup, WM_LBUTTONUP, 0, MAKELPARAM(200, 60));
			Sleep(1000);
			if (!IsWindow(hButtonSetup))
			{
				break;
			}
		}
	}
	
	{
		HWND hMainWnd = NULL;
		HWND hButtonAccept = NULL;
		HWND hNext = NULL;

		printf("接受协议\r\n");
		while (true)
		{
			Sleep(500);

			hMainWnd = FindWindow(_T("#32770"), _T("Autodesk® AutoCAD® 2014"));
			Sleep(100);
			if (NULL == hMainWnd)
			{
				continue;
			}
			if (NULL == (hButtonAccept = GetDlgItem(hMainWnd, 0x477))
				|| NULL == (hNext = GetDlgItem(hMainWnd, 0x1))
				)
			{
				continue;
			}
			SendMessage(hButtonAccept, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 5));
			SendMessage(hButtonAccept, WM_LBUTTONUP, 0, MAKELPARAM(10, 5));
			Sleep(1000);
			if (IsWindowEnabled(hNext) || BST_CHECKED == IsDlgButtonChecked(hButtonAccept, 0x477))
			{
				Sleep(100);
				break;
			}
		}
		while (true)
		{
			Sleep(1000);
			printf("点击“下一步”\r\n");
			SendMessage(hNext, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 5));
			SendMessage(hNext, WM_LBUTTONUP, 0, MAKELPARAM(10, 5));
			if (!IsWindow(hButtonAccept))
			{
				break;
			}
		}
	}
	{
		HWND hMainWnd = NULL;
		HWND hButtonTry = NULL;
		HWND hNext = NULL;

		while (true)
		{
			hMainWnd = FindWindow(_T("#32770"), _T("Autodesk® AutoCAD® 2014"));
			Sleep(100);
			if (NULL == hMainWnd)
			{
				continue;
			}

			Sleep(500);
			printf("点击“试用30天”\r\n");

			if (NULL == (hButtonTry = GetDlgItem(hMainWnd, 0x585))
				|| NULL == (hNext = (GetDlgItem(hMainWnd, 0x1)))
				)
			{
				continue;
			}
			SendMessage(hButtonTry, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 5));
			SendMessage(hButtonTry, WM_LBUTTONUP, 0, MAKELPARAM(10, 5));
			Sleep(1000);
			if (IsWindowEnabled(hNext) || BST_CHECKED == IsDlgButtonChecked(hButtonTry, 0x585))
			{
				Sleep(100);
				break;
			}
		}
		while (true)
		{
			Sleep(1000);
			printf("点击“下一步”\r\n");
			SendMessage(hNext, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 5));
			SendMessage(hNext, WM_LBUTTONUP, 0, MAKELPARAM(10, 5));
			if (!IsWindow(hButtonTry))
			{
				break;
			}
		}
	}
	{
		HWND hMainWnd = NULL;
		HWND hButtonSetup = NULL;

		while (true)
		{
			hMainWnd = FindWindow(_T("#32770"), _T("Autodesk® AutoCAD® 2014"));
			Sleep(100);
			if (NULL == hMainWnd)
			{
				continue;
			}
			Sleep(100);
			if (NULL == (hButtonSetup = GetDlgItem(hMainWnd, 0x1)))
			{
				continue;
			}
			if (!IsWindowEnabled(hButtonSetup))
			{
				printf("AutoCAD已经安装");
				MyPlatform::KillProcessFromName(_T("Setup.exe"));
				return 0;
				break;
			}
			Sleep(1000 * 2);
			SendMessage(hButtonSetup, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 5));
			SendMessage(hButtonSetup, WM_LBUTTONUP, 0, MAKELPARAM(10, 5));
			Sleep(1000);
			if (!IsWindow(hButtonSetup))
			{
				break;
			}
		}
	}
	{
		HWND hMainWnd = NULL;
		printf("AutoCAD安装中.....");
		while (true)
		{
			hMainWnd = FindWindow(_T("#32770"), _T("Autodesk® AutoCAD® 2014"));
			Sleep(100);
			if (NULL == hMainWnd)
			{
				continue;
			}
			break;
		}

		while (IsWindow(hMainWnd))
		{
			Sleep(100);
		}
		Sleep(1000 * 2);
	}
	{
		HWND hMainWnd = NULL;
		HWND hButtonClose = NULL;
		printf("AutoCAD安装完成");
		while (true)
		{
			hMainWnd = FindWindow(_T("#32770"), _T("Autodesk® AutoCAD® 2014"));
			Sleep(100);
			if (NULL == hMainWnd)
			{
				continue;
			}

			if (NULL == (hButtonClose = GetDlgItem(hMainWnd, 0x1)))
			{
				continue;
			}
			Sleep(1000);
			SendMessage(hButtonClose, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 5));
			SendMessage(hButtonClose, WM_LBUTTONUP, 0, MAKELPARAM(10, 5));
			Sleep(1000);
			break;
		}
	}

	MyPlatform::KillProcessFromName(_T("Setup.exe"));
	return 0;
}

class AutoCloseProcess
{
public:
	AutoCloseProcess()
	{
		m_bExit = false;

		m_threadClose.swap(std::thread([](AutoCloseProcess* pACP)->void {
		
			pACP->ThreadCloseProcess();

		}, this));
	}
	~AutoCloseProcess()
	{
		m_bExit = true;
		m_threadClose.join();
	}

	int AddProcess(const TCHAR* szProcessName)
	{
		m_mutex.lock();
		m_vCloseProcess.push_back(szProcessName);
		m_mutex.unlock();
		return 0;
	}

private:
	int ThreadCloseProcess()
	{
		std::vector<MyPlatform::tstring> vCloseProcess;

		while (!m_bExit)
		{
			Sleep(1000);
			vCloseProcess.clear();

			m_mutex.lock();
			vCloseProcess = m_vCloseProcess;
			m_mutex.unlock();

			for (size_t index = 0; index < vCloseProcess.size(); index++)
			{
				MyPlatform::KillProcessFromName(vCloseProcess[index].c_str());
			}
			
			
			
			
			
			
			
			
			
		}
		return 0;
	}

private:
	bool				m_bExit;
	std::thread			m_threadClose;
	std::mutex			m_mutex;
	std::vector<MyPlatform::tstring>	m_vCloseProcess;
};

int SetupSoftware()
{
	AutoCloseProcess closeProcess;

	printf("Setup Starts.\r\n");
	InstallWinrar winrar;

	printf("winrar install...\r\n");
	winrar.Install();
	Sleep(1000 * 3);
	printf("winrar install finished.\r\n");
	InstallOffice office;

	printf("office install...\r\n");

	office.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("EXCEL.EXE"));
	closeProcess.AddProcess(_T("POWERPNT.EXE"));
	closeProcess.AddProcess(_T("WINWORD.EXE"));
	closeProcess.AddProcess(_T("OUTLOOK.EXE"));
	printf("office install finished.\r\n");

	InstallAcrobat acrobat;
	printf("acrobat install...\r\n");

	acrobat.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("Acrobat.exe"));
	printf("acrobat install finished.\r\n");

	InstallPr pr;
	printf("pr install...\r\n");

	pr.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("Adobe Premiere Pro.exe"));
	printf("pr install finished.\r\n");

	InstallAE ae;
	printf("ae install...\r\n");

	ae.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("AfterFX.exe"));
	printf("ae install finished.\r\n");

	InstallPS ps;
	printf("ps install...\r\n");

	ps.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("Photoshop.exe"));
	printf("ps install finished.\r\n");

	InstallDMax dmax;
	printf("3dmax install...\r\n");

	dmax.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("3dsmax.exe"));
	printf("3dmax install finished.\r\n");

	InstallCAD cad;
	printf("cad install...\r\n");

	cad.Install();
	Sleep(1000 * 3);
	closeProcess.AddProcess(_T("acad.exe"));
	printf("cad install finished.\r\n");

	printf("安装完毕， 准备重启。\r\n");
	Sleep(1000 * 3);
	return 0;
}
const TCHAR* gc_szParamSet = _T("-Set");
const TCHAR* gc_szValueNameAutoRun = _T("AutoRunSetup");

void UpdateAdobeStartupSettings()
{
	DWORD Ret = WriteInRegistry(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"),
		_T("Adobe Creative Cloud")
		, "");
}

inline void SetAutoRun()
{
	const TCHAR* c_szRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	TCHAR szFilePath[MAX_PATH] = { 0 };
	TCHAR szStartFP[_MAX_PATH * 2] = { 0 };
	HKEY hKey = NULL;

	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	_stprintf_s(szStartFP, _T("\"%s\" %s"), szFilePath, gc_szParamSet);
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, c_szRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		RegSetValueEx(hKey, gc_szValueNameAutoRun, 0, REG_SZ, (LPBYTE)szStartFP, (_tcslen(szStartFP) + 1) * sizeof(TCHAR));
	}
	RegCloseKey(hKey);
	
	WriteInRegistry(
		HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		_T("EnableLUA"), 0);
	WriteInRegistry(
		HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		_T("ConsentPromptBehaviorAdmin"), 0);
	UpdateAdobeStartupSettings();
}

inline void ReSetAutoRun()
{
	HKEY hKey = NULL;
	const TCHAR* c_szRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, c_szRegPath, 0, KEY_ALL_ACCESS , &hKey) == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, gc_szValueNameAutoRun);
	}
	RegCloseKey(hKey);
}


void ChangeDPI()
{
	 DWORD Ret = WriteInRegistry(HKEY_CURRENT_USER,
		_T("Control Panel\\Desktop"),
		_T("LogPixels")
		, 96);
	
	Ret = WriteInRegistry(HKEY_CURRENT_USER,
		_T("Control Panel\\Desktop"),
		_T("Win8DpiScaling")
		,1 );

	Ret = WriteInRegistry(HKEY_CURRENT_USER,
		_T("Control Panel\\Desktop"),
		_T("DesktopDPIOverride")
		, 0);
}

void SetResolution(int w, int h)
{
	long RetVal = 0;

	DEVMODE newMode{};
	EnumDisplaySettings(NULL,
		ENUM_CURRENT_SETTINGS,
		&newMode);

	newMode.dmSize = sizeof(DEVMODE);

	//newMode.dmPelsWidth = w;
	if (newMode.dmPelsHeight < h)
	{
		newMode.dmPelsHeight = h;
		newMode.dmFields =  /*DM_PELSWIDTH |*/  DM_PELSHEIGHT;

		int result = ChangeDisplaySettings(&newMode, 0);
		if (result == DISP_CHANGE_SUCCESSFUL)
		{
			printf("Set new resolution Succeeded.\r\n");
			// Inspecting the new mode
			std::ostringstream stream;
			stream << newMode.dmPelsWidth << "#"
				<< newMode.dmPelsHeight << "#"
				<< newMode.dmBitsPerPel << "#"
				<< newMode.dmScale << "#"
				<< std::endl;
			std::string result = stream.str();
		}
		else if (result == DISP_CHANGE_BADMODE)
			printf("resolution Mode not supported.\r\n");
		else if (result == DISP_CHANGE_RESTART)
			printf("Restart required.\r\n");
		else
			printf("Failed. Error code = {0}\r\n", result);
	}
	else
		printf("No need to change resolution.\r\n");
}
 
int main(int argc, char** argv)
{
	bool bExitThread = false;
	std::thread threadMoveCursor([](bool* pbExitThread)->void {
		bool bAdd = true;
		while (true)
		{
			if (*pbExitThread)
			{
				break;
			}
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, false, NULL, SPIF_SENDWININICHANGE);
			SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
// 			POINT pointCursor = { 0 };
// 			::GetCursorPos(&pointCursor);
// 
// 			if (bAdd)
// 			{
// 				pointCursor.x += 1;
// 			}
// 			else
// 			{
// 				pointCursor.x -= 1;
// 			}
// 
// 			SetCursorPos(pointCursor.x, pointCursor.y);
// 			bAdd = !bAdd;
			Sleep(1000 * 30);
		}
		//SetThreadExecutionState(ES_CONTINUOUS);
	}, &bExitThread);

	CloseNetAdapter();
	if (argc == 1)
	{
		printf("Change DPI starts. \r\n");

		ChangeDPI();
		printf("Change DPI finishes. \r\n");

		printf("Change resolution starts. \r\n");

		SetResolution(1920, 1080);
		printf("Change resolution finishes. \r\n");

 		SetupSoftware();

		Sleep(1000 * 10);

		SetAutoRun();
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		// Get a token for this process.
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			::MessageBox(NULL, _T("重启计算机失败，请手动重启\r\n"), _T("提示"), MB_OK);
			return -1;
		}
		// Get the LUID for the shutdown privilege.
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1; // one privilege to set
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		// Get the shutdown privilege for this process.
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
		if (GetLastError() != ERROR_SUCCESS)
		{
			::MessageBox(NULL, _T("重启计算机失败，请手动重启\r\n"), _T("提示"), MB_OK);
			return -2;
		}

		// Shut down the system and force all applications to close.
		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0); //重启只需要把EWX_SHUTDOWN改为EWX_REBOOT
		exit(0);
	}
	else if (argc >= 2 && 0 == strcmp(MyPlatform::TcharToChar(gc_szParamSet).c_str(), argv[1]))
	{
		//Sleep(1000 * 30);
 		printf("开始初始化设置\r\n");
 		//::MessageBox(NULL, _T("测试成功"), _T(""), MB_OK);
		ReSetAutoRun();
 
		OutlookSetting outlookSet;
		printf("outlook setup...\r\n");

		outlookSet.Set();
		printf("outlook setup finished.\r\n");

		CADSetting cadSet;
		printf("cad setup...\r\n");

		cadSet.Set();
		printf("cad setup finished.\r\n");

		DMaxSetting dmaxSet;
		printf("3dmax setup...\r\n");

 		dmaxSet.Set();
		printf("3dmax setup finished.\r\n");

		printf("设置完毕\r\n");
	}
	bExitThread = true;
	threadMoveCursor.join();
	return 0;
}

