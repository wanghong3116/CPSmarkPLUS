#include "stdafx.h"
#include "MarkObject.h"
#include "AEMark.h"
#include "AdobeRegisterWindow.h"
#include "AE_Opids.h"

CAEMark::CAEMark()
{
	LogInfo(_T("Invoke CAEMark().."));

	_tcscpy_s(m_szDescript, _T("AE-视频渲染"));
	m_eMarkType = E_MARK_AE;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Calculation;

	TCHAR szZipDir[_MAX_PATH] = { 0 };
	SetMD5PassWd(std::string("9636d3cdd0cf42df918c883da38bb50c")); //latest md5
	//SetMD5PassWd(std::string("dc20b0e0608cb9dd59eaf8bf3de216b8"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

CAEMark::~CAEMark()
{
	LogInfo(_T("Invoke ~CAEMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CAEMark::init()
{
	MyPlatform::GetAppPathFromRegAppPath(m_szAEFP, _ARRAYSIZE(m_szAEFP), _T("AfterFX.exe"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE\\runscriptOpen.jsx"), m_szScriptOpenFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE\\1.avi"), m_szDstVideoFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE\\Fireworks\\render.aep"), m_szProjectFP);

	destroy();
	if (0 != _taccess(m_szAEFP, 0))
	{
		return -1;
	}
	
	if (BENCH_MARK_OK != writeScript()
		|| BENCH_MARK_OK != copyPlugin())
	{
		return -2;
	}

	if (BENCH_MARK_OK != startAEAndCloseRegister())
	{
		return -3;
	}
	DeleteFile(m_szDstVideoFP);
	return BENCH_MARK_OK;
}

int CAEMark::RunAEMark()
{
	TCHAR szCmd[_MAX_PATH * 5] = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	HWND hAlertWnd = NULL;
	DWORD MAX_WAIT_RENDER_TIME = 10 * 60 * 1000;
	int nMaxMinute = 15;

	TakeMeasurement(new CCounterContent(CAE_Opids::DESC_TYPE_MARKER, CAE_Opids::DESC_RUN),
		[&]()->int {
	/// 打开工程
	if (BENCH_MARK_OK != runOpenProject())
	{
		return -1;
	}

	/// 开始渲染
	if (BENCH_MARK_OK != runStarRender())
	{
		return -2;
	}

	/// 等待渲染结束
	if (BENCH_MARK_OK != runWaitRenderOver())
	{
		return -3;
	}
	return BENCH_MARK_OK;
	});
	MyPlatform::KillProcessFromName(_T("AfterFX.exe"));
	return BENCH_MARK_OK;
}



int CAEMark::run()
{
	return RunAEMark();
 }

int CAEMark::destroy()
{
	LogInfo(_T("Invoke CAEMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	MyPlatform::KillProcessFromName(_T("AfterFX.exe"));
	SleepAndRecord(500);
	DeleteFile(m_szDstVideoFP);
	DeleteFile(m_szScriptOpenFP);

	SleepAndRecord(500);
	TCHAR szCacheDir[_MAX_PATH] = { 0 };
	GetTempPath(_MAX_PATH, szCacheDir);
	_tcscat_s(szCacheDir, _MAX_PATH - _tcslen(szCacheDir), _T("\\Adobe\\After Effects"));
	MyPlatform::DeleteDir(szCacheDir);
	return BENCH_MARK_OK;
}

int CAEMark::writeScript()
{
	FILE* fp = NULL;
	TCHAR szProjectFP[_MAX_PATH * 2] = { 0 };
	TCHAR szVideoFP[_MAX_PATH * 2] = { 0 };

	
	_tcscpy_s(szVideoFP, m_szDstVideoFP);
	_tcscpy_s(szProjectFP, m_szProjectFP);
	if (0 != _taccess(szProjectFP, 0))
	{
		return -1;
	}
	changePathDoubleSplit(szVideoFP, _ARRAYSIZE(szVideoFP));
	changePathDoubleSplit(szProjectFP, _ARRAYSIZE(szProjectFP));

	
	CString strContent;
	strContent.Format(_T("var my_file = new File(\"%s\");\r\n")\
		_T("app.exitAfterLaunchAndEval = true;\r\n")\
		_T("app.project.close(CloseOptions.DO_NOT_SAVE_CHANGES);\r\n")\
		_T("app.open(my_file);\r\n")\
		_T("var comp;\r\n")\
		_T("for (i = 1; i <= app.project.items.length; i++) {\r\n")\
		_T("	if (app.project.item(i) instanceof CompItem) {\r\n")\
		_T("		comp = app.project.item(i);\r\n")\
		_T("		break;\r\n")\
		_T("	}\r\n")\
		_T("}\r\n")\
		_T("\r\n")\
		_T("for (i = 1; i <= app.project.renderQueue.items.length; i++) {\r\n")\
		_T("	app.project.renderQueue.item(i).remove();\r\n")\
		_T("\r\n")\
		_T("}\r\n")\
		_T("var rq_item = app.project.renderQueue.items.add(comp);\r\n")\
		_T("rq_item.outputModule(1).file = File(\"%s\");\r\n")\
		_T("rq_item.render = true;\r\n")\
		_T("rq_item.onStatusChanged = function() {};\r\n")\
		_T("app.project.renderQueue.showWindow(true);\r\n")\
	, szProjectFP, szVideoFP);
	
	if (BENCH_MARK_OK != writeUTF8File(m_szScriptOpenFP, strContent))
	{
		return -2;
	}

// 	strContent.Format(_T("if (app.project.renderQueue.item(1).status == RQItemStatus.DONE)\r\n")\
// 		_T("{\r\n")\
// 		_T("	//alert(\"over\");\r\n")\
// 		_T("app.project.close(CloseOptions.DO_NOT_SAVE_CHANGES);\r\n")\
// 		_T("app.exitAfterLaunchAndEval = true;\r\n")\
// 		_T("app.quit();\r\n")\
// 		_T("\r\n")\
// 		_T("}\r\n")\
// 	);
// 	if (BENCH_MARK_OK != writeUTF8File(m_szScriptCloseFP, strContent))
// 	{
// 		return -3;
// 	}

	return BENCH_MARK_OK;
}


int CAEMark::writeUTF8File(const TCHAR* szFilePath, const TCHAR* szText)
{
	FILE* fp = NULL;
	unsigned char szHeader[3] = { 0 };

	_tfopen_s(&fp, szFilePath, _T("wb"));
	if (NULL == fp)
	{
		return -1;
	}
	
	FILE_UTF8_HEADER(szHeader);
	fwrite(szHeader, sizeof(char), _ARRAYSIZE(szHeader), fp);

	std::string& rstrUtf8Content = MyPlatform::TcharToUtf8(szText);
	fwrite(rstrUtf8Content.c_str(), 1, rstrUtf8Content.size(), fp);

	fclose(fp);
	return BENCH_MARK_OK;
}

int CAEMark::startAEAndCloseRegister()
{
	TCHAR szCmd[_MAX_PATH * 5] = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	AdobeRegisterWindow wndRegister(this);
	wndRegister.RegisterWindowHandler([&]()->int {
		_stprintf_s(szCmd, _T("\"%s\" \"%s\" -ro \"%s\""), m_szAEFP, m_szProjectFP, m_szScriptOpenFP);
		if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			LogError({ _T("AE创建进程失败") });
			MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
			return -1;
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return BENCH_MARK_OK;
	}, 30);

	SleepAndRecord(30 * 1000);
	return BENCH_MARK_OK;
}

int CAEMark::copyPlugin()
{
	TCHAR szSrcFile[_MAX_PATH] = { 0 };
	TCHAR szDstFile[_MAX_PATH] = { 0 };

	_stprintf_s(szDstFile, _MAX_PATH, _T("%s\\Plug-ins\\Particular.aex"), MyPlatform::GetFilePathDir(m_szAEFP).c_str());
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AE\\plugin\\Particular.aex"), szSrcFile);
	
	if (0 != _taccess(szSrcFile, 0))
	{
		LogError({ _T("AE插件丢失") });
		return -1;
	}
	CopyFile(szSrcFile, szDstFile, FALSE);

	return BENCH_MARK_OK;
}

int CAEMark::runOpenProject()
{
	TCHAR szCmd[_MAX_PATH * 5] = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	
	SleepAndRecord(500);
	LogInfo(_T("AE::执行脚本，渲染中...."));
	_stprintf_s(szCmd, _T("\"%s\" -ro %s"), m_szAEFP, m_szScriptOpenFP);
	if (FALSE == CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		LogError({ _T("AE创建进程失败") });
		MyPlatform::MyTrace(L0, _T("CMarkObject::runProcessAndWait CreateProcess failed, errno=[%d]"), GetLastError());
		return -1;
	}
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	return BENCH_MARK_OK;
}

int CAEMark::runStarRender()
{
	typedef struct _FindRenderWndParam
	{
		int nScreenW;
		HWND* phRenderWnd;
	}FindRenderWndParam;
	const TCHAR *szCalssName = _T("AE_CApplication_16.0");
	CString strWindowNameWithExt;
	CString strWindowNameNoExt;
	HWND hRenderWnd = NULL;
	int nScreenW = 0;
	DWORD dwTime = 0;
	FindRenderWndParam frwp = { 0 };

	nScreenW = GetSystemMetrics(SM_CXFULLSCREEN);
	LogInfo(_T("查找AE窗口"));
	strWindowNameWithExt.Format(_T("Adobe After Effects CC 2019 - %s"), m_szProjectFP);
	strWindowNameNoExt = strWindowNameWithExt.Left(strWindowNameWithExt.ReverseFind('.'));

	while (dwTime < 2 * 60 * 1000)
	{
		std::vector<HWND> vhWnd;
		MyPlatform::FindWindowWithClassNameAndWindowName(szCalssName, strWindowNameNoExt, true, true, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (m_hAEMainWnd = vhWnd[0]))
		{
			break;
		}
		dwTime += 1;
		Sleep(1);
	}
	if (NULL == m_hAEMainWnd)
	{
		LogError({ _T("AEMark：没有找到AE窗口") });
		return -1;
	}

	SendMessage(m_hAEMainWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	//SetWindowPos(m_hAEMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	::GetFirstChild(m_hAEMainWnd);
	frwp.phRenderWnd = &hRenderWnd;
	frwp.nScreenW = nScreenW;
	EnumChildWindows(m_hAEMainWnd, [](HWND hChild, LPARAM lParam)->BOOL {
		RECT rectChild = { 0 };
		FindRenderWndParam* pFRWParam = (FindRenderWndParam*)lParam;

		GetClientRect(hChild, &rectChild);
		if (pFRWParam->nScreenW == rectChild.right && rectChild.bottom > 100)
		{
			*pFRWParam->phRenderWnd = hChild;
			return FALSE;
		}
		return TRUE;
	}, (LPARAM)&frwp);
	if (NULL == hRenderWnd)
	{
		LogError({ _T("AEMark：没有找到AE Render窗口") });
		return -2;
	}

	CRect rectSelect;
	CPoint pointClick;
	::GetWindowRect(hRenderWnd, rectSelect);
	pointClick.x = rectSelect.right - 40;
	pointClick.y = rectSelect.top + 70;
	SetCursorPos(pointClick.x, pointClick.y);
	SleepAndRecord(500);
	doLButton(pointClick.x, pointClick.y);
	SleepAndRecord(500);
	//SetWindowPos(m_hAEMainWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return BENCH_MARK_OK;
}

int CAEMark::runWaitRenderOver()
{
	HMENU hMainMenu = NULL, hFileMenu = NULL;
	DWORD MAX_WAIT_RENDER_TIME = 10 * 60 * 1000;
	int nMaxMinute = 15;
	bool bExit = false;
	/// 关闭弹出窗口
	std::thread threadCloseErrorWindow([=](bool* pbExit)->void {		
		HWND hWndError = NULL;
		CRect rectWindow;
		CPoint pointClick;
		while (!*pbExit)
		{
			Sleep(1);
			if (NULL == (hWndError = FindWindow(_T("#32770"), _T("After Effects"))))
			{
				continue;
			}

			SetWindowPos(hWndError, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			GetWindowRect(hWndError, rectWindow);
			pointClick.x = rectWindow.right - 60;
			pointClick.y = rectWindow.bottom - 30;
			SetCursorPos(pointClick.x, pointClick.y);
			SleepAndRecord(1000);
			doLButton(pointClick.x, pointClick.y);
			SleepAndRecord(3000);
		}

	}, &bExit);


	hMainMenu = ::GetMenu(m_hAEMainWnd);
	hFileMenu = ::GetSubMenu(hMainMenu, 1);
	nMaxMinute = g_ini.GetInt(CFG_MARK, CFG_MARK_AE_RENDER_MAX_TIME, 15);
	MAX_WAIT_RENDER_TIME = nMaxMinute * 60 * 1000;
	DWORD dwUseTime = 0;
	for (; dwUseTime < MAX_WAIT_RENDER_TIME; dwUseTime += 10)
	{
		UINT nMenuState = ::GetMenuState(hFileMenu, 0, MF_BYPOSITION);
		if (MF_ENABLED == nMenuState)
		{
			break;
		}
		Sleep(10);
	}
	if (dwUseTime >= MAX_WAIT_RENDER_TIME)
	{
		LogError({ _T("AE无法找到结束窗口") });
		MyPlatform::KillProcessFromName(_T("AfterFX.exe"));
		return -2;
	}

	bExit = true;
	if (threadCloseErrorWindow.joinable())
	{
		threadCloseErrorWindow.join();
	}
	return BENCH_MARK_OK;
}
