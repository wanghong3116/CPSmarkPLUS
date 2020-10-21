#include "stdafx.h"
#include "MarkObject.h"
#include "ChromeMark.h"
#include "OfficeRegisterWindow.h"
#include "Chrome_Opids.h"
#include "InformationTrace.h"
#include <future>

std::tstring m_GoogleChromeApplication, m_WebPath;

CChromeMark::CChromeMark()
{
	LogInfo(_T("Invoke CChromeMark().."));

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Chrome-网页浏览"));
	m_eMarkType = E_MARK_NET;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	SetMD5PassWd(std::string("44ecd4d79f4a6c7a91ec7180e5f8f75d")); //latest md5
	//SetMD5PassWd(std::string("8413faebe4173fd5e22ce849b0220dea"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\GoogleChrome.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

CChromeMark::~CChromeMark()
{
	LogInfo(_T("Invoke ~CChromeMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\GoogleChrome\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CChromeMark::init()
{
	destroy();
	SleepAndRecord(500);

	CoInitialize(NULL);
	

	{
		TCHAR szTemp[_MAX_PATH] = { 0 };
		MyPlatform::GetFullPathUnderExe(_T("Resources\\GoogleChrome\\Chrome\\Application\\17.0.963.83\\chrome.exe"), szTemp);
		m_GoogleChromeApplication = szTemp;
		MyPlatform::GetFullPathUnderExe(_T("Resources\\GoogleChrome\\jd.com\\"), szTemp);
		m_WebPath = szTemp;
	}
	if (0 != _taccess(m_GoogleChromeApplication.c_str(), 0))
	{
		LogError({_T("Chrome没有找到")});
		return -3;
	}
	if (0 != _taccess(m_WebPath.c_str(), 0))
	{
		LogError({ _T("要浏览的网页没有找到") });
		return -4;
	}

	return BENCH_MARK_OK;
}

int CChromeMark::run()
{
	 
	if (BENCH_MARK_OK != ExecuteChrome())
	{
		return -2;
	}
	return BENCH_MARK_OK;
}

int CChromeMark::destroy()
{
	LogInfo(_T("Invoke CChromeMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\GoogleChrome\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	CoUninitialize();
	//MyPlatform::KillProcessFromName(_T("OUTLOOK.EXE"));
	SleepAndRecord(500);
	//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
	SleepAndRecord(500);
	//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	SleepAndRecord(500);
	//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	SleepAndRecord(500);
	//MyPlatform::KillProcessFromName(_T("chrome.exe"));
	SleepAndRecord(500);
	return BENCH_MARK_OK;
}


ChromeHelper* CChromeMark::CreateHelper(tstring id)
{
	ChromeHelper* chromeHelper = new ChromeHelper;
	if (NULL == chromeHelper)
	{
		LogError({ _T("网络测试：内存溢出") });
		return NULL;
	}
	
	std::tstring pageAddress;
	pageAddress = m_WebPath + TEXT("item.jd.com\\") + id;
	chromeHelper->setApplication(m_GoogleChromeApplication.c_str());
	chromeHelper->setPageAddress(pageAddress.c_str());
	chromeHelper->setParameters(TEXT("--start-maximized --process-per-tab --incognito --new-window"));
	
	return chromeHelper;
}

Void CChromeMark::StartUp(ChromeHelper* chromeHelper, int nth)
{
	TakeMeasurement(new CCounterContent(CChrome_Opids::DESC_TYPE_MARKER, CChrome_Opids::DESC_OPEN_APPLICATION + MyPlatform::Int64ToStringW(nth)),
		[&]()->int {
	std::tstring lptstrCommands;
	lptstrCommands += TEXT("\"");
	lptstrCommands += chromeHelper->m_Application.c_str();
	future<void> fut = async(launch::async, [=]() {chromeHelper->m_Process.Shell(lptstrCommands.c_str(), 0, true); });
	fut.get();
	return BENCH_MARK_OK; });

	/*MTStr lptstrWindowDesc;
	lptstrWindowDesc.Format(TEXT("[classregexp:Chrome_WidgetWin_\\d+;pid:%d]"), m_Process.m_ProcessInformation.dwProcessId);

	while(!m_Window.Exists(lptstrWindowDesc.C_Str(), 0)){
	SleepAndRecord(100);
	};*/
};
void CChromeMark::GetToPage(ChromeHelper* chromeHelper, tstring url)
{
	TakeMeasurement(new CCounterContent(CChrome_Opids::DESC_TYPE_MARKER, CChrome_Opids::DESC_OPEN_DOCUMENT + url),
		[&]()->int {
	std::tstring  lpstrAddress;
	lpstrAddress += TEXT("\"");
	lpstrAddress += chromeHelper->m_Application.c_str();
	lpstrAddress += TEXT("\" ");
	lpstrAddress += TEXT("\"");
	lpstrAddress += chromeHelper->m_PageAddress.c_str();
	lpstrAddress += TEXT("\" ");
	lpstrAddress += chromeHelper->m_Parameters.c_str();
	future<void> fut = async(launch::async, [=]() {chromeHelper->m_Process.Shell(lpstrAddress.c_str(), 0, true); });
	fut.get();
	return BENCH_MARK_OK; 
});
}

int CChromeMark::ExecuteChrome()
{
	const TCHAR* pszHtmlFP[] = {
		TEXT("863619.html"),
		MyPlatform::CInformationTrace::IsInTestMode == 1 ? NULL : TEXT("972747.html"),
		MyPlatform::CInformationTrace::IsInTestMode == 1 ? NULL : TEXT("975632.html"),
	};
	int nFileCount = 0;
	nFileCount = MyPlatform::CInformationTrace::IsInTestMode == 1 ? 1 : _ARRAYSIZE(pszHtmlFP);
	nFileCount *= 10;
	for (int nHtmlIndex = 0; nHtmlIndex < nFileCount; nHtmlIndex++)
	{
		MyPlatform::KillProcessFromName(_T("chrome.exe"));
		ChromeHelper *chromeHelper = NULL;
		
		LogInfo(_T("Chrome:正在开启浏览器。"));
		StartUp(chromeHelper = CreateHelper(_T("")), nHtmlIndex + 1);
		SleepAndRecord(4 * dwTimerNode3);
		chromeHelper->Terminate();
		SleepAndRecord(5 * dwTimerNode3);
		SleepAndRecord(dwTimerNode3);
		chromeHelper->ShutDown();
		MyPlatform::KillProcessFromName(_T("chrome.exe"));
		delete chromeHelper;
		HWND hLast = NULL;
		CRect rectWindow;

		std::wstring idx = MyPlatform::Int64ToStringW(nHtmlIndex+1);
		int pageIndex = (nHtmlIndex % 3);
		std::tstring pageDesc = idx + _T("_") + pszHtmlFP[pageIndex];
		LogInfo({ _T("Chrome:正在开启浏览器并打开"), PathFindFileName(pszHtmlFP[pageIndex]) });
		GetToPage(chromeHelper = CreateHelper(pszHtmlFP[pageIndex]), pageDesc);
		SleepAndRecord(4 * dwTimerNode3);SleepAndRecord(4 * dwTimerNode3);
		while (!AU3_WinWait(L"[REGEXPCLASS:Chrome_WidgetWin_\\d+;ACTIVE]", L"", 0)) {
			//
		};
		SleepAndRecord(2 * dwTimerNode3);
		hLast = AU3_WinGetHandle(L"[LAST]", L"");
		chromeHelper->m_Window = hLast;
		SetWindowPos(hLast, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		SleepAndRecord(dwTimerNode2);
		SetActiveWindow(hLast);
		GetWindowRect(hLast, rectWindow);
		SetCursorPos((rectWindow.left + rectWindow.right) / 2, (rectWindow.top + rectWindow.bottom) / 2);
		SleepAndRecord(dwTimerNode2);

		for (int nDownIndex = 0; nDownIndex < 10; nDownIndex++)
		{
			LogInfo(_T("Chrome:向下浏览"));
			chromeHelper->ScrollDown();
			SleepAndRecord(dwTimerNode3);
		}

		for (int nUpIndex = 0; nUpIndex < 5; nUpIndex++)
		{
			LogInfo(_T("Chrome:向上浏览"));
			chromeHelper->ScrollUp();
			SleepAndRecord(dwTimerNode3);
		}
		
		/*
		CString sDocTitle;
		DWORD dwWaitTime = 0;

		while(1)
		{
			GetWindowText(hLast, sDocTitle.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
			if (sDocTitle.Find(_T("finish")) >= 0)
			{
				sDocTitle.ReleaseBuffer();
				break;
			}

			sDocTitle.ReleaseBuffer();
			Sleep(dwTimerNode);
			dwWaitTime += 100;

			if (dwWaitTime >= (dwTimerNode3 * 60 * 5))
			{
				sDocTitle.ReleaseBuffer();
				LogError(_T("要浏览的网页没有找到"));
				PostMessage(hLast, WM_CLOSE, 0, 0);
				delete chromeHelpers;

				return -1;
			}
		};
		*/

		SleepAndRecord(dwTimerNode3);
		chromeHelper->Terminate(); 
		SleepAndRecord(dwTimerNode3);
		chromeHelper->ShutDown();
		//PostMessage(hLast, WM_CLOSE, 0, 0);
		SleepAndRecord(dwTimerNode1); 
		delete chromeHelper;
		SleepAndRecord(10 * dwTimerNode3);
	}
	
	return BENCH_MARK_OK;
}

