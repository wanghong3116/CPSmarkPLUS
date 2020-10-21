#include "stdafx.h"
#include "MarkObject.h"
#include "AntiVirus.h"

CAntiVirusMark::CAntiVirusMark()
{
	_tcscpy_s(m_szDescript, _T("杀毒 电脑防护"));
	m_eMarkType = E_MARK_ANTI_VIRUS;
	m_eMarkClassify = E_MARK_CLASSIFY_OFFICE;
	m_bBasicModelType = true;
}

CAntiVirusMark::~CAntiVirusMark()
{

}

BOOL CopyFolder(const TCHAR* lpszTo, const TCHAR* lpszFrom)//拷贝文件夹
{
	TCHAR szFromPathTemp[_MAX_PATH * 2] = { 0 };//源文件路径
	TCHAR szToTemp[_MAX_PATH * 2] = { 0 };//源文件路径
	_tcscpy_s(szFromPathTemp, _ARRAYSIZE(szFromPathTemp), lpszFrom);
	*(szFromPathTemp + _tcslen(szFromPathTemp)) = '\0';
	*(szFromPathTemp + _tcslen(szFromPathTemp) + 1) = '\0';
	_tcscpy_s(szToTemp, _ARRAYSIZE(szToTemp), lpszTo);
	*(szToTemp + _tcslen(szToTemp)) = '\0';
	*(szToTemp + _tcslen(szToTemp) + 1) = '\0';
	SHFILEOPSTRUCT FileOp;
	SecureZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));//secureZeroMemory和ZeroMerory的区别
															 //根据MSDN上，ZeryMerory在当缓冲区的字符串超出生命周期的时候，
															 //会被编译器优化，从而缓冲区的内容会被恶意软件捕捉到。
															 //引起软件安全问题，特别是对于密码这些比较敏感的信息而说。
															 //而SecureZeroMemory则不会引发此问题，保证缓冲区的内容会被正确的清零。
															 //如果涉及到比较敏感的内容，尽量使用SecureZeroMemory函数。
	FileOp.fFlags = FOF_NOCONFIRMATION; //操作与确认标志 
	FileOp.hNameMappings = NULL; //文件映射
	FileOp.hwnd = NULL; //消息发送的窗口句柄；
	FileOp.lpszProgressTitle = NULL; //文件操作进度窗口标题 
	FileOp.pFrom = szFromPathTemp; //源文件及路径 
	FileOp.pTo = szToTemp; //目标文件及路径 
	FileOp.wFunc = FO_COPY; //操作类型 
	return SHFileOperation(&FileOp);
}


int CAntiVirusMark::init()
{
	MyPlatform::GetFullPathUnderExe(_T("kingsoft antivirus\\kxetray.exe"), m_szKingFP);
	MyPlatform::GetFullPathUnderExe(_T("kingsoft antivirus\\findC.bmp"), m_szDriveCImageFP);
	MyPlatform::GetFullPathUnderExe(_T("kingsoft antivirus\\finddir.bmp"), m_szFindDirImageFP);
	MyPlatform::GetFullPathUnderExe(_T("kingsoft antivirus\\findC_CN.bmp"), m_szDriveCCNImageFP);
	const TCHAR* CHECK_DIR = _T("C:\\$AAA");
	const TCHAR* COPY_FILE_NAME = _T("Haiyang.rar");
	TCHAR szSourceFile[_MAX_PATH] = { 0 };
	TCHAR szDestFile[_MAX_PATH] = { 0 };

	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:启动软件。。。"));
	if (0 != _taccess(m_szKingFP, 0)
		|| 0 != _taccess(m_szDriveCImageFP, 0)
		|| 0 != _taccess(m_szFindDirImageFP, 0)
		|| 0 != _taccess(m_szDriveCCNImageFP, 0)
		)
	{
		return -1;
	}
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	while (!IsWindow(hKismainWnd) || !::IsWindowVisible(hKismainWnd))
	{
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);
		if (!CreateProcess(m_szKingFP, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			Sleep(100);
			continue;
		}
		CLOSEHANDLE(pi.hProcess);
		CLOSEHANDLE(pi.hThread);

		hKismainWnd = ::FindWindow(_T("kismain{1ACD30B1-18F3-4f4d-B52D-4709D099998C}"), _T("金山毒霸"));
		printf("FindWindow  金山毒霸\r\n");
	}
	Sleep(1000);
	::SetWindowPos(hKismainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	
	GetWindowThreadProcessId(hKismainWnd, &dwKismainProcessID);

	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:初始化目录。。。"));
	BOOL bCopyOK = TRUE;
	if (0 != _taccess(CHECK_DIR, 0))
	{
		MyPlatform::GetFullPathUnderExe(COPY_FILE_NAME, szSourceFile, _ARRAYSIZE(szSourceFile));
		CreateDirectory(CHECK_DIR, NULL);
		Sleep(1000);
		//bCopyOK =  CopyFolder(CHECK_DIR, szSourceDir);
		_stprintf_s(szDestFile, _ARRAYSIZE(szDestFile), _T("%s\\%s"), CHECK_DIR, COPY_FILE_NAME);
		CopyFile(szSourceFile, szDestFile, FALSE);
	}

	return BENCH_MARK_OK;
}


int GetHWNDImage(HWND hWnd, int& nImageW, int& nImageH, void** ppOutBits)
{
	HDC hDC = NULL;
	CImage image;
	CRect rect;
	void* pImageBits = NULL;

	if (!IsWindow(hWnd))
	{
		return -1;
	}

	hDC = GetDC(hWnd);
	GetClientRect(hWnd, rect);
	nImageW = rect.Width();
	nImageH = rect.Height();
	if (nImageW == 0 || nImageH == 0)
	{
		return -2;
	}

	image.Create(nImageW, -nImageH, 32);
	BitBlt(image.GetDC(), 0, 0, rect.Width(), rect.Height(), hDC, 0, 0, SRCCOPY);
	if (NULL == (*ppOutBits = calloc(nImageW *  nImageH, 4)))
	{
		return -2;
	}
	pImageBits = image.GetBits();
	memcpy(*ppOutBits, pImageBits, nImageW * nImageH * 4);

	image.ReleaseDC();
	memcpy(pImageBits, *ppOutBits, nImageW * nImageH * 4);
#ifdef _DEBUG
// 	static int a = 0;
// 	CString str;
// 	str.Format(_T("C:\\Users\\86\\Desktop\\test\\%d.bmp"), a++);
// 	image.Save(str, Gdiplus::ImageFormatBMP);
#endif
	return 0;
	//	
	// 	IStream* pOutStream = NULL;
	// 	if (CreateStreamOnHGlobal(NULL, TRUE, &pOutStream) == S_OK)
	// 	{
	// 		image.Save(pOutStream, Gdiplus::ImageFormatJPEG);
	// 		HGLOBAL hOutGlobal = NULL;
	// 		GetHGlobalFromStream(pOutStream, &hOutGlobal);
	// 		LPBYTE pBits = (LPBYTE)GlobalLock(hOutGlobal);
	// 		ulBufferLen = (DWORD)GlobalSize(pBits);
	// 		*pBuffer = new char[ulBufferLen];
	// 		memcpy(*pBuffer, pBits, ulBufferLen);
	// 
	// 		GlobalUnlock(hOutGlobal);
	// 		pOutStream->Release();
	// 	}
	// 	IStream imageStream;
	//image.Save()
}


bool FindImageIsSameContentAtBigImagePoint(int* pBigImage, int nBigImageW, int nBigImageH, int nBigImageX, int nBigImageY,
	int* pFindImage, int nFindW, int nFindH)
{
	if (nBigImageX + nFindW > nBigImageW || nBigImageY + nFindH > nBigImageH)
	{
		false;
	}

	for (int nY = 0; nY < nFindH; nY++)
	{
		if (0 != memcmp(pBigImage + nBigImageX + (nBigImageY + nY) * nBigImageW, pFindImage + nY * nFindW, nFindW * sizeof(int)))
		{
			return false;
		}
	}

	return true;
}

int FindImagePositionFromHWnd(HWND hWnd, const TCHAR* szFindImageFP, long& nFindX, long& nFindY)
{
	int nWndW = 0, nWndH = 0, nFindW = 0, nFindH = 0;
	void* pWndImageData = NULL, *pFindImageData = NULL;
	CImage imageFind, imageFind32;

	if (!IsWindow(hWnd))
	{
		return -1;
	}
	if (0 != _taccess(szFindImageFP, 0))
	{
		return -2;
	}
	if (0 != GetHWNDImage(hWnd, nWndW, nWndH, &pWndImageData))
	{
		return -3;
	}
	if (S_OK != imageFind.Load(szFindImageFP))
	{
		return -4;
	}
	imageFind32.Create(imageFind.GetWidth(), -imageFind.GetHeight(), 32);
	imageFind.BitBlt(imageFind32.GetDC(), 0, 0, SRCCOPY);
	imageFind32.ReleaseDC();
	pFindImageData = imageFind32.GetBits();
	nFindW = imageFind32.GetWidth();
	nFindH = imageFind32.GetHeight();

	for (int nY = 0; nY < nWndH; nY++)
	{
		//int* pnWndRow = (int*)pWndImageData + nY * nWndW;
		for (int nX = 0; nX < nWndW; nX++)
		{
			if (FindImageIsSameContentAtBigImagePoint((int*)pWndImageData, nWndW, nWndH, nX, nY, (int*)pFindImageData, nFindW, nFindH))
			{
				nFindX = nX;
				nFindY = nY;
				return 0;
			}

		}
	}

	return -5;
}


void GetProcessAllWindow(DWORD dwProcessID, std::vector<HWND>& vhAllWindow)
{
	struct EnumParam
	{
		DWORD				dwFindID;
		std::vector<HWND>*	pvhWindow;
	};

	EnumParam param = { 0 };

	param.dwFindID = dwProcessID;
	param.pvhWindow = &vhAllWindow;
	EnumWindows([](_In_ HWND   hwnd, _In_ LPARAM lParam)->BOOL {
		EnumParam *pEnumParam = (EnumParam*)lParam;
		DWORD dwCurID = 0;

		GetWindowThreadProcessId(hwnd, &dwCurID);
		if (dwCurID == pEnumParam->dwFindID)
		{
			pEnumParam->pvhWindow->push_back(hwnd);
		}

		return TRUE;
	}, (LPARAM)&param);
}


int CAntiVirusMark::run()
{

	ULONGLONG ullCurTime = MyPlatform::GetTimeStampMillisecond();

	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:病毒查杀。。。"));
	HWND hSelect = NULL;
	CRect rectSelect;
	::GetWindowRect(hSelect, rectSelect);
	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:寻找选择文件夹窗口"));
	printf("Before rect=[%d,%d, %d,%d]", rectSelect.left, rectSelect.top, rectSelect.right, rectSelect.bottom);
	{
		POINT point = { 238,532 };

		::ClientToScreen(hKismainWnd, &point);
		int nLoopCount = 0;
		do
		{
			BOOL bResult = SetCursorPos(point.x, point.y);
			printf("SetCursorPos x=[%d] y=[%d] error=[%d]\r\n ", point.x, point.y, bResult ? bResult : GetLastError());
			Sleep(1000);

			Sleep(1000);
			doLButton(point.x, point.y);
			Sleep(1000);

#if 1
			std::vector<HWND> vhAllWnd;
			GetProcessAllWindow(dwKismainProcessID, vhAllWnd);
			for (int nIndex = 0; nIndex < (int)vhAllWnd.size(); nIndex++)
			{
				::GetWindowRect(vhAllWnd[nIndex], rectSelect);
				if (120 == rectSelect.Width() && 71 == rectSelect.Height())
				{
					// 					int nImageW = 0, nimageH = 0;
					// 					void* pImageData = NULL;
					// 					GetHWNDImage(vhAllWnd[nIndex], nImageW, nimageH, &pImageData);
					// 					free(pImageData);
					CString strText;
					::GetWindowText(vhAllWnd[nIndex], strText.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
					strText.ReleaseBuffer();
					_tprintf(_T("caption= %s\r\n"), (const TCHAR*)strText);
					::GetClassName(vhAllWnd[nIndex], strText.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
					_tprintf(_T("class= %s\r\n"), (const TCHAR*)strText);
					hSelect = vhAllWnd[nIndex];
					break;
				}

			}
#endif

			nLoopCount++;
			::GetWindowRect(hSelect, rectSelect);
			printf("click rect=[%d,%d, %d,%d]", rectSelect.left, rectSelect.top, rectSelect.right, rectSelect.bottom);
			printf("IsWindowVisible select loop=[%d]\r\n ", nLoopCount);
		} while (nLoopCount < 10 && !::IsWindowVisible(hSelect));
	}
	Sleep(1000);

	HWND hWndSlectDir = NULL;
	SetCursorPos(0, 0);
	
	if (!IsWindow(hSelect) || !::IsWindowVisible(hSelect))
	{
		m_strLastErrMsg.Format(_T("无法找到选择自定义窗口"));
		return -1;
	}

	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:点击选择自定义"));
	{
		CRect rectSelect;
		::GetWindowRect(hSelect, rectSelect);

		rectSelect.top += 40;
		rectSelect.left += 60;

		SetCursorPos(rectSelect.left, rectSelect.top);
		Sleep(1000);
		doLButton(rectSelect.left, rectSelect.top);
		Sleep(1000);


		int nLoop = 0;
		do
		{
			std::vector<HWND> vhAllWnd;
			GetProcessAllWindow(dwKismainProcessID, vhAllWnd);
			for (int nIndex = 0; nIndex < (int)vhAllWnd.size(); nIndex++)
			{
				CString strText;
				::GetWindowText(vhAllWnd[nIndex], strText.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
				strText.ReleaseBuffer();
				if (_T("浏览文件夹") == strText)
				{
					_tprintf(_T("caption= %s\r\n"), (const TCHAR*)strText);
					::GetClassName(vhAllWnd[nIndex], strText.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
					_tprintf(_T("class= %s\r\n"), (const TCHAR*)strText);
					hWndSlectDir = vhAllWnd[nIndex];
					break;
				}

			}
			nLoop++;
			printf("FindWindow 浏览文件夹 loop=[%d]\r\n ", nLoop);
			Sleep(100);
		} while (nLoop < 10 && !::IsWindowVisible(hWndSlectDir));

	}

	
	if (!::IsWindow(hWndSlectDir) || !::IsWindowVisible(hWndSlectDir))
	{
		m_strLastErrMsg.Format(_T("无法找到浏览文件夹窗口"));
		return -2;
	}
	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:选择目录"));
	{
		//std::vector<HWND> vAllChildWnd;
		//GetAllChildWindow(hWndSlectDir, vAllChildWnd);
		//Func(vAllChildWnd[0]);
		POINT pointFind;
		printf("FindImagePositionFromHWnd find \"C\" \r\n");
		
		if (0 != FindImagePositionFromHWnd(hWndSlectDir, m_szDriveCImageFP, pointFind.x, pointFind.y)
			&& 0 != FindImagePositionFromHWnd(hWndSlectDir, m_szDriveCCNImageFP,pointFind.x, pointFind.y))
		{
			m_strLastErrMsg.Format(_T("无法找到\"C:\""));
			return -3;
		}
		printf("FindImagePositionFromHWnd find \"C\" success \r\n");
		::ClientToScreen(hWndSlectDir, &pointFind);

		pointFind.x -= 45;
		pointFind.y += 4;
		SetCursorPos(pointFind.x, pointFind.y);
		Sleep(200);
		doLButton(pointFind.x, pointFind.y);
		Sleep(1000);

		printf("FindImagePositionFromHWnd find \"C:\\AAA\" success \r\n");
		if (0 != FindImagePositionFromHWnd(hWndSlectDir, m_szFindDirImageFP, pointFind.x, pointFind.y))
		{
			m_strLastErrMsg.Format(_T("无法找到\"AAA\"文件夹"));
			return -4;
		}
		SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:开始杀毒"));
		printf("FindImagePositionFromHWnd find \"C:\\AAA\" success \r\n");
		::ClientToScreen(hWndSlectDir, &pointFind);

		pointFind.x -= 19;
		pointFind.y += 4;
		SetCursorPos(pointFind.x, pointFind.y);
		Sleep(200);
		doLButton(pointFind.x, pointFind.y);
		Sleep(1000);

		pointFind = { 282, 363 };
		::ClientToScreen(hWndSlectDir, &pointFind);
		SetCursorPos(pointFind.x, pointFind.y);
		Sleep(200);
		doLButton(pointFind.x, pointFind.y);
		Sleep(1000);


		const TCHAR* szResultImage = _T("result.bmp");
		DWORD dwMaxTime = 1000 * 60 * 10;
		DWORD dwRunTime = 0;
		SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:杀毒中。。。。"));
		do
		{
			int nImageW = 0;
			int nImageH = 0;
			void* pImageData = NULL;
			CImage image;

			GetHWNDImage(hKismainWnd, nImageW, nImageH, &pImageData);
			image.Destroy();
			image.Create(nImageW, -nImageH, 32);
			memcpy(image.GetBits(), pImageData, nImageW * nImageH * 4);
			free(pImageData);
			image.Save(szResultImage, Gdiplus::ImageFormatBMP);
			Sleep(1000 * 2);
			dwRunTime += 2000;
		} while (dwRunTime < dwMaxTime && 0 != FindImagePositionFromHWnd(hKismainWnd, szResultImage, pointFind.x, pointFind.y));
		if (dwRunTime >= dwMaxTime)
		{
			m_strLastErrMsg.Format(_T("无法结束杀毒"));
			return -5;
		}
		SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("金山毒霸:杀毒结束"));
		printf("close check\r\n");
		pointFind = { 787, 19 };
		::ClientToScreen(hKismainWnd, &pointFind);
		SetCursorPos(pointFind.x, pointFind.y);
		Sleep(200);
		doLButton(pointFind.x, pointFind.y);
		Sleep(1000);

		printf("close window\r\n");
		pointFind = { 787, 19 };
		::ClientToScreen(hKismainWnd, &pointFind);
		SetCursorPos(pointFind.x, pointFind.y);
		Sleep(200);
		doLButton(pointFind.x, pointFind.y);
		Sleep(1000);

		printf("over\r\n");

	}
	m_nRunTime = MyPlatform::GetTimeStampMillisecond() - ullCurTime;
	return BENCH_MARK_OK;
}

int CAntiVirusMark::destroy()
{

	return BENCH_MARK_OK;
}
