#include "stdafx.h"
#include "MarkObject.h"
#include "PremiereMark.h"
#include "AdobeRegisterWindow.h"
#include "Premiere_Opids.h"

#include "windows.h"
#include "Lmcons.h"

#include <fstream>
#include <iostream>

int get_file_size(std::string filename, LPARAM lParam) // path to file
{
	FILE *filepoint;
	errno_t err;
	CPremiereMark* pThis = (CPremiereMark*)lParam;
	pThis->LogInfo(_T("get_file_size start."));
	FILE *p_file = NULL;

	err = fopen_s(&filepoint, filename.c_str(), "rb");

	if (err == 0) //no err
	{
		fseek(filepoint, 0, SEEK_END);
		int size = ftell(filepoint);
		rewind(filepoint);

		pThis->LogInfo(_T("get_file_size end. size > 0"));

		if (size < (pThis->EXPECTED_RENDER_FILESIZE_MB - ((pThis->EXPECTED_RENDER_FILESIZE_MB) / 20)) * 1000000)
			pThis->LogInfo({ _T("OutputVideoFile Size (get_file_size) Is BELOW Acceptable Minimum MB Std. Dev. Range: "), MyPlatform::Int64ToStringW(size) }); //Error
		else
			pThis->LogInfo({ _T("OutputVideoFile Size (get_file_size) Is Above Acceptable Minimum MB Std. Dev. Range: "), MyPlatform::Int64ToStringW(size) }); //Success
	}
	else
		pThis->LogInfo(_T("get_file_size end. size = 0"));
	fclose(filepoint);
	return 0;
}

CPremiereMark::CPremiereMark()
{
	LogInfo(_T("Invoke CPremiereMark().."));

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Premiere-视频剪辑"));
	m_eMarkType = E_MARK_PREMIERE;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Calculation;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("003bf56158ce680f2288443b1aa30c8a")); //latest md5
	//SetMD5PassWd(std::string("1f18a5da668602b1bce4fc99d6a3ff9f"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

CPremiereMark::~CPremiereMark()
{
	LogInfo(_T("Invoke ~CPremiereMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CPremiereMark::init()
{
	LogInfo(_T("init start."));

	if (PLATFORM_FUNC_NOERROR != MyPlatform::GetAppPathFromRegAppPath(m_szPremiereFP, _ARRAYSIZE(m_szPremiereFP), _T("Adobe Premiere Pro.exe")))
	{
		LogError({ _T("没有正确安装Premiere") });
		return -1;
	}
	LogInfo(_T("found Premiere exe path"));

	_tcscpy_s(m_szPremiereFP, _ARRAYSIZE(m_szPremiereFP), m_szPremiereFP + 1);
	m_szPremiereFP[_tcslen(m_szPremiereFP) - 1] = '\0';
	if (0 != _taccess(m_szPremiereFP, 0))
	{
		LogError({ _T("没有正确安装Premiere(installed incorrectly)") });
		return -1;
	}

	if (BENCH_MARK_OK != initDependFile())
	{
		return -2;
	}
	LogInfo(_T("finish init dependent files."));

	// 	if (BENCH_MARK_OK != changeProjectVideoPath())
	// 	{
	// 		return -3;
	// 	}
		/// 删除渲染中间文件夹
		//deleteIntermediateFile();

	if (BENCH_MARK_OK != startPremiere())
	{
		LogError({ _T("startPremiere failed.") });
		return -4;
	}

	
	LogInfo(_T("init end."));
	return BENCH_MARK_OK;
}

int CPremiereMark::run()
{
	LogInfo(_T("run starts"));
	if (BENCH_MARK_OK != SendRenderOrder())
	{
		LogInfo(_T("SendRenderOrder failed."));
		return -2;
	}
	LogInfo(_T("run ends."));

	return BENCH_MARK_OK;
}

int CPremiereMark::destroy()
{
	LogInfo(_T("destroy starts."));

	LogInfo(_T("Invoke CPremiereMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	AU3_Sleep(2000);

	MyPlatform::KillProcessFromName(_T("Adobe Premiere Pro.exe"));
	/// 删除渲染中间文件夹 
	deleteIntermediateFiles();

	LogInfo(_T("destroy ends."));

	/**/
	LogInfo(_T("start check OutputVideoFile Size (get_file_size)."));

	if (0 == _taccess(m_outputVideoNameFoundFromVecList.c_str(), 0)) //found current output Video File after render completed
	{
		LogInfo({ _T("found rendered output video file: "), m_outputVideoNameFoundFromVecList });

		tstring destFile = _T("C:\\bay3.mp4");
		tstring srcFile = m_outputVideoNameFoundFromVecList;

		CopyFile(srcFile.c_str(), destFile.c_str(), FALSE); //copy found current rendered output Video File to temp dir.
		LogInfo({ _T("Copy current rendered output video file from src to dest --"), srcFile.c_str(), destFile.c_str() });

		string str = "C:\\bay3.mp4";
		get_file_size(str, (LPARAM)this);

		LogInfo({ _T("deleting backup rendered output video File --"), destFile });
		DeleteFile(destFile.c_str());

		LogInfo(_T("finish check OutputVideoFile Size (get_file_size)."));
	}
	else
		LogInfo({ _T("NOT found rendered output video file: "), m_outputVideoNameFoundFromVecList });

	LogInfo(_T("destroy ends."));

	return BENCH_MARK_OK;
}

int CPremiereMark::initDependFile()
{
	LogInfo(_T("Prepare preset file"));

	TCHAR username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName((TCHAR*)username, &username_len);

	tstring prefFile = _T("C:\\Users\\");
	prefFile.append(username).append(_T("\\Documents\\Adobe\\Premiere Pro\\13.0\\Profile-TEST\\Adobe Premiere Pro Prefs"));
	DeleteFile(prefFile.c_str());

	MyPlatform::DeleteDir(L"C:\\Users\\TEST\\AppData\\Roaming\\Adobe\\Common\\Media Cache Files");

	TCHAR szSrcDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr"), szSrcDir);

	
	tstring prefDest = _T("C:\\Users\\");
	prefDest.append(username).append(_T("\\Documents\\Adobe\\Adobe Media Encoder\\13.0\\"));
	prefDest.append(_T("AMEPrefs.xml"));
	tstring prefSrc = tstring(szSrcDir).append(_T("\\")).append(_T("AMEPrefs.xml"));
	CopyFile(prefSrc.c_str(), prefDest.c_str(), FALSE);

	tstring destFile = _T("C:\\Users\\"); //user dir. filepath
	destFile.append(username).append(_T("\\Documents\\Adobe\\Adobe Media Encoder\\13.0\\Presets\\"));
	MyPlatform::CreateFullDirectory (destFile.c_str());
	destFile.append(PresetFileName); //user dir. filepath

	tstring srcFile = tstring(szSrcDir).append(_T("\\")).append(PresetFileName);

	TCHAR szExeFP[_MAX_PATH] = { 0 };
	MyPlatform::GetAppPathFromRegAppPath(szExeFP, _ARRAYSIZE(szExeFP), _T("Adobe Premiere Pro.EXE")); //install dir. filepath
	tstring destFile2 = tstring(szExeFP).substr(1, tstring(szExeFP).find_last_of('\\'));
	destFile2.append(_T("Presets\\")).append(PresetFileName); //install dir. filepath

	CopyFile (srcFile.c_str(), destFile.c_str(), FALSE);

	LogInfo({ _T("Copy preset file from src to 1st dest --"), srcFile.c_str(), destFile.c_str() }); //copy to user dir. filepath

	CopyFile(srcFile.c_str(), destFile2.c_str(), FALSE);

	LogInfo({ _T("Copy preset file from src to 2nd dest --"), srcFile.c_str(), destFile2.c_str() }); //copy to install dir. filepath

	LogInfo(_T("Prepare project files"));

	TCHAR szBackupProjectFP[_MAX_PATH] = { 0 };
	TCHAR szBackupEmptyProjectFP[_MAX_PATH] = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\run"), m_szRunDir);
	LogInfo({ _T("making run directory --"), m_szRunDir });
	MyPlatform::CreateFullDirectory(m_szRunDir);

	LogInfo(_T("Prepare output video files list"));  //list of possible output video files

	m_outputVideoNameVec.push_back(_T("bay3.mp4"));
	m_outputVideoNameVec.push_back(_T("trees3.mp4"));
	m_outputVideoNameVec.push_back(_T("wheat3.mp4"));

	static bool foundOutputVideoName = false;
	tstring workingVideoFileName = _T("");
	for each (auto name in m_outputVideoNameVec)
	{
		workingVideoFileName = (tstring(m_szRunDir).append(_T("\\")).append(name)).c_str(); //filepath of possible output video file

		LogInfo({ _T("current out video file from list: "), workingVideoFileName });

		if (0 == _taccess(workingVideoFileName.c_str(), 0)) //found workingFileName: possible output video file
		{
			LogInfo({ _T("found video file from list: "), workingVideoFileName });
			m_outputVideoNameFoundFromVecList = workingVideoFileName; //use this var. later when getting this file size
			foundOutputVideoName = true;
			break;
		}
		else //NOT found workingFileName: possible output video file
		{
			LogInfo({ _T("NOT found video file from list: "), workingVideoFileName });
		}
	}

	if (foundOutputVideoName) //if output video file found
	{
		LogInfo({ _T("deleting outputVideoFileName -- "), workingVideoFileName });
		DeleteFile(workingVideoFileName.c_str());
	}
	else
		m_outputVideoNameFoundFromVecList = (tstring(m_szRunDir).append(_T("\\bay3.mp4"))).c_str(); //set this var. to default "bay3.mp4" output video filename used by Premiere


	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\empty_proj.prproj"), szBackupEmptyProjectFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\run\\empty_proj_temp.prproj"), m_szEmptyProjectFP);

	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\render_clips_1080_H.264.prproj"), szBackupProjectFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\run\\render_clips_1080_H.264_temp.prproj"), m_szProjectFP);

	LogInfo({ _T("deleting empty_proj_temp.prproj in run directory --"), m_szEmptyProjectFP });
	DeleteFile(m_szEmptyProjectFP);

	LogInfo({ _T("deleting render_clips_1080_H.264_temp.prproj in run directory --"), m_szProjectFP });
	DeleteFile(m_szProjectFP);

	LogInfo({ _T("Copy empty_proj.prproj file from src to dest --"), szBackupEmptyProjectFP, m_szEmptyProjectFP });
	CopyFile(szBackupEmptyProjectFP, m_szEmptyProjectFP, FALSE);

	LogInfo({ _T("Copy render_clips_1080_H.264.prproj file from src to dest --"), szBackupProjectFP, m_szProjectFP });
	CopyFile(szBackupProjectFP, m_szProjectFP, FALSE);

	if (0 != _taccess(m_szProjectFP, 0) || 0 != _taccess(m_szEmptyProjectFP, 0))
	{
		LogError({ _T("can't find either project file "), m_szProjectFP, m_szEmptyProjectFP });
		return -1;
	}

	LogInfo(_T("Prepare video files"));

	m_videoNameVec.push_back(_T("bay1.mp4"));
	m_videoNameVec.push_back(_T("bay2.mp4"));
	m_videoNameVec.push_back(_T("bay3.mp4"));

	m_videoNameVec.push_back(_T("trees1.mp4"));
	m_videoNameVec.push_back(_T("trees2.mp4"));
	m_videoNameVec.push_back(_T("trees3.mp4"));

	m_videoNameVec.push_back(_T("wheat1.mp4"));
	m_videoNameVec.push_back(_T("wheat2.mp4"));
	m_videoNameVec.push_back(_T("wheat3.mp4"));

	TCHAR szworkingDir[_MAX_PATH] = { 0 };
	TCHAR szbackupDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(WorkingVideoDir.c_str(), szworkingDir);

	for each (auto name in m_videoNameVec)
	{
		tstring workingFileName = (tstring(szworkingDir).append(name)).c_str();
		tstring backupFileName = (tstring(BackeupVideoDir).append(name)).c_str();

		::CopyFile(workingFileName.c_str(), backupFileName.c_str(), FALSE);
		LogInfo({ _T("Copy video file from src to dest --"), workingFileName.c_str(), backupFileName.c_str() });

		if (0 != _taccess(workingFileName.c_str(), 0) || (0 != _taccess(backupFileName.c_str(), 0)))
		{
			LogError({ _T("can't find video file "), workingFileName, backupFileName });
			return -2;
		}
	}

	LogInfo(_T("initDependFile finished."));

	return BENCH_MARK_OK;
}

int CPremiereMark::writeScript()
{
	FILE *fp = NULL;
	CString strScriptContent;
	TCHAR szScriptFP[_MAX_PATH] = { 0 };
	TCHAR szProjectFP[_MAX_PATH * 2] = { 0 };

	LogInfo(_T("16"));
	_tcscpy_s(szProjectFP, _ARRAYSIZE(szProjectFP), m_szProjectFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\openproject.jsx"), szScriptFP);
	changePathDoubleSplit(szProjectFP, _ARRAYSIZE(szProjectFP));
	strScriptContent.Format(_T("var project = new File(\"%s\");\r\n")\
		_T("app.openDocument(project);\r\n"),
		szProjectFP);

	_tfopen_s(&fp, szScriptFP, _T("wb"));
	if (NULL == fp)
	{
		LogError({ _T("PremiereMark写入脚本错误，无法写入文件"), szScriptFP });
		return -1;
	}

	unsigned char szHeader[3] = { 0 };
	FILE_UTF8_HEADER(szHeader);
	fwrite(szHeader, sizeof(char), _ARRAYSIZE(szHeader), fp);

	std::string& rstrUtf8Content = MyPlatform::TcharToUtf8(strScriptContent.GetBuffer(0));
	fwrite(rstrUtf8Content.c_str(), 1, rstrUtf8Content.size(), fp);

	fclose(fp);
	return BENCH_MARK_OK;
}

int CPremiereMark::startPremiere()
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	CString strCmd;
	AdobeRegisterWindow wndRegister(this);
	wndRegister.RegisterWindowHandler([&]() ->int {
		LogInfo(_T("17"));
		TCHAR szProjectFP[_MAX_PATH] = { 0 };
		strCmd.Format(_T("\"%s\" \"%s\""), m_szPremiereFP, m_szEmptyProjectFP);			
		if (FALSE == CreateProcess(NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			MyPlatform::MyTrace(L0, _T("CPremiereMark::run CreateProcess failed, errno=[%d]"), GetLastError());
			LogError({ _T("PremiereMark：打开工程失败") });
			return -1;
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		LogInfo(_T("18"));
			
		return BENCH_MARK_OK;
	});		
	Sleep(5 * 1000);
	TakeMeasurement(new CCounterContent(CPremiere_Opids::DESC_TYPE_MARKER, CPremiere_Opids::DESC_OPEN_DOCUMENT),
		[&]()->int
	{
		strCmd.Format(_T("\"%s\" \"%s\""), m_szPremiereFP, m_szProjectFP);	
		if (FALSE == CreateProcess(NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			MyPlatform::MyTrace(L0, _T("CPremiereMark::run CreateProcess failed, errno=[%d]"), GetLastError());
			LogError({ _T("PremiereMark：打开工程失败") });
			return -1;
		}
		
		LogInfo(_T("20"));

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		findPremiereWnd();
		
		return BENCH_MARK_OK;
	});
	
	return BENCH_MARK_OK;
}

int CPremiereMark::findPremiereWnd()
{
	const TCHAR *szCalssName = _T("Premiere Pro");
	CString strWindowNameWithExt;
	CString strWindowNameNoExt;
	DWORD dwTime = 0;
	LogInfo(_T("21"));

	LogInfo(_T("查找Premiere窗口"));
	strWindowNameWithExt.Format(_T("Adobe Premiere Pro CC 2019 - %s"), m_szProjectFP);
	strWindowNameNoExt = strWindowNameWithExt.Left(strWindowNameWithExt.ReverseFind('.'));
	strWindowNameNoExt.Replace(_T("Desktop"), _T("桌面"));
	strWindowNameNoExt.Replace(_T("Users"), _T("用户"));
	LogInfo({ tstring(szCalssName), _T("---"), strWindowNameNoExt.GetString() });

	while (dwTime < 2 * 60 * 1000)
	{
		std::vector<HWND> vhWnd;
		MyPlatform::FindWindowWithClassNameAndWindowName(szCalssName, strWindowNameNoExt.GetString(), false, false, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (hPromiereMainWnd = vhWnd[0]))
		{
			LogInfo({ _T("PremiereMark Wnd Found size is ="), MyPlatform::Int64ToStringW(vhWnd.size()), _T("hWnd="), MyPlatform::Int64ToStringW((int)hPromiereMainWnd) });
			break;
		}
		dwTime += 1;
		Sleep(1);
	}
	LogInfo(_T("23"));

	if (NULL == hPromiereMainWnd)
	{
		LogError({ _T("PremiereMark：没有找到PremiereMark窗口") });
		return -2;
	}
	LogInfo(_T("24"));

	return BENCH_MARK_OK;

}

int CPremiereMark::SendRenderOrder()
{
#ifdef DEBUG
//SetWindowPos(hPromiereMainWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
	SetWindowPos(hPromiereMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif

	SendExportKey(/*hPromiereMainWnd*/);

	LogInfo(_T("Premiere播放视频 for 0 minutes constant duration."));
	//SleepAndRecord(2 * 60 * 1000);

	return BENCH_MARK_OK;
}

BOOL CALLBACK TheEnumProc(HWND hWnd, LPARAM lParam)
{
	static bool found = false;
	if (found)
		return false;
	CPremiereMark* pThis = (CPremiereMark*)lParam;
	/*
	int keystrokesend = 4;
	INPUT input[4];
	ZeroMemory(input, sizeof(input));


	input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
	input[1].ki.wVk = input[2].ki.wVk = 0x4D;
	input[0].ki.wVk = input[3].ki.wVk = VK_CONTROL;
	input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;

	if (keystrokesend != SendInput(4, input, sizeof(INPUT)))
	{
		pThis->LogInfo(_T("SendInput failed to send all."));
		return 0;
	}
	*/
	AU3_Sleep(3000);
	int intFileMenuX = 20;
	int intFileMenuY = 35;

	AU3_MouseMove(intFileMenuX, intFileMenuY);
	AU3_Sleep(500);
	AU3_MouseClick(L"left", intFileMenuX, intFileMenuY);
	AU3_Sleep(2000);
	AU3_MouseMove(intFileMenuX, 625);
	AU3_Sleep(500);
	AU3_MouseClick(L"left", intFileMenuX, 625);
	AU3_Sleep(2000);
	AU3_MouseMove(intFileMenuX + 350, 625);
	AU3_Sleep(500);
	AU3_MouseClick(L"left", intFileMenuX + 350, 625);


	pThis->SleepAndRecord(2 * 1000);
	std::vector<HWND> vhWnd;
	HWND hExportWnd = NULL;
	int dwTime = 0;
	while (dwTime < 10 * 1000)
	{
		MyPlatform::FindWindowWithClassNameAndWindowName(_T("#32770"), _T("导出设置"), false, false, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (hExportWnd = vhWnd[0]))
		{
			pThis->LogInfo({ _T("hExportWnd ="), MyPlatform::Int64ToStringW((int)hExportWnd) });
			found = true;
			break;
		}
		dwTime += 1000;
		pThis->SleepAndRecord(1000);
	}
	if (hExportWnd != NULL)
	{
		pThis->importPreset();


		pThis->LogInfo(_T("click on export button."));
		RECT rectWindow = { 0 };
		GetWindowRect(hExportWnd, &rectWindow); //window rect

		POINT pt = { rectWindow.left + 820, rectWindow.top + 680 };

		SetCursorPos(pt.x, pt.y);
		pThis->LogInfo({ _T("get window rect top =, left = "), MyPlatform::Int64ToStringW(pt.x), MyPlatform::Int64ToStringW(pt.y) });
		doKeyEvent(VK_CONTROL, false, false);
		doLButton(pt.x, pt.y);
	}

	int dwTime1 = 0;

	pThis->LogInfo({ _T("Invoke TakeMeasurement for this wnd. Counter: "), MyPlatform::Int64ToStringW(pThis->enum_proc_counter) });

	pThis->TakeMeasurement (new CCounterContent(CPremiere_Opids::DESC_TYPE_MARKER, CPremiere_Opids::DESC_RUN),
		[&]()->int
	{
		while (dwTime1 < pThis->MAX_RENDER_TIME_SECS * 1000 && IsWindowVisible(hExportWnd))
		{
			dwTime1 += 200;
			Sleep(200);
		}
		return BENCH_MARK_OK;
	});


	pThis->LogInfo(_T("set input complete for this wnd."));
	return TRUE;
}

int CPremiereMark::SendExportKey()
{
	HWND hWndFirst = GetWindow(hPromiereMainWnd, GW_HWNDFIRST);
	HWND hWndTopmost = GetWindow(hPromiereMainWnd, GW_CHILD);
	HWND hForegroundWnd = GetForegroundWindow();
	if (hForegroundWnd == hPromiereMainWnd)
	{
		LogInfo(_T("hForegroundWnd is parent window."));
	}
	else if (hForegroundWnd == hWndFirst)
	{
		LogInfo(_T("hForegroundWnd is first window."));
	}
	else if (hForegroundWnd == hWndTopmost)
	{
		LogInfo(_T("hForegroundWnd is top most window."));
	}
	if (hWndFirst == hWndTopmost)
	{
		LogInfo(_T("hWndFirst is top most window."));
	}

	EnumChildWindows(hPromiereMainWnd, TheEnumProc, (LPARAM)this);
	return 0;
}

int CPremiereMark::deleteIntermediateFiles()
{
	TCHAR szDirFP[_MAX_PATH] = { 0 };

	MyPlatform::KillProcessFromName(_T("Adobe Premiere Pro.exe"));
	SleepAndRecord(500);

	for each (auto name in m_videoNameVec)
	{
		tstring backupFileName = (tstring(BackeupVideoDir) + name).c_str(); //eg. C:\<video filename>
		LogInfo({ _T("deleting backup File --"), backupFileName });
		DeleteFile(backupFileName.c_str());
	}
	LogInfo(_T("finish deleteIntermediateFiles."));

	return BENCH_MARK_OK;
}

int CPremiereMark::importPreset()
{
	tstring strExportSettingsWinTitle = L"[CLASS:#32770; TITLE:导出设置]";
	tstring strExportSettingsText = L"OS_ViewContainer";
	tstring strExportSettingsControlClass = L"DroverLord - Window Class1";
	tstring strExportSettingsCommentsFormControlClass = L"Edit19";
	tstring strOfflineMaterialWarningWinTitle = L"[CLASS:#32770; TITLE:Adobe Premiere Pro]";
	tstring strOfflineMaterialWarningText = L"OS_ViewContainer";

	tstring strEncodeProgressWinTitle = L"[CLASS:#32770; TITLE:Encoding;]";
	tstring strEncodeProgressText = L"OS_ViewContainer";

	int intFormatBtnXOffset = 246;
	int intFormatBtnYOffset = 154;

	int intImportPresetBtnXOffset = 97;
	int intImportPresetBtnYOffset = 157;

	int intOutputBtnXOffset = 312;
	int intOutputBtnYOffset = 227;

	int intExportBtnXOffset = 178;
	int intExportBtnYOffset = 36;


	//Verify that the application window with the desired projected open appears and bring it to the foreground
	//Set the window match mode to substring match, preserving the original value
	int intOldWinMatchModeValue = AU3_Opt(L"WinTitleMatchMode", 2);

	//Restore the window match mode back to it's previous state
	AU3_Opt(L"WinTitleMatchMode", intOldWinMatchModeValue);

	//Slow down the key send rate to minimize errors
	int intOldSendKeyDelay = AU3_Opt(L"SendKeyDelay", 250);

		//We will attempt to bring up the Encode dialog by focusing on a the sequence panel
		//and simulating the "CTRL-m" keyboard sequence using the function Send().We will attempt to
		//bring up the "Export Settings" dialog multiple times in a "retry loop".
	int intNumAttempts = 0;
	int intMaxAttempts = 15;
	
	
	//Move the encode window and resize it
	AU3_WinMove (strExportSettingsWinTitle.c_str(), strExportSettingsText.c_str(), 0, 0, 1024, 720);

	AU3_Sleep(5000);
	
	RECT rect;
	AU3_WinGetPos (strExportSettingsWinTitle.c_str(), strExportSettingsText.c_str(), &rect);

	AU3_WinActivate(strExportSettingsWinTitle.c_str(), strExportSettingsText.c_str());

	AU3_Sleep(2000);

	//Click on the Format drop down box via mouse movements and select "H.264" by simulating the keyboard
	//sequence: "h->{SPACEBAR}"
	int intFormatBtnX = rect.right - intFormatBtnXOffset;
	int intFormatBtnY = intFormatBtnYOffset;

	// click on the Select Preset drop down
	AU3_MouseMove(intFormatBtnX, intFormatBtnY);

	AU3_Sleep(2000);

	AU3_MouseClick(L"left", intFormatBtnX, intFormatBtnY);

	// click on the second item in the drop down
	AU3_Sleep(2000);
	AU3_MouseMove(intFormatBtnX, intFormatBtnY + 42);
	AU3_Sleep(2000);
	AU3_MouseClick(L"left", intFormatBtnX, intFormatBtnY + 42);

	//click on the preset drop down again
	AU3_Sleep(2000);
	AU3_MouseMove(intFormatBtnX, intFormatBtnY);
	AU3_Sleep(2000);
	AU3_MouseClick(L"left", intFormatBtnX, intFormatBtnY);

	// click on the scrollbar to scroll up
	AU3_Sleep(2000);
	AU3_MouseMove(intFormatBtnX + 138, intFormatBtnY + 22);
	AU3_Sleep(2000);
	AU3_MouseClick(L"left", intFormatBtnX + 138, intFormatBtnY + 22);


	// click on the render clips preset to select
	AU3_Sleep(2000);
	AU3_MouseMove(intFormatBtnX, intFormatBtnY + 22);
	AU3_Sleep(2000);
	AU3_MouseClick(L"left", intFormatBtnX, intFormatBtnY + 22);
	AU3_Sleep(2000);
	/*
	
	//HKL lang = GetKeyboardLayout(GetWindowThreadProcessId (GetForegroundWindow(), NULL));
	//unsigned int i = (unsigned int)lang & 0x0000FFFF;
	AU3_Sleep(3000);

	//if (i == 1033) // keyboard layout is english
	//{
	//	AU3_Send(L"h{SPACE}");
	//}
	//else
	//{
	//	AU3_Send(L"!h{SPACE}");
	//}


	AU3_MouseMove(730, 365);

	AU3_Sleep(2000);

	AU3_MouseClick(L"left", 730, 365);

	AU3_Sleep(2000);

	AU3_MouseMove(450, intFormatBtnY);

	AU3_Sleep(2000);

	AU3_MouseClick(L"left", 450, intFormatBtnY);

	AU3_Sleep(3000);
	
	//char *language = "0409";
	//sscanf_s (language, "%x", &i);
	//SendMessage(GetActiveWindow(), WM_INPUTLANGCHANGEREQUEST, 0, i);


	AU3_Sleep(4000);

	//Click on the import preset button to bring up the Import Preset dialog where we specify which preset we want.
	int intImportPresetBtnX = rect.right - intImportPresetBtnXOffset;
	int intImportPresetBtnY = intImportPresetBtnYOffset;

	AU3_MouseMove(intImportPresetBtnX, intImportPresetBtnY);


	AU3_Sleep(2000);

	AU3_MouseClick(L"left", intImportPresetBtnX, intImportPresetBtnY);

	TCHAR username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName((TCHAR*)username, &username_len);
	tstring destFile = _T("C:\\Users\\"); //user dir. filepath
	destFile.append(username).append(_T("\\Documents\\Adobe\\Adobe Media Encoder\\13.0\\Presets\\")); //user dir. filepath

	destFile.append(PresetFileName); //user dir. filepath

	tstring strPresetFile = destFile;
	//Handle the Import Preset window that should appear
	importPreset(strPresetFile);
	*/

	//Now click on the specify output file path link by using mouse movements
	int intOutputBtnX = rect.right - intOutputBtnXOffset;
	int intOutputBtnY = intOutputBtnYOffset;

	AU3_MouseMove(intOutputBtnX, intOutputBtnY);

	AU3_Sleep(2000);

	AU3_MouseClick(L"left", intOutputBtnX, intOutputBtnY);

	MyPlatform::GetFullPathUnderExe(_T("Resources\\Pr\\run"), m_szRunDir);
	
	tstring strOutputFile = L"";
	strOutputFile.append(m_szRunDir).append(_T("\\bay3.mp4")); //user dir. filepath

	//Handle the "Save As" dialog for the output file
	setEncodeOutputPath(strOutputFile);

	AU3_Sleep(3000);

	return 1;
	
	
}


int CPremiereMark::importPreset(tstring strPresetFileToImportFullPath)
{

	tstring strFuncName = L"ImportPreset";

	tstring strImportPresetWinTitle = L"[CLASS:#32770; TITLE:导入预设]";
	tstring strImportPresetWinText = L"epr";
	tstring strImportPresetFileField = L"Edit1";
	tstring strImportPresetOpenButton = L"Button1";

	tstring strChooseNameWinTitle = L"[CLASS:#32770; TITLE:选择名称]";
	tstring strChooseNameWinText = L"ViewContainer";
	tstring strChooseNameField = L"Edit1";

	// Check for the existance of the dialog
	if (!(AU3_WinWait(strImportPresetWinTitle.c_str(), strImportPresetWinText.c_str(), 30)))
	{
		//ThrowScriptError(22018, strFuncName & ": The Import Preset Dialog did not appear as we anticipated.")
	}//; Else all is well

	// Open the desired preset
	AU3_WinActivate(strImportPresetWinTitle.c_str(), strImportPresetWinText.c_str());

	AU3_Sleep(1000);

	AU3_ControlFocus(strImportPresetWinTitle.c_str(), strImportPresetWinText.c_str(), strImportPresetFileField.c_str());

	AU3_Sleep(1000);

	AU3_ControlSetText(strImportPresetWinTitle.c_str(), strImportPresetWinText.c_str(), strImportPresetFileField.c_str(), strPresetFileToImportFullPath.c_str());

	AU3_Sleep(1000);

	AU3_ControlFocus(strImportPresetWinTitle.c_str(), strImportPresetWinText.c_str(), strImportPresetOpenButton.c_str());

	AU3_Sleep(1000);

	AU3_ControlClick (strImportPresetWinTitle.c_str(), strImportPresetWinText.c_str(), strImportPresetOpenButton.c_str(), L"left", 1);

	//Check to ensure the Import dialog goes away as expected
	if (!(AU3_WinWaitClose(strImportPresetWinTitle.c_str(), strChooseNameWinText.c_str(), 30))) {

		//ThrowScriptError(22019, strFuncName & ": The Import Preset Dialog did not disappear as we anticipated.")

	} // Else all is well

	// Now check that the "Choose Name" dialog appears during the Import Preset process
	if (!(AU3_WinWait(strChooseNameWinTitle.c_str(), strChooseNameWinText.c_str(), 30)))
	{
		//ThrowScriptError(22020, strFuncName & ": The Choose Name Dialog did not appear as we anticipated.")
	} // Else all is well

		// Click OK to import the preset
	AU3_WinActivate(strChooseNameWinTitle.c_str(), strChooseNameWinText.c_str());

	AU3_Sleep(1000);

	AU3_ControlFocus(strChooseNameWinTitle.c_str(), strChooseNameWinText.c_str(), strChooseNameField.c_str());

	AU3_Sleep(1000);

	RECT rect;
	AU3_WinGetPos(strChooseNameWinTitle.c_str(), strChooseNameWinText.c_str(), &rect);

	int intOutputBtnX = rect.left + 355;
	int intOutputBtnY = rect.top -10;

	AU3_MouseMove(intOutputBtnX, intOutputBtnY);

	AU3_Sleep(2000);

	AU3_MouseClick(L"left", intOutputBtnX, intOutputBtnY);

	//AU3_Send(L"{ENTER}");

	// Check to ensure the Import dialog goes away as expected
	if (!(AU3_WinWaitClose(strChooseNameWinTitle.c_str(), strChooseNameWinText.c_str(), 30)))
	{
		//ThrowScriptError(22021, strFuncName & ": The Choose Name Dialog did not disappear as we anticipated.");
	} // Else all is well


	AU3_Sleep(4000);
	tstring strSavePresetWinTitle = L"[CLASS:#32770; TITLE:保存预设]";

	if (AU3_WinWait(strSavePresetWinTitle.c_str(), L"", 10))
	{
		AU3_WinActivate(strSavePresetWinTitle.c_str(), L"");
		AU3_WinWaitActive(strSavePresetWinTitle.c_str(), L"", 10);
		AU3_WinSetState(strSavePresetWinTitle.c_str(), L"", SW_SHOW);

		RECT rect;
		AU3_WinGetPos(strSavePresetWinTitle.c_str(), L"", &rect);

		AU3_WinActivate(strSavePresetWinTitle.c_str(), L"");

		AU3_Sleep(1000);

		//Click on the Format drop down box via mouse movements and select "H.264" by simulating the keyboard
		//sequence: "h->{SPACEBAR}"
		int intFormatBtnX = rect.left + 150;
		int intFormatBtnY = rect.top + 98; // intFormatBtnYOffset;

		AU3_MouseMove(intFormatBtnX, intFormatBtnY);

		AU3_Sleep(2000);

		AU3_MouseClick(L"left", intFormatBtnX, intFormatBtnY);

		AU3_WinWaitClose(strSavePresetWinTitle.c_str(), L"", 30);
	} // Else all is well

	return 1;
}

int CPremiereMark::setEncodeOutputPath(tstring strOutputFile)
{
	tstring strSaveAsDialogWinTitle = L"[CLASS:#32770; TITLE:另存为]";
	tstring strSaveAsDialogText = L"*.";
	tstring strSaveAsDialogFileNameCC = L"Edit1";
	tstring strSaveAsDialogSaveButtonCC = L"Button2";

	// Verify the existance and bring to the foreground the "Save As" dialog using WinWaitAndActivate()
	AU3_WinWait(strSaveAsDialogWinTitle.c_str(), strSaveAsDialogText.c_str(), 30);

	AU3_WinActivate(strSaveAsDialogWinTitle.c_str(), strSaveAsDialogText.c_str());

	AU3_Sleep(2500);

	// Set the desired output directory using ControlSetText()
	AU3_ControlFocus(strSaveAsDialogWinTitle.c_str(), strSaveAsDialogText.c_str(), strSaveAsDialogFileNameCC.c_str());

	AU3_Sleep(1000);

	//AU3_Send(strOutputFile.c_str());
	AU3_ControlSetText(strSaveAsDialogWinTitle.c_str(), strSaveAsDialogText.c_str(), strSaveAsDialogFileNameCC.c_str(), strOutputFile.c_str());


	AU3_Sleep(1000);

	// Click the "Save" button to confirm the output file location of the encode.
	AU3_ControlClick(strSaveAsDialogWinTitle.c_str(), strSaveAsDialogText.c_str(), strSaveAsDialogSaveButtonCC.c_str(), L"left", 1);

	// Check to ensure the Import dialog goes away as expected
	if (!(AU3_WinWaitClose(strSaveAsDialogWinTitle.c_str(), strSaveAsDialogText.c_str(), 30)))
	{
		//ThrowScriptError(22022, $c_strFuncName & ": The Choose Name Dialog did not disappear as we anticipated.")
	}// Else all is well

	AU3_Sleep(1000);

	return 1;
} //== > SetEncodeOutputPath

