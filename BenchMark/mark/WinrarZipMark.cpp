#include "stdafx.h"
#include "MarkObject.h"
#include "WinrarZipMark.h"
#include "Winrar_Opids.h"

using namespace MyPlatform;

CWinrarZipMark::CWinrarZipMark()
{
	LogInfo(_T("Invoke CWinrarZipMark().."));

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Winrar-ѹ����ѹ������"));
	m_eMarkType = E_MARK_WINRAR;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("f1a4983e29b0b1be611b1d4724fe4b01"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

CWinrarZipMark::~CWinrarZipMark()
{
	LogInfo(_T("Invoke ~CWinrarZipMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CWinrarZipMark::init()
{
	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	SleepAndRecord(500);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\WinRAR.exe"), m_szWinrarFP);
	if (0 != _taccess(m_szWinrarFP, 0))
	{
		LogError({ _T("û���ҵ�ѹ������") });
		return -1;
	}
	if (BENCH_MARK_OK != copyTestFile())
	{
		return -2;
	}


	return BENCH_MARK_OK;
}

int CWinrarZipMark::run()
{
	return WzipDemo7Z();
}

int CWinrarZipMark::destroy()
{
	LogInfo(_T("Invoke CWinrarZipMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	SleepAndRecord(500);
	return BENCH_MARK_OK;
}

typedef double Double;

int CWinrarZipMark::WzipDemo7Z()
{
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	TCHAR szZipTempFile[_MAX_PATH] = { 0 };
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\Ziptest"), szZipDir);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\Ziptest\\"), szZipTempFile);
	_tcscat_s(szZipTempFile, _MAX_PATH - _tcslen(szZipTempFile), TEST_ZIP_FILE_NAME);
	MyPlatform::DeleteDir(szZipDir);
	SleepAndRecord(500);
	MyPlatform::CreateFullDirectory(szZipDir);
	CreateDirectory(szZipDir, NULL);

	if (BENCH_MARK_OK != addZipFileByMouse())
	{
		return -1;
	}

	/// ѹ��
	{
		//ILog *lpLog = GetController()->GetTipsLog();
		LogInfo(TEXT("ѹ���ļ�"));

		TakeMeasurement(  new CCounterContent(CWinrar_Opids::DESC_TYPE_MARKER, CWinrar_Opids::DESC_RAR),
			[&]()->int {

		MyPlatform::tstringstream streamParam;
		std::auto_ptr<TCHAR> szParam;

		streamParam << _T("\"") << m_szWinrarFP << _T("\" a -o+ -mt32 ")
			<< _T("\"") << szZipTempFile << _T("\" ")
			<< _T("\"") << m_szSourceDir << _T("\"");
		MyPlatform::tstring strCmd = streamParam.str();
		szParam.reset((TCHAR *)calloc(strCmd.size() + 1, sizeof(TCHAR)));
		if (NULL == szParam.get())
		{
			LogError({ _T("ѹ���п����ڴ�ʧ��") });
			return -3;
		}
		_tcscpy_s(szParam.get(), strCmd.size() + 1, strCmd.c_str());

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		LogInfo(TEXT("��ѹ�ļ�"));
		LogInfo(_T("Winrar:ѹ���ļ���...."));
		if (!CreateProcess(NULL, szParam.get(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			LogError({ _T("ѹ���д���ѹ������ʧ��") });
			return -4;
		}
		WaitForSingleObject(pi.hThread, INFINITE);
		return BENCH_MARK_OK; });

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	SleepAndRecord(500);
	
	if (BENCH_MARK_OK != unZipFileByMose(szZipDir))
	{
		return -5;
	}

	/// ��ѹ
	{
		TakeMeasurement(  new CCounterContent(CWinrar_Opids::DESC_TYPE_MARKER, CWinrar_Opids::DESC_UNRAR),
			[&]()->int {

		MyPlatform::tstringstream streamParam;

		std::auto_ptr<TCHAR> szParam;
		streamParam << _T("\"") << m_szWinrarFP << _T("\" x -o+ -mt32 ")
			<< _T("\"") << szZipTempFile << _T("\" ")
			<< _T("\"") << szZipDir << _T("\"");

		CreateDirectory(szZipDir, NULL);
		MyPlatform::tstring strCmd = streamParam.str();
		szParam.reset((TCHAR *)calloc(strCmd.size() + 1, sizeof(TCHAR)));
		if (NULL == szParam.get())
		{
			LogError({ _T("��ѹ�п����ڴ�ʧ��") });
			return -1;
		}
		_tcscpy_s(szParam.get(), strCmd.size() + 1, strCmd.c_str());

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		LogInfo(_T("Winrar:��ѹ�ļ�"));
		LogInfo(TEXT("��ѹ�ļ�"));
		if (!CreateProcess(NULL, szParam.get(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			LogError({ _T("��ѹ�д�����ѹ����ʧ��") });
			return -2;
		}
		WaitForSingleObject(pi.hThread, INFINITE);
		return BENCH_MARK_OK; });

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	SleepAndRecord(500);
	MyPlatform::DeleteDir(m_szSourceDir);
	MyPlatform::DeleteDir(szZipDir);
	MyPlatform::DeleteDir(m_szSourceDir);

	return BENCH_MARK_OK;
}

int CWinrarZipMark::copyTestFile()
{
	//TCHAR szFirstSrcFile[_MAX_PATH] = { 0 };
	//TCHAR szSecondSrcFile[_MAX_PATH] = { 0 };
	TCHAR szDstFile[_MAX_PATH] = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\test"), m_szSourceDir);
	MyPlatform::DeleteDir(m_szSourceDir);
	MyPlatform::CreateFullDirectory(m_szSourceDir);
	CreateDirectory(m_szSourceDir, NULL);

	MyPlatform::GetFullPathUnderExe(_T("Resources\\WinRAR\\test\\tempZipUnZip.dat"), szDstFile);

// 	MyPlatform::GetFullPathUnderExe(_T("AE\\Fireworks\\[4K123]����0603.mp4"), szFirstSrcFile);
// 	MyPlatform::GetFullPathUnderExe(_T("AE\\Fireworks\\3.mp4"), szSecondSrcFile);
// 	if (0 != _taccess(szFirstSrcFile, 0))
// 	{
// 		LogError({_T("ѹ�����ԣ������ļ�������"), szFirstSrcFile);
// 		return -1;
// 	}
// 	if (0 != _taccess(szSecondSrcFile, 0))
// 	{
// 		LogError({_T("ѹ�����ԣ������ļ�������"), szFirstSrcFile);
// 		return -2;
// 	}

	LogInfo(_T("Winrar:����׼��ѹ����ѹ���ļ�..."));
#if 0
	for (int nIdx = 0; nIdx < 3; nIdx++)
	{
		_stprintf_s(szDstFile, _T("%s\\first_%d.dat"), m_szSourceDir, nIdx + 1);
// 		LogInfo(_T("Winrar:�����ļ�%s��%S"), 
// 			MyPlatform::GetFilePathFullName(szFirstSrcFile).c_str(),
// 			MyPlatform::GetFilePathFullName(szDstFile).c_str());
		CopyFile(szFirstSrcFile, szDstFile, FALSE);

		_stprintf_s(szDstFile, _T("%s\\second_%d.dat"), m_szSourceDir, nIdx + 1);
// 		LogInfo(_T("Winrar:�����ļ�%s��%S"),
// 			MyPlatform::GetFilePathFullName(szSecondSrcFile).c_str(),
// 			MyPlatform::GetFilePathFullName(szDstFile).c_str());
		CopyFile(szSecondSrcFile, szDstFile, FALSE);
	}
#else

#if 0
	for (int nIdx = 0; nIdx < 1000; nIdx++)
	{
		FILE* fp = NULL;
		int nDataCount = nIdx;
		char* pusData = (char*)malloc(nDataCount * sizeof(char));

		_stprintf_s(szDstFile, _ARRAYSIZE(szDstFile),  _T("%s\\%d.dat"),  m_szSourceDir, nIdx);
		_tfopen_s(&fp, szDstFile, _T("wb"));
		if (NULL == fp || NULL == pusData)
		{
			LogError({_T("�޷�д����Դ�ļ�\r\n"));
			return -3;
		}

		for (int nIdx = 0; nIdx < 13; nIdx++)
		{


			fwrite(pusData, sizeof(char), nDataCount, fp);
		}
		free(pusData);
		fclose(fp);
	}
#endif 

	//2019-11-07 �޸�Ϊ���һ��5GB�Ĵ��ļ�.
	int nOnceDataCount = 500 * 1024 * 1024;

	FILE* fp = NULL;
	_stprintf_s(szDstFile, _ARRAYSIZE(szDstFile), _T("%s\\%d.dat"), m_szSourceDir, 1);

	_tfopen_s(&fp, szDstFile, _T("wb"));
	if (NULL == fp)
	{
		LogError({ _T("�޷�д����Դ�ļ�\r\n") });
		return -3;
	}

	char* pPusData = (char*)malloc(nOnceDataCount * sizeof(char));
	if (pPusData == nullptr)
	{
		LogError({ _T("�޷�д����Դ�ļ�\r\n") });
		return -3;
	}

	::memset(pPusData, 0xff, nOnceDataCount * sizeof(char));

	for (int nIndex = 0; nIndex < 10; nIndex++)
	{
		fwrite(pPusData, sizeof(char), nOnceDataCount, fp);	
	}

	free(pPusData);
	fclose(fp);

#endif

	return BENCH_MARK_OK;
}

int CWinrarZipMark::addZipFileByMouse()
{
	HWND hWinrar = NULL, hAddFileWnd = NULL;
	DWORD dwTime = 0;
	CRect rectWindow = { 0 };

	
	/// ��������
	{
	
		TCHAR szCmd[_MAX_PATH * 3] = { 0 };
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };

		LogInfo(_T("Winrar:��ѹ��Ŀ¼"));
		_stprintf_s(szCmd, _ARRAYSIZE(szCmd), _T("\"%s\" \"%s\""), m_szWinrarFP, m_szSourceDir);
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			LogError({ _T("ѹ���д���ѹ������ʧ��") });
			return -1;
		}
		SleepAndRecord(dwTimerNode3);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		SleepAndRecord(dwTimerNode3);
	}

	/// �ҵ�����
	LogInfo(_T("Winrar:Ѱ��������"));
	SleepAndRecord(dwTimerNode1);
	while (dwTime < 60 * 1000)
	{
		std::vector<HWND> vhWnd;
		MyPlatform::FindWindowWithClassNameAndWindowName(_T("WinRarWindow"), _T("test - WinRAR"), false, true, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (hWinrar = vhWnd[0]))
		{
			break;
		}
		dwTime += 1;
		SleepAndRecord(10);
	}
	if (NULL == hWinrar)
	{
		LogError({ _T("û���ҵ�Winrar����") });
		return -2;
	}

	/// ���ѡ���ļ�
	LogInfo(_T("Winrar:ѡ��ѹ���ļ�"));
	SetWindowPos(hWinrar, HWND_TOP, 100, 100, 1000, 1000, SWP_SHOWWINDOW);
	SleepAndRecord(dwTimerNode1);
	GetWindowRect(hWinrar, rectWindow);
	{
		CPoint pointFile(rectWindow.TopLeft());
		pointFile.Offset(CSize(205, 205));
		doKeyEvent(VK_CONTROL, false, false);
		doLButton(pointFile.x, pointFile.y);
		SleepAndRecord(dwTimerNode3);
		doLButton(pointFile.x, pointFile.y);
		SleepAndRecord(dwTimerNode1);
		doKeyClick(0x41, false);
		SleepAndRecord(dwTimerNode1);
		doKeyEvent(VK_CONTROL, true, false);


	}

	/// 
	LogInfo(_T("Winrar:Ѱ����Ӵ���"));
	while (dwTime < 60 * 1000)
	{
		CPoint pointFile = rectWindow.TopLeft();
		pointFile.Offset(CSize(50, 70));
		doLButton(pointFile.x, pointFile.y);
		SleepAndRecord(dwTimerNode1);

		std::vector<HWND> vhWnd;
		MyPlatform::FindWindowWithClassNameAndWindowName(_T("#32770"), _T("ѹ���ļ����Ͳ���"), false, true, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (hAddFileWnd = vhWnd[0]))
		{
			break;
		}
		 
		dwTime += 1;
		SleepAndRecord(10);
	}
	if (NULL == hAddFileWnd)
	{
		LogError({ _T("û���ҵ�Winrar����") });
		return -3;
	}

	/// 
	LogInfo(_T("Winrar:���ȷ��"));
	SleepAndRecord(dwTimerNode1);
	{
		GetWindowRect(hAddFileWnd, rectWindow);
		CPoint point = rectWindow.TopLeft();

		point.Offset(CSize(160,380));
		doLButton(point.x, point.y);
	}

	SleepAndRecord(dwTimerNode1);
	SendMessage(hAddFileWnd, WM_CLOSE, 0, 0);
	SendMessage(hWinrar, WM_CLOSE, 0, 0);
	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	return BENCH_MARK_OK;
}

int CWinrarZipMark::unZipFileByMose(const TCHAR* szZipFileDir)
{
	HWND hWinrar = NULL, hAddFileWnd = NULL;
	DWORD dwTime = 0;
	CRect rectWindow = { 0 };
	
	
	/// ��������
	{
		TCHAR szCmd[_MAX_PATH * 3] = { 0 };
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };

		LogInfo(_T("Winrar:��ѹ��Ŀ¼"));
		_stprintf_s(szCmd, _ARRAYSIZE(szCmd), _T("\"%s\" \"%s\""), m_szWinrarFP, szZipFileDir);
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			LogError({ _T("ѹ���д���ѹ������ʧ��") });
			return -1;
		}
		SleepAndRecord(dwTimerNode1);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		SleepAndRecord(dwTimerNode1);
	}

	/// �ҵ�����
	LogInfo(_T("Winrar:Ѱ��������"));
	SleepAndRecord(dwTimerNode1);
	while (dwTime < 60 * 1000)
	{
		std::vector<HWND> vhWnd;
		MyPlatform::FindWindowWithClassNameAndWindowName(_T("WinRarWindow"), _T("Ziptest - WinRAR"), false, true, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (hWinrar = vhWnd[0]))
		{
			break;
		}
		if (NULL != hWinrar)
		{
			break;
		}
		dwTime += 1;
		SleepAndRecord(10);
	}
	if (NULL == hWinrar)
	{
		LogError({ _T("û���ҵ�Winrar����") });
		return -2;
	}

	/// ���ѡ���ļ�
	LogInfo(_T("Winrar:ѡ�н�ѹ�ļ�"));
	
	SetWindowPos(hWinrar, HWND_TOP, 100, 100, 1000, 1000, SWP_SHOWWINDOW);
	SleepAndRecord(dwTimerNode1);
	GetWindowRect(hWinrar, rectWindow);
	{
		CPoint pointFile(rectWindow.TopLeft());
		pointFile.Offset(CSize(100, 205));
		doKeyEvent(VK_CONTROL, false, false);
		doLButton(pointFile.x, pointFile.y);
		SleepAndRecord(2000);
		doKeyEvent(VK_CONTROL, true, false);
	}

	/// 
	LogInfo(_T("Winrar:Ѱ�ҽ�ѹ����"));
	while (dwTime < 60 * 1000)
	{
		CPoint pointFile = rectWindow.TopLeft();
		pointFile.Offset(CSize(100, 70));
		doLButton(pointFile.x, pointFile.y);
		SleepAndRecord(dwTimerNode1);

		std::vector<HWND> vhWnd;
		MyPlatform::FindWindowWithClassNameAndWindowName(_T("#32770"), _T("��ѹ·����ѡ��"), false, true, &vhWnd);
		if (vhWnd.size() != 0 && NULL != (hAddFileWnd = vhWnd[0]))
		{
			break;
		}
		dwTime += 1;
		SleepAndRecord(10);
	}
	if (NULL == hAddFileWnd)
	{
		LogError({ _T("û���ҵ�Winrar����") });
		return -3;
	}

	/// 
	LogInfo(_T("Winrar:���ȷ��"));
	SleepAndRecord(dwTimerNode1);
	{
		GetWindowRect(hAddFileWnd, rectWindow);
		CPoint point = rectWindow.TopLeft();

		point.Offset(CSize(240, 430));
		doLButton(point.x, point.y);
	}

	SleepAndRecord(dwTimerNode1);
	SendMessage(hAddFileWnd, WM_CLOSE, 0, 0);
	SendMessage(hWinrar, WM_CLOSE, 0, 0);
	MyPlatform::KillProcessFromName(_T("WinRAR.exe"));
	return BENCH_MARK_OK;
}
