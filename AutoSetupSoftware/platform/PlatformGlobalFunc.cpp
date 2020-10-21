#include "stdafx.h"

#include "PlatformHeader.h"
#include "PlatformGlobalFunc.h"
namespace MyPlatform
{

#include <shlwapi.h>
#include <commdlg.h>
#include <commdlg.inl>
#include <shellapi.h>
#include <tlhelp32.h>
using namespace std;

int ScreenToClient(HWND hWnd, RECT* pRect)
{
	::ScreenToClient(hWnd, (LPPOINT)pRect);
	::ScreenToClient(hWnd, (LPPOINT)pRect + 1);
	return 1;
}

int  CreateFullDirectoryA(const char* szPath)
{
	if (NULL == szPath)
		return -1;

	if (0 == _access(szPath, 0))
	{
		return PLATFORM_FUNC_NOERROR;
	}

	char szDir[MAX_PATH] = {0};
	strcpy_s(szDir, sizeof(szDir) / sizeof(szDir[0]), szPath);

	char* szCur = szDir;
	while ('\0' != *szCur)
	{
		if ('/' == *szCur)
			*szCur = '\\';
		szCur++;
	}

	szCur = szDir;
	if (':' == *(szDir + 1))
		szCur += 3;

	while('\0' != *szCur)
	{
		szCur = strchr(szCur, '\\');
		if (NULL == szCur)
			break;
		*szCur = '\0';

		if (0 != _access(szDir, 0))
		{
			if (FALSE == CreateDirectoryA(szDir, NULL))
			{
				int nErr = GetLastError();
				if (ERROR_ALREADY_EXISTS != nErr)
				{
					MyTraceA(L0, "error: CreateFullDirectory CreateDirectory failed!errno=[%d] szPath=[%s]\n", nErr, szDir);
					return -2;
				}
			}
		}

		*szCur = '\\';
		szCur++;
	}

	if (FALSE == CreateDirectoryA(szPath, NULL))
	{
		int nErr = GetLastError();
		if (ERROR_ALREADY_EXISTS != nErr)
		{
			MyTraceA(L0, "error: CreateFullDirectory CreateDirectory failed!errno=[%d] szPath=[%s]\n", nErr, szDir);
			return -2;
		}
	}

	return PLATFORM_FUNC_NOERROR;
}

int CreateFullDirectoryW( const wchar_t* szPath )
{
	if (NULL == szPath)
		return -1;

	if (0 == _waccess(szPath, 0))
	{
		return PLATFORM_FUNC_NOERROR;
	}

	wchar_t szDir[MAX_PATH] = {0};
	wcscpy_s(szDir, sizeof(szDir) / sizeof(szDir[0]), szPath);

	wchar_t* szCur = szDir;
	while ('\0' != *szCur)
	{
		if ('/' == *szCur)
			*szCur = '\\';
		szCur++;
	}

	szCur = szDir;
	if (':' == *(szDir + 1))
		szCur += 3;

	while(L'\0' != *szCur)
	{
		szCur = wcschr(szCur, L'\\');
		if (NULL == szCur)
			break;
		*szCur = '\0';

		if (0 != _waccess(szDir, 0))
		{
			if (FALSE == CreateDirectoryW(szDir, NULL))
			{
				int nErr = GetLastError();
				if (ERROR_ALREADY_EXISTS != nErr)
				{
					MyTraceW(L0, L"error: CreateFullDirectory CreateDirectory failed!errno=[%d] szPath=[%s]\n", nErr, szDir);
					return -2;
				}
			}
		}

		*szCur = '\\';
		szCur++;
	}

	if (FALSE == CreateDirectoryW(szPath, NULL))
	{
		int nErr = GetLastError();
		if (ERROR_ALREADY_EXISTS != nErr)
		{
			MyTraceW(L0, L"error: CreateFullDirectory CreateDirectory failed!errno=[%d] szPath=[%s]\n", nErr, szDir);
			return -2;
		}
	}

	return PLATFORM_FUNC_NOERROR;
}

double GetDlgItemDouble(HWND hWnd, int nID)
{
	char szData[MAX_PATH] = {0};
	::GetDlgItemTextA(hWnd, nID, szData, MAX_PATH);
	return atof(szData);
}

HWND FindWindowWithTime(const TCHAR* szClassName, const TCHAR* szWinName, int nMaxTime)
{
	int nFindTime = 0;
	HWND hFindWnd = NULL;

	while (nFindTime < nMaxTime)
	{
		if (NULL != (hFindWnd = ::FindWindow(szClassName, szWinName)))
		{
			return hFindWnd;
		}
		nFindTime += 10;
		Sleep(10);
	}

	return NULL;
}

HWND FindWindowExWithTime(HWND hParent, HWND hAfter, const TCHAR* szClassName, const TCHAR* szWinName, int nMaxTime)
{
	int nFindTime = 0;
	HWND hFindWnd = NULL;

	while (nFindTime < nMaxTime)
	{
		if (NULL != (hFindWnd = ::FindWindowEx(hParent, hAfter, szClassName, szWinName)))
		{
			return hFindWnd;
		}
		nFindTime += 10;
		Sleep(10);
	}

	return NULL;
}

int SetDlgItemDouble(HWND hWnd, int nID, double dData)
{
	char szData[MAX_PATH] = {0};
	sprintf_s(szData, MAX_PATH, "%lf", dData);
	::SetDlgItemTextA(hWnd, nID, szData);
	return 1;
}

int InvalidCtrl(HWND hParent, int nID, bool bErase)
{
	HWND hWnd = ::GetDlgItem(hParent, nID);

	RECT rect = {0};
	::GetWindowRect(hWnd, &rect);

	::ScreenToClient(hParent, (LPPOINT)&rect);
	::ScreenToClient(hParent, (LPPOINT)&rect + 1);

	::InvalidateRect(hParent, &rect, bErase);
	return 1;
}


int GetOpenFolderW( wchar_t* szFolder, const wchar_t* szTitle, HWND hParent, const wchar_t* szInitFolder /*= NULL*/ )
{
	BROWSEINFOW bi				= {0};
	bi.hwndOwner				= hParent;
	bi.pszDisplayName			= szFolder;
	bi.lpszTitle				= szTitle;
#ifdef BIF_USENEWUI
	bi.ulFlags					= BIF_USENEWUI ;
#else
	bi.ulFlags				= BIF_RETURNONLYFSDIRS | BIF_EDITBOX ;
#endif
	bi.lParam					= (LPARAM)szInitFolder;

	LPITEMIDLIST  idl			= SHBrowseForFolderW(&bi);
	if (idl == NULL) 
	{
		MyTraceW(L1, L"warning: csGetOpenFolder have not choice the folder!\n");
		return	-1;
	}

	/// 获取路径
	if (FALSE == SHGetPathFromIDListW(idl, szFolder))
	{
		MyTraceW(L0, L"error :csGetOpenFolder SHGetPathFromIDListA execute failed!\n");
		return	-2;
	}

	CoTaskMemFree(idl);

	return	PLATFORM_FUNC_NOERROR;
}


int	GetOpenFolderA( char* szFolder, const char* szTitle, HWND hWnd, const char* szInitFolder)
{
	BROWSEINFOA bi				= {0};
	bi.hwndOwner				= hWnd;
	bi.pszDisplayName			= szFolder;
	bi.lpszTitle				= szTitle;
#ifdef BIF_USENEWUI
	bi.ulFlags					= BIF_USENEWUI ;
#else
	bi.ulFlags				= BIF_RETURNONLYFSDIRS | BIF_EDITBOX ;
#endif
	//bi.ulFlags				= BIF_RETURNONLYFSDIRS | BIF_EDITBOX ;
	bi.lParam					= (LPARAM)szInitFolder;

	LPITEMIDLIST  idl			= SHBrowseForFolderA(&bi);
	if (idl == NULL) 
	{
		MyTraceA(L1, "warning: csGetOpenFolder have not choice the folder!\n");
		return	-1;
	}

	/// 获取路径
	if (FALSE == SHGetPathFromIDListA(idl, szFolder))
	{
		MyTraceA(L0, "error :csGetOpenFolder SHGetPathFromIDListA execute failed!\n");
		return	-2;
	}

	CoTaskMemFree(idl);

	return	PLATFORM_FUNC_NOERROR;
}


string CharToUtf8( const char* szData )
{
	wstring& strTemp = CharToWchar(szData);
	return WcharToUtf8(strTemp.c_str());
}


string Utf8ToChar(const char* szData)
{
	wstring& strTemp = Utf8ToWchar(szData); 
	return WcharToChar(strTemp.c_str());
}

string TcharToChar( const TCHAR *szData )
{
#ifdef _UNICODE
	return WcharToChar(szData);
#else
	return szData;
#endif
}


std::string WcharToChar( const wchar_t* szData )
{
	char *szAnsiData = NULL;
	string strTmp = string();
	if ( szData == NULL)
	{
		return strTmp;
	}

	DWORD dwMinSize	= 0;
	int nCodepage		= 0;

	nCodepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	dwMinSize = WideCharToMultiByte(nCodepage, NULL, szData, -1, NULL, 0, NULL, FALSE);
	szAnsiData = new char[dwMinSize];
	WideCharToMultiByte(nCodepage, NULL, szData, -1, szAnsiData, dwMinSize, NULL, FALSE);
	strTmp.assign(szAnsiData);
	delete[] szAnsiData;

	return strTmp;
}


std::string WcharToUtf8( const wchar_t* szData )
{
	char *szDst = NULL;
	string strTmp = string();
	if ( szData == NULL)
	{
		return strTmp;
	}

	DWORD dwMinSize	= 0;

	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, szData, -1, NULL, 0, NULL, FALSE);
	szDst = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, szData, -1, szDst, dwMinSize, NULL, FALSE);
	strTmp.assign(szDst);
	delete[] szDst;

	return strTmp;
}


tstring WcharToTchar( const wchar_t* szData )
{
#ifdef _UNICODE
	return tstring(szData);
#else
	return WcharToChar(szData);
#endif
}


string TcharToUtf8( const TCHAR *szData )
{
#ifdef UNICODE
	return WcharToUtf8(szData);
#else
	return CharToUtf8(szData);
#endif
	
}

std::wstring CharToWchar( const char* szData )
{
	wstring strDest = wstring();
	if (NULL == szData)
	{
		return strDest;
	}

	wchar_t* szChar		= NULL;
	DWORD dwMinSize		= 0;
	int nCodepage		= 0;

	nCodepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	dwMinSize = MultiByteToWideChar(nCodepage, NULL, szData, -1, NULL,0);
	szChar = new wchar_t[dwMinSize];
	if (NULL != szData)
	{
		MultiByteToWideChar(nCodepage, NULL, szData, -1, szChar, dwMinSize);
		strDest = szChar;
		delete szChar;
	}

	return strDest;
}


std::wstring Utf8ToWchar( const char* szData )
{
	wstring strDst;
	if (NULL == szData)
	{
		return strDst;
	}

	DWORD dwMinSize		= 0;
	wchar_t* szChar		= NULL;

	dwMinSize = MultiByteToWideChar(CP_UTF8, NULL, szData, -1, NULL,0);
	szChar = new wchar_t[dwMinSize];
	if (NULL != szData)
	{
		MultiByteToWideChar(CP_UTF8, NULL, szData, -1, szChar, dwMinSize);
		strDst = szChar;
		delete szChar;
	}

	return strDst;
}

std::string Unicode2Utf8(const char* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUtf8, len, NULL,NULL);
	std::string ret(szUtf8);
	free(szUtf8);

	return ret;
}

std::wstring TcharToWchar( const TCHAR* szData )
{
#ifdef _UNICODE
	return szData;
#else
	return CharToWchar(szData);
#endif
}

tstring CharToTchar(const char* szData)
{

#ifdef UNICODE
	return CharToWchar(szData);
#else
	return szData;
#endif
	
}

int GetFullPathUnderExeA( const char* szName, char* szPath, int nSize /*= _MAX_PATH*/ )
{
	GetModuleFileNameA(GetModuleHandleA(NULL), szPath, nSize);
	sprintf_s(strrchr(szPath, '\\') + 1, nSize - strlen(szPath), "%s", szName);
	return 1;
}

int GetFullPathUnderExeW( const wchar_t* szName, wchar_t* szPath, int nBufferLen/*= _MAX_PATH*/ )
{
	GetModuleFileNameW(GetModuleHandleW(NULL), szPath, nBufferLen);
	swprintf_s(wcsrchr(szPath, L'\\') + 1, nBufferLen - wcslen(szPath), L"%s", szName);
	return 1;
}

std::string Int64ToStringA( __int64 nData )
{
	string strData		= "";
	char szData[MAX_PATH]	= {0};
	sprintf_s(szData, MAX_PATH, "%I64u", nData);
	strData = szData;

	return strData;
}


std::wstring Int64ToStringW( __int64 nData )
{
	wstring strData		= L"";
	wchar_t szData[MAX_PATH]	= {0};
	swprintf_s(szData, MAX_PATH, L"%I64u", nData);
	strData = szData;
	
	return strData;
}


tstring Utf8ToTchar( const char* szData )
{
#ifdef _UNICODE
	return Utf8ToWchar(szData);
#else
	return Utf8ToChar(szData);
#endif
}

int GetPngImgWAndH(const TCHAR* szImgFP, int* pnWidth, int* pnHeight)
{
	unsigned char pcTmpW[4]	={'0'};   /// < 文件宽度
	unsigned char pcTmpH[4]	={'0'};   /// < 文件高度

	FILE* fp = NULL;
	_tfopen_s(&fp, szImgFP, _T("rb"));
	if (NULL == fp)
	{
		return -1;
	}

	fseek(fp, 0, SEEK_SET);
	fseek(fp, 16, SEEK_CUR);
	fread(pcTmpW, 1, 4, fp);
	fread(pcTmpH, 1, 4, fp);

	*pnWidth		= (int)(pnWidth[2] * 256 + pnWidth[3]);
	*pnHeight		= (int)(pnHeight[2] * 256 + pnHeight[3]);

	return PLATFORM_FUNC_NOERROR;
}

int GetJpgImgWAndH(const TCHAR* szImgFP, int* pnWidth, int* pnHeight)
{
	FILE* fp = NULL;
	int nRet = PLATFORM_FUNC_NOERROR;
	int nFileLen = 0;
	unsigned char* pcImgData = NULL;
	unsigned char* pcDataPos = NULL;

	_tfopen_s(&fp, szImgFP, _T("rb"));
	if (NULL == fp)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	nFileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	pcImgData = (unsigned char*)malloc(nFileLen);
	if (NULL == pcImgData)
	{
		return -2;
	}
	fread(pcImgData, nFileLen, 1, fp);
	pcDataPos = pcImgData;
	for ( int nIdx=0; nIdx < nFileLen - 9; ++nIdx )
	{
		if ( pcDataPos[nIdx]==0xFF && pcDataPos[nIdx+1]==0xC0 )
		{
			pcDataPos = pcImgData + nIdx;
			break;
		}
	}
	if (pcDataPos != pcImgData)
	{
		*pnHeight		= pcDataPos[5] * 255 + pcDataPos[6];
		*pnWidth		= pcDataPos[7] * 255 + pcDataPos[8];
	}
	nRet = pcDataPos != pcImgData ? PLATFORM_FUNC_NOERROR : -3;

	return nRet;
}

int GetImgWAndH(const TCHAR* szImgFP, int* pnWidth, int* pnHeight)
{
	FILE* fp = NULL;
	_tfopen_s(&fp, szImgFP, _T("rb"));
	if (NULL == fp)
	{
		return -1;
	}


	const char szPngTag[]			= {0x50,0x4e,0x47,0x00};
	const char szJpgTag[]			= {0x4A,0x46,0x49,0x46,0x00};
	const int IMGTAGCOUNT			= 16;
	char pcReadTag[IMGTAGCOUNT]		= {0};
	char szDstTag[8]				= {0};

	fread(pcReadTag, sizeof(pcReadTag), 1, fp);
	FCLOSE(fp);

	/// png
	{
		strncpy_s(szDstTag, sizeof(szDstTag), pcReadTag + 1, 3);
		if (0 == strcmp(szDstTag, szPngTag))
		{
			return GetPngImgWAndH(szImgFP, pnWidth, pnHeight);
		}
	}
	/// jpg
	{
		strncpy_s(szDstTag, sizeof(szDstTag), pcReadTag + 6, 4);
		if (0 == strcmp(szDstTag, szJpgTag))
		{
			return GetJpgImgWAndH(szImgFP, pnWidth, pnHeight);
		}
	}

	pnWidth = pnHeight = 0;
	return -2;
}

int WcharToChar( char** pszDst, const wchar_t* szSrc )
{
	DWORD dwMinSize	= 0;
	int nCodepage		= 0;

	nCodepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	dwMinSize = WideCharToMultiByte(nCodepage, NULL, szSrc, -1, NULL, 0, NULL, FALSE);
	*pszDst  = (char*)malloc(dwMinSize);
	if (NULL == *pszDst)
	{
		return -1;
	}

	WideCharToMultiByte(nCodepage, NULL, szSrc, -1, *pszDst, dwMinSize, NULL,FALSE);

	return PLATFORM_FUNC_NOERROR;
}



E_BookType GetBookTypeByFilePathW(const wchar_t* szFilePath)
{
	char* szCharFP	= NULL;
	E_BookType eBType = E_BT_END;

	if (PLATFORM_FUNC_NOERROR != WcharToChar(&szCharFP, szFilePath))
	{
		return eBType;
	}
	eBType = GetBookTypeByFilePathA(szCharFP);
	FREE(szCharFP);
	return eBType;
}

E_BookType GetBookTypeByFilePathA(const char* szFilePath)
{
	string strFile = szFilePath;

	if (string::npos != strFile.find(".pdz")
		|| string::npos != strFile.find(".pdg")
		|| string::npos != strFile.find(".pdzx"))
	{
		return E_BT_PDG;
	}
	else if (string::npos != strFile.find(".pdf"))
	{
		return E_BT_PDF;
	}
	else if (string::npos != strFile.find(".epub")
		|| string::npos != strFile.find(".txt"))
	{
		return E_BT_EPUB;
	}
	
	return E_BT_END;
}

int GetSafeProcAddress( const HMODULE hDll, const char* szFName, FARPROC* pFunc )
{
	*pFunc = GetProcAddress(hDll, szFName);
	if (NULL == *pFunc)
	{
		MyTraceA(L0, "error: GetSafeProcAddress szFName=[%s] nErrNum=[%d]\n", szFName, GetLastError());
		return -1;
	}

	return PLATFORM_FUNC_NOERROR;
}

#pragma comment(lib, "Shlwapi.lib")

int GetAllFilesUnderDirWithOutFilterA( const char* szDirFP, vector<string>* pvOutFiles, BOOL bChildOnly )
{
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryA(szDirFP))
	{
		return -1;
	}

	char  szFile[MAX_PATH]			= {0};
	char  szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAA wfd			= {0};
	BOOL bRet						= TRUE;
	HANDLE hFind					= NULL;

	strcpy_s(szFormatFile, ARRARY_COUNT(szFormatFile), szDirFP);
	if ('\\' == *(szFormatFile + strlen(szFormatFile) - 1))
	{
		*(szFormatFile + strlen(szFormatFile) - 1) = '\0';
	}
	sprintf_s(szFormatFile, sizeof(szFormatFile) / sizeof(szFormatFile[0]), "%s\\*.*", szDirFP);
	hFind = ::FindFirstFileA(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			sprintf_s(szFile, MAX_PATH, "%s\\%s", szDirFP, wfd.cFileName);
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				string strFPath = szFile;
				pvOutFiles->push_back(strFPath);
			}
			else if (!bChildOnly && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				GetAllFilesUnderDirWithOutFilterA(szFile, pvOutFiles, bChildOnly);
			}
		}
		bRet = ::FindNextFileA(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	return PLATFORM_FUNC_NOERROR;
}

int SplitExtArrayA(const char *szFilter, std::vector<std::string> *pvExt)
{
	if (NULL == pvExt)
	{
		return -1;
	}

	const char *szCurPos = szFilter;
	const char *szPrePos = szFilter;
	pvExt->clear();
	while (NULL != (szCurPos = strchr(szCurPos, L'|')))
	{
		string strTemp(szPrePos, szCurPos - szPrePos);
		szPrePos = ++szCurPos;
		pvExt->push_back(strTemp);
	}
	{
		string strTemp(szPrePos, szFilter + strlen(szFilter) - szPrePos);
		pvExt->push_back(strTemp);
	}

	return PLATFORM_FUNC_NOERROR;
}

int SplitExtArrayW(const wchar_t *szFilter, std::vector<std::wstring> *pvExt)
{
	if (NULL == pvExt)
	{
		return -1;
	}
	const wchar_t *szCurPos = szFilter;
	const wchar_t *szPrePos = szFilter;
	pvExt->clear();
	while (NULL != (szCurPos = wcschr(szCurPos, L'|')))
	{
		wstring strTemp(szPrePos, szCurPos - szPrePos);
		szPrePos = ++szCurPos;
		pvExt->push_back(strTemp);
	}
	{
		wstring strTemp(szPrePos, szFilter + wcslen(szFilter) - szPrePos);
		pvExt->push_back(strTemp);
	}
	return PLATFORM_FUNC_NOERROR;
}

/************************************************************************/
/* 
function：
	GetAllFilesUnderDirA - 获取目录下所有文件
Param
	szDirFP    - 目的文件夹
	pvOutFiles - 找到的文件存放
	szFilter   -需要的文件后缀名以"|"分割 例如： ".exe|.png"
*/
/************************************************************************/
int GetAllFilesUnderDirA( const char* szDirFP, vector<string>* pvOutFiles, const char* szFilter, BOOL bChildOnly )
{
	int nRet = GetAllFilesUnderDirWithOutFilterA(szDirFP, pvOutFiles, bChildOnly);
	if (PLATFORM_FUNC_NOERROR != nRet)
	{
		return nRet;
	}
	if (NULL == szFilter)
	{
		return PLATFORM_FUNC_NOERROR;
	}

	vector<string> vstrFilterExt;
	vector<string> vstrFindFiles;
	const char* szCurPos	= szFilter;
	const char* szPrePos	= szFilter;
	char szExt[_MAX_EXT]	= {0};
	int nFilterCount		= 0;
	int nCount				= 0;

	while(NULL != (szCurPos = strchr(szCurPos, '|')))
	{
		string strTemp(szPrePos, szCurPos - szPrePos);
		szPrePos = ++szCurPos;
		vstrFilterExt.push_back(strTemp);
	}
	{
		string strTemp(szPrePos, szFilter + strlen(szFilter) - szPrePos);
		vstrFilterExt.push_back(strTemp);
	}
 	
 	nFilterCount = (int)vstrFilterExt.size();
	if (0 == nFilterCount)
	{
		return PLATFORM_FUNC_NOERROR;
	}
	vstrFindFiles = *pvOutFiles;
	pvOutFiles->clear();
	nCount = (int)vstrFindFiles.size();
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		_splitpath_s(vstrFindFiles[nIdx].c_str(), NULL, 0, NULL, 0, NULL, 0, szExt, ARRARY_COUNT(szExt));
		for (int nEIdx = 0; nEIdx < nFilterCount; nEIdx++)
		{
			if (0 == _stricmp(szExt, vstrFilterExt[nEIdx].c_str()))
			{
				pvOutFiles->push_back(vstrFindFiles[nIdx]);
				break;
			}
		}
	}

	return PLATFORM_FUNC_NOERROR;
}

int GetAllFilesUnderDirWithOutFilterW( const wchar_t* szDirFP, std::vector<std::wstring>* pvOutFiles, BOOL bChildOnly )
{
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryW(szDirFP))
	{
		return -1;
	}

	wchar_t szFile[MAX_PATH]		= {0};
	wchar_t szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAW wfd			= {0};
	BOOL bRet						= TRUE;
	HANDLE hFind					= NULL;

	wcscpy_s(szFormatFile, ARRARY_COUNT(szFormatFile), szDirFP);
	if (L'\\' == *(szFormatFile + wcslen(szFormatFile) - 1))
	{
		*(szFormatFile + wcslen(szFormatFile) - 1) = L'\0';
	}
	swprintf_s(szFormatFile, _MAX_PATH, L"%s\\*.*", szDirFP);
	hFind = ::FindFirstFileW(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			swprintf_s(szFile, MAX_PATH, L"%s\\%s", szDirFP, wfd.cFileName);
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				wstring strFPath = szFile;
				pvOutFiles->push_back(strFPath);
			}
			else if (!bChildOnly && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				GetAllFilesUnderDirWithOutFilterW(szFile, pvOutFiles);
			}
		}
		bRet = ::FindNextFileW(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	return PLATFORM_FUNC_NOERROR;
}

int GetAllFilesUnderDirW( const wchar_t* szDirFP, std::vector<std::wstring>* pvOutFiles, const wchar_t* szFilter /*= NULL*/, BOOL bChildOnly )
{
	int nRet = GetAllFilesUnderDirWithOutFilterW(szDirFP, pvOutFiles, bChildOnly);
	if (PLATFORM_FUNC_NOERROR != nRet)
	{
		return nRet;
	}
	if (NULL == szFilter)
	{
		return PLATFORM_FUNC_NOERROR;
	}


	vector<wstring> vstrFilterExt;
	const wchar_t* szCurPos	= szFilter;
	const wchar_t* szPrePos	= szFilter;
	vector<wstring> vstrFindFiles;
	wchar_t szExt[_MAX_EXT]	= {0};
	int nFilterCount		= 0;
	int nCount				= 0;

	while(NULL != (szCurPos = wcschr(szCurPos, L'|')))
	{
		wstring strTemp(szPrePos, szCurPos - szPrePos);
		szPrePos = ++szCurPos;
		vstrFilterExt.push_back(strTemp);
	}
	{
		wstring strTemp(szPrePos, szFilter + wcslen(szFilter) - szPrePos);
		vstrFilterExt.push_back(strTemp);
	}
	nFilterCount = (int)vstrFilterExt.size();
	if (0 == nFilterCount)
	{
		return PLATFORM_FUNC_NOERROR;
	}

	vstrFindFiles = *pvOutFiles;
	pvOutFiles->clear();
	nCount = (int)vstrFindFiles.size();
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		_wsplitpath_s(vstrFindFiles[nIdx].c_str(), NULL, 0, NULL, 0, NULL, 0, szExt, ARRARY_COUNT(szExt));
		for (int nFIdx = 0; nFIdx < nFilterCount; nFIdx++)
		{
			if (0 == _wcsicmp(szExt, vstrFilterExt[nFIdx].c_str()))
			{
				pvOutFiles->push_back(vstrFindFiles[nIdx]);
				break;
			}
		}
	}

	return PLATFORM_FUNC_NOERROR;
}


int DeleteDirA( const char* szDirFP )
{
	char  szFile[MAX_PATH]			= {0};
	char  szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAA wfd			= {0};
	BOOL bRet					= TRUE;
	HANDLE hFind					= NULL;

	sprintf_s(szFormatFile, sizeof(szFormatFile) / sizeof(szFormatFile[0]), "%s\\*.*", szDirFP);
	hFind = ::FindFirstFileA(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			sprintf_s(szFile, MAX_PATH, "%s\\%s", szDirFP, wfd.cFileName);
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				DeleteFileA(szFile);
			}
			else
			{
				DeleteDirA(szFile);
			}
		}

		bRet = ::FindNextFileA(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	RemoveDirectoryA(szDirFP);

	return PLATFORM_FUNC_NOERROR;
}

int DeleteDirW( const wchar_t* szDirFP )
{
	wchar_t szFile[MAX_PATH]		= {0};
	wchar_t szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAW wfd			= {0};
	BOOL bRet					= TRUE;
	HANDLE hFind					= NULL;

	swprintf_s(szFormatFile, MAX_PATH, L"%s\\*.*", szDirFP);
	hFind = ::FindFirstFileW(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			swprintf_s(szFile, MAX_PATH, L"%s\\%s", szDirFP, wfd.cFileName);
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				DeleteFileW(szFile);
			}
			else
			{
				DeleteDirW(szFile);
			}
		}

		bRet = ::FindNextFileW(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	RemoveDirectoryW(szDirFP);

	return PLATFORM_FUNC_NOERROR;
}

BYTE toHex(const BYTE &x)  
{  
	return x > 9 ? x -10 + 'A': x + '0';  
}  

BYTE fromHex(const BYTE &x)  
{  
	return isdigit(x) ? x-'0' : x-'A'+10;  
} 

string URLEncode(const string &sIn)  
{  
	string sOut;  
	for( size_t ix = 0; ix < sIn.size(); ix++ )  
	{        
		BYTE buf[4] = {0};  
		if( isalnum( (BYTE)sIn[ix] ) || strchr("-_.!~*'()", sIn[ix]) )  
		{        
			buf[0] = sIn[ix];  
		}
		else if (' ' == sIn[ix])
		{
			buf[0] = '+';
		}
		else  
		{  
			buf[0] = '%';  
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );  
			buf[2] = toHex( (BYTE)sIn[ix] % 16);  
		}  
		sOut += (char *)buf;  
	}  
	return sOut;  
}; 

string URLEncode( const char* szURL )
{
	string strUrl = szURL;
	return URLEncode(strUrl);
}

string URLDecode(const string &sIn)  
{  
	string sOut;  
	for( size_t ix = 0; ix < sIn.size(); ix++ )  
	{  
		BYTE ch = 0;  
		if(sIn[ix]=='%')  
		{  
			ch = (fromHex(sIn[ix+1])<<4);  
			ch |= fromHex(sIn[ix+2]);  
			ix += 2;  
		}  
		else if(sIn[ix] == '+')  
		{  
			ch = ' ';  
		}  
		else  
		{  
			ch = sIn[ix];  
		}  
		sOut += (char)ch;  
	}  
	return sOut;  
} 

string URLDecode(const char* szUrl)
{
	string strUrl = szUrl;
	return URLDecode(strUrl);
}

int GetFileSizeA( const char* szFileP, int* pnSize )
{
	FILE* fp = NULL;
	
	fopen_s(&fp, szFileP, "rb");
	if (NULL == fp)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	*pnSize = ftell(fp);
	FCLOSE(fp);
	return PLATFORM_FUNC_NOERROR;
}

int GetFileSizeW( const wchar_t* szFileP, int* pnSize )
{
	FILE* fp = NULL;

	_wfopen_s(&fp, szFileP, L"rb");
	if (NULL == fp)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	*pnSize = ftell(fp);
	FCLOSE(fp);
	return PLATFORM_FUNC_NOERROR;
}

int GetMyOpenFileNameA( char* szFileName, int nFNSize /*= _MAX_PATH*/, const char* szFilter /*= "All Files\0*.*\0\0"*/, const char* szTitle /*= NULL*/, HWND hParent /*= NULL*/ )
{
	OPENFILENAMEA ofn		= {0};
	ofn.lStructSize			= sizeof(ofn);
	ofn.hwndOwner			= hParent;
	ofn.lpstrFilter			= szFilter;
	ofn.lpstrFile			= szFileName;
	ofn.nMaxFile			= nFNSize;
	ofn.lpstrTitle			= szTitle;
	ofn.Flags				= OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	if (0 == GetOpenFileNameA(&ofn))
	{
		return -1;
	}

	return PLATFORM_FUNC_NOERROR;
}

int GetMyOpenFileNameW( wchar_t* szFileName, int nFNSize /*= _MAX_PATH*/, const wchar_t* szFilter /*= L"All Files\0*.*\0\0"*/, const wchar_t* szTitle /*= NULL*/, HWND hParent /*= NULL*/ )
{
	OPENFILENAMEW ofn		= {0};
	ofn.lStructSize			= sizeof(ofn);
	ofn.hwndOwner			= hParent;
	ofn.lpstrFilter			= szFilter;
	ofn.lpstrFile			= szFileName;
	ofn.nMaxFile			= nFNSize;
	ofn.lpstrTitle			= szTitle;
	ofn.Flags				= OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	if (FALSE == GetOpenFileNameW(&ofn))
	{
		DWORD dwErr = CommDlgExtendedError();
		return -1;
	}

	return PLATFORM_FUNC_NOERROR;
}

int GetAllFileDataA( const char* szFilePath, void** ppData, bool bZeroEnd )
{
	FILE* fp	= NULL;
	int nFLen	= 0;

	fopen_s(&fp, szFilePath, "rb");
	if (NULL == fp)
	{
		MyTraceA(L0, "GetAllFileDataA fopen_s failed, szFile=[%s] err=[%d]", szFilePath, errno);
		*ppData = NULL;
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	nFLen = ftell(fp) + (bZeroEnd ? 2 : 0);
	*ppData = malloc(nFLen);
	if (NULL == *ppData)
	{
		MyTraceA(L0, "GetAllFileDataA malloc failed, szFile=[%s] nFLen=[%d]", szFilePath, nFLen);
		*ppData = NULL;
		FCLOSE(fp);
		return -2;
	}
	memset(*ppData, 0, nFLen);
	fseek(fp, 0, SEEK_SET);
	fread(*ppData, nFLen, 1, fp);

	FCLOSE(fp);
	return PLATFORM_FUNC_NOERROR;
}

int GetAllFileDataW( const wchar_t* szFilePath, void** ppData, bool bZeroEnd )
{
	FILE* fp	= NULL;
	int nFLen	= 0;

	_wfopen_s(&fp, szFilePath, L"rb");
	if (NULL == fp)
	{
		MyTraceW(L0, L"GetAllFileDataW _wfopen_s failed, szFile=[%s] err=[%d]", szFilePath, errno);
		*ppData = NULL;
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	nFLen = ftell(fp) + (bZeroEnd ? 2 : 0);
	*ppData = malloc(nFLen);
	if (NULL == *ppData)
	{
		MyTraceW(L0, L"GetAllFileDataW malloc failed, szFile=[%s] nFLen=[%d]", szFilePath, nFLen);
		*ppData = NULL;
		FCLOSE(fp);
		return -2;
	}
	memset(*ppData, 0, nFLen);
	fseek(fp, 0, SEEK_SET);
	fread(*ppData, nFLen, 1, fp);

	FCLOSE(fp);
	return PLATFORM_FUNC_NOERROR;
}


double mywtof( const wchar_t* szSrc )
{
	return atof(WcharToChar(szSrc).c_str());	
}

bool DestoryDirectory(const TCHAR* pDir)
{
	SHFILEOPSTRUCT sop;
	sop.wFunc = FO_DELETE;
	TCHAR buf[MAX_PATH];
	memset(buf, 0, MAX_PATH*sizeof(TCHAR));
	_tcscpy_s(buf, ARRARY_COUNT(buf), pDir);

	sop.pFrom = buf;
	sop.pTo = NULL;
	sop.fFlags = /*FOF_ALLOWUNDO |*/ FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI;

	return !SHFileOperation(&sop);
}

int KillProcessFromName(const TCHAR *szProcessName)
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
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			::TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	}
	return PLATFORM_FUNC_NOERROR;
}

int GetDirUnderDirA(const char* szParentDir, std::vector<std::string>* pvstrOutDirFP, bool bChildOnly /*= true*/)
{
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryA(szParentDir))
	{
		return -1;
	}
	
	char  szDirFP[MAX_PATH]			= {0};
	char  szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAA wfd			= {0};
	BOOL bRet						= TRUE;
	HANDLE hFind					= NULL;
	
	sprintf_s(szFormatFile, sizeof(szFormatFile) / sizeof(szFormatFile[0]), "%s\\*.*", szParentDir);
	hFind = ::FindFirstFileA(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			sprintf_s(szDirFP, MAX_PATH, "%s\\%s", szParentDir, wfd.cFileName);
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				string strFPath = szDirFP;
				pvstrOutDirFP->push_back(strFPath);
				if (!bChildOnly)
				{
					GetDirUnderDirA(szDirFP, pvstrOutDirFP, bChildOnly);
				}
			}
		}
		bRet = ::FindNextFileA(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	return PLATFORM_FUNC_NOERROR;
}

int GetDirUnderDirW( const wchar_t* szParentDir, std::vector<std::wstring>* pvstrOutDirFP, bool bChildOnly /*= true*/ )
{
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryW(szParentDir))
	{
		return -1;
	}
	
	wchar_t szDirFP[MAX_PATH]		= {0};
	wchar_t szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAW wfd			= {0};
	BOOL bRet						= TRUE;
	HANDLE hFind					= NULL;
	
	swprintf_s(szFormatFile, _MAX_PATH, L"%s\\*.*", szParentDir);
	hFind = ::FindFirstFileW(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return -1;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			swprintf_s(szDirFP, MAX_PATH, L"%s\\%s", szParentDir, wfd.cFileName);
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				wstring strFPath = szDirFP;
				pvstrOutDirFP->push_back(strFPath);
				if (!bChildOnly)
				{
					GetDirUnderDirW(szDirFP, pvstrOutDirFP, bChildOnly);
				}
			}
		}
		bRet = ::FindNextFileW(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	return PLATFORM_FUNC_NOERROR;
}

typedef struct _tagEnumWindowProcParam  
{
	void *pClassName;
	void *pWndName;
	std::vector<HWND> *pvhwndFind;
}EnumWindowProcParam;

BOOL CALLBACK EnumWindowsProcW(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{
	EnumWindowProcParam* pParam = (EnumWindowProcParam*)lParam;
	const wchar_t *szCalssName = NULL;
	const wchar_t *szWndName = NULL;
	std::vector<HWND> *pvhwndFind = NULL;
	wchar_t szCurWndClassName[_MAX_PATH] = { 0 };
	wchar_t szCurWndWndName[_MAX_PATH] = { 0 };

	if (NULL == pParam)
	{
		return FALSE;
	}
	szCalssName = (const wchar_t*)pParam->pClassName;
	szWndName = (const wchar_t *)pParam->pWndName;
	pvhwndFind = pParam->pvhwndFind;

	GetClassNameW(hwnd, szCurWndClassName, _ARRAYSIZE(szCurWndClassName));
	GetWindowTextW(hwnd, szCurWndWndName, _ARRAYSIZE(szCurWndWndName));
	if (0 == _wcsicmp(szCalssName, szCurWndClassName)
		&& NULL != wcsstr(szCurWndWndName, szWndName)
		)
	{
		pvhwndFind->push_back(hwnd);
	}


	return TRUE;
}

BOOL CALLBACK EnumWindowsProcA(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{
	EnumWindowProcParam* pParam = (EnumWindowProcParam*)lParam;
	const char *szCalssName = NULL;
	const char *szWndName = NULL;
	std::vector<HWND> *pvhwndFind = NULL;
	char szCurWndClassName[_MAX_PATH] = { 0 };
	char szCurWndWndName[_MAX_PATH] = { 0 };

	if (NULL == pParam)
	{
		return FALSE;
	}
	szCalssName = (const char*)pParam->pClassName;
	szWndName = (const char *)pParam->pWndName;
	pvhwndFind = pParam->pvhwndFind;

	GetClassNameA(hwnd, szCurWndClassName, _ARRAYSIZE(szCurWndClassName));
	GetWindowTextA(hwnd, szCurWndWndName, _ARRAYSIZE(szCurWndWndName));
	if (0 == _stricmp(szCalssName, szCurWndClassName)
		&& NULL != strstr(szCurWndWndName, szWndName)
		)
	{
		pvhwndFind->push_back(hwnd);
	}


	return TRUE;
}

int FindWindowWithClassNameAndWindowNameW(const wchar_t *szClassName, const wchar_t* szWndName, bool bPartialWndName, std::vector<HWND> *pvhwndFind)
{
	if (NULL == pvhwndFind)
	{
		return -1;
	}

	if (!bPartialWndName)
	{
		HWND hWnd = FindWindowW(szClassName, szWndName);
		if (NULL == hWnd)
		{
			return -2;
		}
		pvhwndFind->push_back(hWnd);
		return PLATFORM_FUNC_NOERROR;
	}
	else
	{
		EnumWindowProcParam param;

		int nPreCount = pvhwndFind->size();
		param.pClassName = (void*)szClassName;
		param.pWndName = (void*)szWndName;
		param.pvhwndFind = pvhwndFind;
		EnumWindows(EnumWindowsProcW, (LPARAM)&param);
		if (nPreCount == pvhwndFind->size())
		{
			return -2;
		}
		return PLATFORM_FUNC_NOERROR;
	}


	return PLATFORM_FUNC_NOERROR;
}

int FindWindowWithClassNameAndWindowNameA(const char *szClassName, const char* szWndName, bool bPartialWndName, std::vector<HWND> *pvhwndFind)
{
	if (NULL == pvhwndFind)
	{
		return -1;
	}

	if (!bPartialWndName)
	{
		HWND hWnd = FindWindowA(szClassName, szWndName);
		if (NULL == hWnd)
		{
			return -2;
		}
		pvhwndFind->push_back(hWnd);
		return PLATFORM_FUNC_NOERROR;
	}
	else
	{
		EnumWindowProcParam param;

		int nPreCount = pvhwndFind->size();
		param.pClassName = (void*)szClassName;
		param.pWndName = (void*)szWndName;
		param.pvhwndFind = pvhwndFind;
		EnumWindows(EnumWindowsProcA, (LPARAM)&param);
		if (nPreCount == pvhwndFind->size())
		{
			return -2;
		}
		return PLATFORM_FUNC_NOERROR;
	}
	return PLATFORM_FUNC_NOERROR;
}

int GetWndInDlgWndByNameA(std::vector<HWND>* pvhFind, HWND hParent, const char* szCtrlCName, const char* szCtrlTitle, BOOL bInChildDlg)
{
	HWND hChild				= hParent;
	HWND hCtrl				= NULL;
	char szCName[_MAX_PATH] =	{0};
	char szTitle[_MAX_PATH]	= {0};
	
	hChild = ::GetWindow(hChild, GW_CHILD);
	do 
	{
		if (NULL == hChild)
		{
			break;
		}
		::GetClassNameA(hChild, szCName, sizeof(szCName) / sizeof(szCName[0]));
		::GetWindowTextA(hChild, szTitle, sizeof(szTitle) / sizeof(szTitle[0]));

		if ((NULL == szCtrlTitle) && 0 == _stricmp(szCtrlCName, szCName))
		{
			pvhFind->push_back(hChild);
		}
		else if ((NULL == szCtrlCName) && 0 == _stricmp(szCtrlTitle, szTitle))
		{
			pvhFind->push_back(hChild);
		}
		else if (NULL != szCtrlTitle && NULL != szCtrlCName 
			&& 0 == _stricmp(szCtrlCName, szCName) && 0 == _stricmp(szCtrlTitle, szTitle))
		{
			pvhFind->push_back(hChild);
		}
		if (bInChildDlg && 0 == _stricmp(szCName, "#32770"))
		{
			GetWndInDlgWndByNameA(pvhFind, hChild, szCtrlCName, szCtrlTitle, bInChildDlg);
		}
	} while (hChild = ::GetWindow(hChild, GW_HWNDNEXT));
	
	return PLATFORM_FUNC_NOERROR;
}

int GetWndInDlgWndByNameW(std::vector<HWND>* pvhFind, HWND hParent, const wchar_t* szCtrlCName, const wchar_t* szCtrlTitle, BOOL bInChildDlg )
{
	HWND hChild				= hParent;
	HWND hCtrl				= NULL;
	wchar_t szCName[_MAX_PATH] =	{0};
	wchar_t szTitle[_MAX_PATH]	= {0};
	
	hChild = ::GetWindow(hChild, GW_CHILD);
	do 
	{
		if (NULL == hChild)
		{
			break;
		}
		::GetClassNameW(hChild, szCName, sizeof(szCName) / sizeof(szCName[0]));
		::GetWindowTextW(hChild, szTitle, sizeof(szTitle) / sizeof(szTitle[0]));
		
		if ((NULL == szCtrlTitle) && 0 == _wcsicmp(szCtrlCName, szCName))
		{
			pvhFind->push_back(hChild);
		}
		else if ((NULL == szCtrlCName) && 0 == _wcsicmp(szCtrlTitle, szTitle))
		{
			pvhFind->push_back(hChild);
		}
		else if (NULL != szCtrlTitle && NULL != szCtrlCName 
			&& 0 == _wcsicmp(szCtrlCName, szCName) && 0 == _wcsicmp(szCtrlTitle, szTitle))
		{
			pvhFind->push_back(hChild);
		}
		if (bInChildDlg && 0 == _wcsicmp(szCName, L"#32770"))
		{
			GetWndInDlgWndByNameW(pvhFind, hChild, szCtrlCName, szCtrlTitle, bInChildDlg);
		}
	} while (hChild = ::GetWindow(hChild, GW_HWNDNEXT));
	
	return PLATFORM_FUNC_NOERROR;
}

int WriteFileDataA( const char* szFilePath, const void* pData, int nLen, char* szMode )
{
	FILE* fp = NULL;

	fopen_s(&fp, szFilePath, szMode);
	if (NULL == fp)
	{
		MyTraceA(L0, "WriteFileDataA szFilePath=[%s] szMode[%s]", szFilePath, szMode);
		return -1;
	}
	
	fwrite(pData, nLen, 1, fp);
	FCLOSE(fp);
	return PLATFORM_FUNC_NOERROR;
}

int WriteFileDataW( const wchar_t* szFilePath, const void* pData, int nLen, wchar_t* szMode )
{
	FILE* fp = NULL;
	
	_wfopen_s(&fp, szFilePath, szMode);
	if (NULL == fp)
	{
		MyTraceW(L0, L"WriteFileDataW szFilePath=[%s] szMode[%s]", szFilePath, szMode);
		return -1;
	}
	
	fwrite(pData, nLen, 1, fp);
	FCLOSE(fp);
	return PLATFORM_FUNC_NOERROR;
}

BOOL IsStrHasChineseA( const char* szStr )
{
	int nLen = (int)strlen(szStr);
	for (int nIdx = 0; nIdx < nLen; nIdx++)
	{
		if ((szStr[nIdx] > 0x80) || (szStr[nIdx] < 0))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL IsStrHasChineseW( const wchar_t* szStr )
{
	int nLen = (int)wcslen(szStr);
	for (int nIdx = 0; nIdx < nLen; nIdx++)
	{
		if (((char*)(szStr + nIdx))[1] != 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IsDirHaveThisExtFileA( const char* szDirFP, const char* szExt, BOOL bChildOnly /*= FALSE*/ )
{
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryA(szDirFP))
	{
		return FALSE;
	}
	
	char  szFile[MAX_PATH]			= {0};
	char  szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAA wfd			= {0};
	BOOL bRet						= TRUE;
	HANDLE hFind					= NULL;
	BOOL bFind						= FALSE;
	char* szFileExtPos					= NULL;

	vector<string> vstrFilterExt;
	const char* szCurPos	= szExt;
	const char* szPrePos	= szExt;
	int nCount				= 0;
	
	while(NULL != (szCurPos = strchr(szCurPos, '|')))
	{
		string strTemp(szPrePos, szCurPos - szPrePos);
		szPrePos = ++szCurPos;
		vstrFilterExt.push_back(strTemp);
	}
	{
		string strTemp(szPrePos, szExt + strlen(szExt) - szPrePos);
		vstrFilterExt.push_back(strTemp);
	}
	nCount = (int)vstrFilterExt.size();
	
	sprintf_s(szFormatFile, sizeof(szFormatFile) / sizeof(szFormatFile[0]), "%s\\*.*", szDirFP);
	hFind = ::FindFirstFileA(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			sprintf_s(szFile, MAX_PATH, "%s\\%s", szDirFP, wfd.cFileName);
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (NULL != (szFileExtPos = strrchr(szFile, '.')))
				{
					for (int nIdx = 0; nIdx < nCount; nIdx++)
					{
						if (0 == _stricmp(szFileExtPos, vstrFilterExt[nIdx].c_str()))
						{
							FindClose(hFind);
							return TRUE;
						}
					}
				}
				
			}
			else if (!bChildOnly && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				bFind = IsDirHaveThisExtFileA(szFile, szExt, bChildOnly);
				if (bFind)
				{
					FindClose(hFind);
					return TRUE;
				}
			}
		}
		bRet = ::FindNextFileA(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	return FALSE;
}

BOOL IsDirHaveThisExtFileW( const wchar_t* szDirFP, const wchar_t* szExt, BOOL bChildOnly /*= FALSE*/ )
{
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryW(szDirFP))
	{
		return FALSE;
	}
	
	wchar_t  szFile[MAX_PATH]			= {0};
	wchar_t  szFormatFile[MAX_PATH]	= {0};
	WIN32_FIND_DATAW wfd			= {0};
	BOOL bRet						= TRUE;
	HANDLE hFind					= NULL;
	BOOL bFind						= FALSE;
	wchar_t* szFileExtPos					= NULL;

	vector<wstring> vstrFilterExt;
	const wchar_t* szCurPos	= szExt;
	const wchar_t* szPrePos	= szExt;
	int nCount				= 0;
	
	while(NULL != (szCurPos = wcschr(szCurPos, '|')))
	{
		wstring strTemp(szPrePos, szCurPos - szPrePos);
		szPrePos = ++szCurPos;
		vstrFilterExt.push_back(strTemp);
	}
	{
		wstring strTemp(szPrePos, szExt + wcslen(szExt) - szPrePos);
		vstrFilterExt.push_back(strTemp);
	}
	nCount = (int)vstrFilterExt.size();
	
	swprintf_s(szFormatFile, sizeof(szFormatFile) / sizeof(szFormatFile[0]), L"%s\\*.*", szDirFP);
	hFind = ::FindFirstFileW(szFormatFile, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}
	while (TRUE)
	{
		if (wfd.cFileName[0] != '.')
		{
			swprintf_s(szFile, MAX_PATH, L"%s\\%s", szDirFP, wfd.cFileName);
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (NULL != (szFileExtPos = wcsrchr(szFile, '.')))
				{
					for (int nIdx = 0; nIdx < nCount; nIdx++)
					{
						if (0 == _wcsicmp(szFileExtPos, vstrFilterExt[nIdx].c_str()))
						{
							FindClose(hFind);
							return TRUE;
						}
					}
				}
				
			}
			else if (!bChildOnly && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				bFind = IsDirHaveThisExtFileW(szFile, szExt, bChildOnly);
				if (bFind)
				{
					FindClose(hFind);
					return TRUE;
				}
			}
		}
		bRet = ::FindNextFileW(hFind, &wfd);
		if (!bRet)
		{
			break;
		}
	}
	FindClose(hFind);
	return FALSE;
}

int CopyDirW( const wchar_t* szSrcDir, const wchar_t* szDstDir, BOOL bFailIfExists )
{
	vector<wstring> vstrFiles;
	int nCount = 0, nSrcDirLen = 0;
	wchar_t szNewFileFP[_MAX_PATH]	= {0};
	wchar_t szFileDir[_MAX_PATH]	= {0};
	wchar_t szDrive[_MAX_DRIVE]		= {0};
	wchar_t szDir[_MAX_DIR]			= {0};

	GetAllFilesUnderDirW(szSrcDir, &vstrFiles);
	nCount = (int)vstrFiles.size();
	nSrcDirLen = (int)wcslen(szSrcDir);
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		swprintf_s(szNewFileFP, sizeof(szNewFileFP) / sizeof(szNewFileFP[0]), L"%s%s", szDstDir, vstrFiles[nIdx].c_str() + nSrcDirLen);
		_wsplitpath_s(szNewFileFP, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), NULL, 0, NULL, 0);
		swprintf_s(szFileDir, ARRARY_COUNT(szFileDir), L"%s%s", szDrive, szDir);
		CreateFullDirectoryW(szFileDir);
		CopyFileW(vstrFiles[nIdx].c_str(), szNewFileFP, bFailIfExists);
	}

	return PLATFORM_FUNC_NOERROR;
}

int CopyDirA( const char* szSrcDir, const char* szDstDir, BOOL bFailIfExists )
{
	vector<string> vstrFiles;
	int nCount = 0, nSrcDirLen = 0;
	char szNewFileFP[_MAX_PATH]	= {0};
	char szFileDir[_MAX_PATH]	= {0};
	char szDrive[_MAX_DRIVE]		= {0};
	char szDir[_MAX_DIR]			= {0};
	
	GetAllFilesUnderDirA(szSrcDir, &vstrFiles);
	nCount = (int)vstrFiles.size();
	nSrcDirLen = (int)strlen(szSrcDir);
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		sprintf_s(szNewFileFP, ARRARY_COUNT(szNewFileFP), "%s%s", szDstDir, vstrFiles[nIdx].c_str() + nSrcDirLen);
		_splitpath_s(szNewFileFP, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), NULL, 0, NULL, 0);
		sprintf_s(szFileDir, ARRARY_COUNT(szFileDir), "%s%s", szDrive, szDir);
		CreateFullDirectoryA(szFileDir);
		CopyFileA(vstrFiles[nIdx].c_str(), szNewFileFP, bFailIfExists);
	}
	
	return PLATFORM_FUNC_NOERROR;

}

int MoveDirW( const wchar_t* szSrcDir, const wchar_t* szDstDir, BOOL bFailIfExists )
{
	vector<wstring> vstrFiles;
	int nCount = 0, nSrcDirLen = 0;
	wchar_t szNewFileFP[_MAX_PATH]	= {0};
	wchar_t szFileDir[_MAX_PATH]	= {0};
	wchar_t szDrive[_MAX_DRIVE]		= {0};
	wchar_t szDir[_MAX_DIR]			= {0};
	
	CreateFullDirectoryW(szDstDir);
	GetAllFilesUnderDirW(szSrcDir, &vstrFiles);
	nCount = (int)vstrFiles.size();
	nSrcDirLen = (int)wcslen(szSrcDir);
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		swprintf_s(szNewFileFP, ARRARY_COUNT(szNewFileFP), L"%s%s", szDstDir, vstrFiles[nIdx].c_str() + nSrcDirLen);
		_wsplitpath_s(szNewFileFP, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), NULL, 0, NULL, 0);
		swprintf_s(szFileDir, ARRARY_COUNT(szFileDir), L"%s%s", szDrive, szDir);
		CreateFullDirectoryW(szFileDir);
		if (!bFailIfExists)
		{
			DeleteFileW(szNewFileFP);
		}
		MoveFileW(vstrFiles[nIdx].c_str(), szNewFileFP);
	}
	DeleteDirW(szSrcDir);
	return PLATFORM_FUNC_NOERROR;
}

int MoveDirA( const char* szSrcDir, const char* szDstDir, BOOL bFailIfExists )
{
	vector<string> vstrFiles;
	int nCount = 0, nSrcDirLen = 0;

	char szNewFileFP[_MAX_PATH]	= {0};
	char szFileDir[_MAX_PATH]	= {0};
	char szDrive[_MAX_DRIVE]	= {0};
	char szDir[_MAX_DIR]		= {0};
	
	CreateFullDirectoryA(szDstDir);
	GetAllFilesUnderDirA(szSrcDir, &vstrFiles);
	nCount = (int)vstrFiles.size();
	nSrcDirLen = (int)strlen(szSrcDir);
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		sprintf_s(szNewFileFP, ARRARY_COUNT(szNewFileFP), "%s%s", szDstDir, vstrFiles[nIdx].c_str() + nSrcDirLen);
		_splitpath_s(szNewFileFP, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), NULL, 0, NULL, 0);
		sprintf_s(szFileDir, ARRARY_COUNT(szFileDir), "%s%s", szDrive, szDir);
		CreateFullDirectoryA(szFileDir);
		if (!bFailIfExists)
		{
			DeleteFileA(szNewFileFP);
		}
		MoveFileA(vstrFiles[nIdx].c_str(), szNewFileFP);
	}
	DeleteDirA(szSrcDir);
	return PLATFORM_FUNC_NOERROR;
}

int DeleteDirPathEndSeparatorA( char* szDirFP )
{
	if (('\\' == *(szDirFP + strlen(szDirFP) - 1))
		|| ('/' == *(szDirFP + strlen(szDirFP) - 1)))
	{
		*(szDirFP + strlen(szDirFP) - 1) = '\0';
	}
	return PLATFORM_FUNC_NOERROR;
}

int DeleteDirPathEndSeparatorW( wchar_t* szDirFP )
{
	if ((L'\\' == *(szDirFP + wcslen(szDirFP) - 1))
		|| (L'/' == *(szDirFP + wcslen(szDirFP) - 1)))
	{
		*(szDirFP + wcslen(szDirFP) - 1) = L'\0';
	}
	return PLATFORM_FUNC_NOERROR;
}

std::string ChangeFilePathExtA( const char* szSrcFP, const char* szDstExt )
{
	char szDrive[_MAX_DRIVE]	= {0};
	char szDir[_MAX_DIR]		= {0};
	char szFName[_MAX_FNAME]	= {0};
	char szExt[_MAX_EXT]		= {0};
	char szDstFP[_MAX_PATH]		= {0};

	_splitpath_s(szSrcFP, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));
	_makepath_s(szDstFP, ARRARY_COUNT(szDstFP), szDrive, szDir, szFName, szDstExt);

	return szDstFP;
}

std::wstring ChangeFilePathExtW( const wchar_t* szSrcFP, const wchar_t* szDstExt )
{
	wchar_t szDrive[_MAX_DRIVE]	= {0};
	wchar_t szDir[_MAX_DIR]		= {0};
	wchar_t szFName[_MAX_FNAME]	= {0};
	wchar_t szExt[_MAX_EXT]		= {0};
	wchar_t szDstFP[_MAX_PATH]	= {0};

	_wsplitpath_s(szSrcFP, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));
	_wmakepath_s(szDstFP, ARRARY_COUNT(szDstFP), szDrive, szDir, szFName, szDstExt);

	return szDstFP;
}

std::string GetFilePathDirA( const char* szFilePath, BOOL bEndSeparator )
{
	char szDrive[_MAX_DRIVE]	= {0};
	char szDir[_MAX_DIR]		= {0};
	char szFName[_MAX_FNAME]	= {0};
	char szExt[_MAX_EXT]		= {0};
	char szDstFP[_MAX_PATH]		= {0};

	_splitpath_s(szFilePath, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));
	sprintf_s(szDstFP, "%s%s", szDrive, szDir);
	
	if (!bEndSeparator)
	{
		DeleteDirPathEndSeparatorA(szDstFP);
	}
	return szDstFP;
}

std::wstring GetFilePathDirW( const wchar_t* szFilePath, BOOL bEndSeparator )
{
	wchar_t szDrive[_MAX_DRIVE]	= {0};
	wchar_t szDir[_MAX_DIR]		= {0};
	wchar_t szFName[_MAX_FNAME]	= {0};
	wchar_t szExt[_MAX_EXT]		= {0};
	wchar_t szDstFP[_MAX_PATH]		= {0};

	_wsplitpath_s(szFilePath, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));
	swprintf_s(szDstFP, L"%s%s", szDrive, szDir);
	if (!bEndSeparator)
	{
		DeleteDirPathEndSeparatorW(szDstFP);
	}
	
	return szDstFP;
}

std::string GetFilePathFullNameA( const char* szFilePath )
{
	char szDrive[_MAX_DRIVE]	= {0};
	char szDir[_MAX_DIR]		= {0};
	char szFName[_MAX_FNAME]	= {0};
	char szExt[_MAX_EXT]		= {0};
	char szDstFP[_MAX_PATH]		= {0};

	_splitpath_s(szFilePath, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));
	sprintf_s(szDstFP, "%s%s", szFName, szExt);

	return szDstFP;
}

std::wstring GetFilePathFullNameW( const wchar_t* szFilePath )
{
	wchar_t szDrive[_MAX_DRIVE]	= {0};
	wchar_t szDir[_MAX_DIR]		= {0};
	wchar_t szFName[_MAX_FNAME]	= {0};
	wchar_t szExt[_MAX_EXT]		= {0};
	wchar_t szDstFP[_MAX_PATH]		= {0};

	_wsplitpath_s(szFilePath, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));
	swprintf_s(szDstFP, L"%s%s", szFName, szExt);

	return szDstFP;
}


std::string GetFilePathExtA(const char* szFilePath)
{
	char szDrive[_MAX_DRIVE]	= {0};
	char szDir[_MAX_DIR]		= {0};
	char szFName[_MAX_FNAME]	= {0};
	char szExt[_MAX_EXT]		= {0};
	char szDstFP[_MAX_PATH]		= {0};

	_splitpath_s(szFilePath, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));

	return szExt;
}

std::wstring GetFilePathExtW(const wchar_t* szFilePath)
{
	wchar_t szDrive[_MAX_DRIVE]	= {0};
	wchar_t szDir[_MAX_DIR]		= {0};
	wchar_t szFName[_MAX_FNAME]	= {0};
	wchar_t szExt[_MAX_EXT]		= {0};

	_wsplitpath_s(szFilePath, szDrive, ARRARY_COUNT(szDrive), szDir, ARRARY_COUNT(szDir), szFName, ARRARY_COUNT(szFName), szExt, ARRARY_COUNT(szExt));

	return szExt;
}


int GetAppPathFromRegAppPath(TCHAR *szAppPath, int nPathCount, const TCHAR *szExeName)
{
	TCHAR szSubKey[_MAX_PATH] = { 0 };

	_stprintf_s(szSubKey, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\%s"), szExeName);
	if (ERROR_SUCCESS != RegGetValue(HKEY_LOCAL_MACHINE, szSubKey, _T(""), RRF_RT_REG_SZ, NULL, szAppPath, (LPDWORD)&nPathCount))
	{
		return - 1;
	}

	return PLATFORM_FUNC_NOERROR;
}

int GetFileCreateTime(const TCHAR *szFilePath, SYSTEMTIME *pstCreate)
{
	HANDLE hFile = NULL;
	FILETIME ftCreate = { 0 };

	hFile = CreateFile(szFilePath, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return -1;
	}

	GetFileTime(hFile, &ftCreate, NULL, NULL);
	FileTimeToSystemTime(&ftCreate, pstCreate);

	CLOSEHANDLE(hFile);
	return PLATFORM_FUNC_NOERROR;
}

int GetFileCreateTimestampSecond(const TCHAR *szFilePath, ULONGLONG *pullCreateTime)
{
	HANDLE hFile = NULL;
	FILETIME ftCreate = { 0 };

	hFile = CreateFile(szFilePath, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return -1;
	}

	GetFileTime(hFile, &ftCreate, NULL, NULL);
	ULARGE_INTEGER ull = { 0 };

	ull.LowPart = ftCreate.dwLowDateTime;
	ull.HighPart = ftCreate.dwHighDateTime;

	*pullCreateTime =((ull.QuadPart - 116444736000000000ULL) / 10000000ULL);

	CLOSEHANDLE(hFile);
	return PLATFORM_FUNC_NOERROR;
}

int GetDispatchTypeName(IDispatchPtr _dispatch, BSTR *pbstrName)
{
	if (NULL == _dispatch)
	{
		return -1;
	}

	ITypeInfo* ptypeInfo = NULL;
	LCID lcidDef = GetUserDefaultLCID();
	HRESULT hr = _dispatch->GetTypeInfo(0, lcidDef, &ptypeInfo);
	if (FAILED(hr))
	{
		return -2;
	}
	BSTR bstrDocString;
	hr = ptypeInfo->GetDocumentation(-1, pbstrName, &bstrDocString, NULL, NULL);
	if (FAILED(hr))
	{
		return -3;
	}
	return PLATFORM_FUNC_NOERROR;
}

bool	SetDlgItemDouble(HWND hWnd, int nIDDlgItem, double dValue, int nPrecision)
{
	TCHAR	szFormat[UCHAR_NUM] = { 0 };
	_stprintf_s(szFormat, _T("%%.%dlf"), nPrecision);
	TCHAR	szValue[UCHAR_NUM] = { 0 };
	_stprintf_s(szValue, szFormat, dValue);
	StrTrim(szValue, _T(" "));
	return	(TRUE == SetDlgItemText(hWnd, nIDDlgItem, szValue));
}

int CreateFileFromResourceA(int nID, char* szType, char* szFile)
{
	HRSRC		hRsrc = NULL;
	HGLOBAL		hSrc = NULL;
	FILE*		fp = NULL;
	void*		pSrc = NULL;

	try
	{
		if (NULL == (hRsrc = FindResourceA(NULL, MAKEINTRESOURCEA(nID), szType)))
		{
			throw - 1;
		}

		if (NULL == (hSrc = LoadResource(NULL, hRsrc)))
		{
			throw - 2;
		}

		if (NULL == (pSrc = LockResource(hSrc)))
		{
			throw - 3;
		}

		int		nSrc = SizeofResource(NULL, hRsrc);
		if (0 == nSrc)
		{
			throw - 4;
		}

		fopen_s(&fp, szFile, "wb");
		if (NULL == fp)
		{
			throw - 5;
		}

		if (nSrc != fwrite(pSrc, 1, nSrc, fp))
		{
			throw - 6;
		}

		throw PLATFORM_FUNC_NOERROR;
	}
	catch (int nErrno)
	{
		FCLOSE(fp);
		if (NULL != pSrc)
		{
			UnlockResource(hSrc);
			pSrc = NULL;
		}

		if (NULL == hSrc)
		{
			FreeResource(hSrc);
			hSrc = NULL;
		}

		return	nErrno;
	}


	return	1;
}

int CreateFileFromResourceW(int nID, wchar_t* szType, wchar_t* szFile)
{
	HRSRC		hRsrc = NULL;
	HGLOBAL		hSrc = NULL;
	FILE*		fp = NULL;
	void*		pSrc = NULL;

	try
	{
		if (NULL == (hRsrc = FindResourceW(NULL, MAKEINTRESOURCEW(nID), szType)))
		{
			throw - 1;
		}

		if (NULL == (hSrc = LoadResource(NULL, hRsrc)))
		{
			throw - 2;
		}

		if (NULL == (pSrc = LockResource(hSrc)))
		{
			throw - 3;
		}

		int		nSrc = SizeofResource(NULL, hRsrc);
		if (0 == nSrc)
		{
			throw - 4;
		}

		_wfopen_s(&fp, szFile, L"wb");
		if (NULL == fp)
		{
			throw - 5;
		}

		if (nSrc != fwrite(pSrc, 1, nSrc, fp))
		{
			throw - 6;
		}

		throw PLATFORM_FUNC_NOERROR;
	}
	catch (int nErrno)
	{
		FCLOSE(fp);
		if (NULL != pSrc)
		{
			UnlockResource(hSrc);
			pSrc = NULL;
		}

		if (NULL == hSrc)
		{
			FreeResource(hSrc);
			hSrc = NULL;
		}

		return	nErrno;
	}


	return	1;
}

ULONGLONG GetTimeStampMillisecond(void)

{
	FILETIME ft = { 0 };
	SYSTEMTIME st = { 0 };
	ULARGE_INTEGER ull = { 0 };

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	return ((ull.QuadPart - 116444736000000000ULL) / 10000000ULL) * 1000 + st.wMilliseconds;
}

#include "DbgHelp.h"
#pragma comment(lib, "DbgHelp.lib")
//使所有版本都可以捕获到异常 
void DisableSetUnhandledExceptionFilter()
{
	void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")), "SetUnhandledExceptionFilter");

	if (addr)
	{
		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}


//程序未捕获的异常处理函数 
int ExceptionWriteDump(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	TCHAR szExePath[_MAX_PATH] = { 0 };
	TCHAR *szFNamePos = NULL;

	GetModuleFileName(NULL, szExePath, _MAX_PATH);
	if (NULL != (szFNamePos = _tcsrchr(szExePath, _T('\\'))))
	{
		szFNamePos++;
		_tcscpy_s(szFNamePos, _ARRAYSIZE(szFNamePos), _T("information.dmp"));
	}
	else
	{
		_tcscpy_s(szExePath, _ARRAYSIZE(szExePath), _T("C:\\information.dmp"));
	}

	HANDLE hFile = ::CreateFile(szExePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId = ::GetCurrentThreadId();
		einfo.ExceptionPointers = ExceptionInfo;
		einfo.ClientPointers = FALSE;

		MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &einfo, NULL, NULL);
		::CloseHandle(hFile);
	}

	return 0;
}

};
