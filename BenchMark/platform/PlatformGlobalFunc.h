
#ifndef WYD_GLOBAL_H_2014
#define WYD_GLOBAL_H_2014

namespace MyPlatform
{
#include "PlatformHeader.h"


#ifdef _UNICODE
#define MyTrace					MyTraceW

#define DeleteDir				DeleteDirW
#define CopyDir					CopyDirW
#define MoveDir					MoveDirW
#define IsDirHaveThisExtFile	IsDirHaveThisExtFileW
#define CreateFullDirectory		CreateFullDirectoryW
#define GetAllFilesUnderDir		GetAllFilesUnderDirW
#define SplitExtArray			SplitExtArrayW
#define GetAllFilesUnderDirWithOutFilter GetAllFilesUnderDirWithOutFilterW
#define GetDirUnderDir			GetDirUnderDirW
#define DeleteDirPathEndSeparator DeleteDirPathEndSeparatorW

#define GetFullPathUnderExe		GetFullPathUnderExeW
#define GetOpenFolder			GetOpenFolderW
#define GetMyOpenFileName		GetMyOpenFileNameW
#define GetBookTypeByFilePath	GetBookTypeByFilePathW
#define ChangeFilePathExt		ChangeFilePathExtW
#define GetFilePathDir			GetFilePathDirW
#define GetFilePathFullName		GetFilePathFullNameW
#define GetFilePathExt			GetFilePathExtW

#define GetFileSize				GetFileSizeW
#define WriteFileData			WriteFileDataW
#define GetAllFileData			GetAllFileDataW

#define FindWindowWithClassNameAndWindowName	FindWindowWithClassNameAndWindowNameW
#define GetWndInDlgWndByName	GetWndInDlgWndByNameW

#define IsStrHasChinese			IsStrHasChineseW
#define Int64ToString			Int64ToStringW
#define DoubleToString			DoubleToStringW

#define CreateFileFromResource	CreateFileFromResourceW
#else
#define MyTrace					MyTraceA

#define DeleteDir				DeleteDirA
#define CopyDir					CopyDirA
#define MoveDir					MoveDirA
#define IsDirHaveThisExtFile	IsDirHaveThisExtFileA
#define CreateFullDirectory		CreateFullDirectoryA
#define GetAllFilesUnderDir		GetAllFilesUnderDirA
#define SplitExtArray			SplitExtArrayA
#define GetAllFilesUnderDirWithOutFilter GetAllFilesUnderDirWithOutFilterA
#define GetDirUnderDir			GetDirUnderDirA
#define DeleteDirPathEndSeparator DeleteDirPathEndSeparatorA

#define GetFullPathUnderExe		GetFullPathUnderExeA
#define GetOpenFolder			GetOpenFolderA
#define GetMyOpenFileName		GetMyOpenFileNameA
#define GetBookTypeByFilePath	GetBookTypeByFilePathA
#define ChangeFilePathExt		ChangeFilePathExtA
#define GetFilePathDir			GetFilePathDirA
#define GetFilePathFullName		GetFilePathFullNameA
#define GetFilePathExt			GetFilePathExtW

#define GetFileSize				GetFileSizeA
#define WriteFileData			WriteFileDataA
#define GetAllFileData			GetAllFileDataA

#define FindWindowWithClassNameAndWindowName	FindWindowWithClassNameAndWindowNameA
#define GetWndInDlgWndByName	GetWndInDlgWndByNameA

#define IsStrHasChinese			IsStrHasChineseA
#define Int64ToString			Int64ToStringA

#define CreateFileFromResource	CreateFileFromResourceA
#endif
#define MyTraceA	CInformationTrace::TraceA
#define MyTraceW	CInformationTrace::TraceW
int					GetFullPathUnderExeA(const char* szName, char* szPath, int nSize = _MAX_PATH);
int					GetFullPathUnderExeW(const wchar_t* szName, wchar_t* szPath, int nSize= _MAX_PATH);
int					CreateFullDirectoryA(const char* szPath);
int					CreateFullDirectoryW(const wchar_t* szPath);
int					DeleteDirA(const char* szDirFP);
int					DeleteDirW(const wchar_t* szDirFP);
int					GetAllFilesUnderDirA( const char* szDirFP, std::vector<std::string>* pvOutFiles, const char* szFilter = NULL, BOOL bChildOnly = FALSE);
int					GetAllFilesUnderDirW(const wchar_t* szDirFP, std::vector<std::wstring>* pvOutFiles, const wchar_t* szFilter = NULL, BOOL bChildOnly = FALSE);
int					GetAllFilesUnderDirWithOutFilterW(const wchar_t* szDirFP, std::vector<std::wstring>* pvOutFiles, BOOL bChildOnly = FALSE);
int					GetAllFilesUnderDirWithOutFilterA(const char* szDirFP, std::vector<std::string>* pvOutFiles, BOOL bChildOnly = FALSE);
int					SplitExtArrayA(const char *szFilter, std::vector<std::string> *pvExt);
int					SplitExtArrayW(const wchar_t *szFilter, std::vector<std::wstring> *pvExt);
int					GetDirUnderDirA(const char* szParentDir, std::vector<std::string>* pvstrOutDirFP, bool bChildOnly = true);
int					GetDirUnderDirW(const wchar_t* szParentDir, std::vector<std::wstring>* pvstrOutDirFP, bool bChildOnly = true);
BOOL				IsDirHaveThisExtFileA(const char* szDir, const char* szExt, BOOL bChildOnly = FALSE);
BOOL				IsDirHaveThisExtFileW(const wchar_t* szDir, const wchar_t* szExt, BOOL bChildOnly = FALSE);
int					CopyDirW(const wchar_t* szSrcDir, const wchar_t* szDstDir, BOOL bFailIfExists = FALSE);
int					CopyDirA(const char* szSrcDir, const char* szDstDir, BOOL bFailIfExists = FALSE);
int					MoveDirW(const wchar_t* szSrcDir, const wchar_t* szDstDir, BOOL bFailIfExists = FALSE);
int					MoveDirA(const char* szSrcDir, const char* szDstDir, BOOL bFailIfExists = FALSE);
int					DeleteDirPathEndSeparatorA(char* szDirFP);
int					DeleteDirPathEndSeparatorW(wchar_t* szDirFP);

int					GetMyOpenFileNameA(char* szFileName, int nFNSize = _MAX_PATH, const char* szFilter = "All Files\0*.*\0\0", const char* szTitle = NULL, HWND hParent = NULL);
int					GetMyOpenFileNameW(wchar_t* szFileName, int nFNSize = _MAX_PATH, const wchar_t* szFilter = L"All Files\0*.*\0\0", const wchar_t* szTitle = NULL, HWND hParent = NULL);
int					GetOpenFolderA( char* szFolder, const char* szTitle = NULL, HWND hWnd = NULL, const char* szInitFolder = NULL);
int					GetOpenFolderW(wchar_t* szFolder, const wchar_t* szTitle = NULL, HWND hParent = NULL, const wchar_t* szInitFolder = NULL);
std::string			ChangeFilePathExtA(const char* szSrcFP, const char* szDstExt);
std::wstring		ChangeFilePathExtW(const wchar_t* szSrcFP, const wchar_t* szDstExt);
std::string			GetFilePathDirA(const char* szFilePath, BOOL bEndSeparator = FALSE);
std::wstring		GetFilePathDirW(const wchar_t* szFilePath, BOOL bEndSeparator = FALSE);
std::string			GetFilePathFullNameA(const char* szFilePath);
std::wstring		GetFilePathFullNameW(const wchar_t* szFilePath);
std::string			GetFilePathExtA(const char* szFilePath);
std::wstring		GetFilePathExtW(const wchar_t* szFilePath);
int					GetAppPathFromRegAppPath(TCHAR *szAppPath, int nPathCount, const TCHAR *szExeName);
int					GetFileCreateTime(const TCHAR *szFilePath, SYSTEMTIME *stCreate);
int					GetFileCreateTimestampSecond(const TCHAR *szFilePath, ULONGLONG *pullCreateTime);
int					GetAllFileDataA(const char* szFilePath, void** ppData, bool bZeroEnd = false);
int					GetAllFileDataW(const wchar_t* szFilePath, void** ppData, bool bZeroEnd = false);
int					WriteFileDataA(const char* szFilePath, const void* pData, int nLen, char* szMode);
int					WriteFileDataW(const wchar_t* szFilePath, const void* pData, int nLen, wchar_t* szMode);

int					CreateFileFromResourceA(int nID, char* szType, char* szFile);
int					CreateFileFromResourceW(int nID, wchar_t* szType, wchar_t* szFile);

int					DownloadWithCookies(const char* cszURL, std::string* pStrData, const bool* pbExit, std::vector<std::string>* pvstrHttpHeader = NULL);
int					DownloadFile(const char* cszURL, const TCHAR* cszFName, const bool* pbExit, tstring* pstrNameToServer = NULL, int* pnRetCode = NULL, std::vector<std::string>* pvstrHttpHeader = NULL);
int					UploadFile(tstring* pstrReceive, const char* cszURL, const TCHAR* cszFPath, const bool* pbExit);
int					PostData(std::string* strReceive, const char* cszURL, const char* cszPostData, const bool* pbExit, std::string* pstrResponseHeader = NULL, long* plRet = NULL, std::vector<std::string>* pvstrHttpHeader = NULL);
std::string			GetHttpHeaderValueByName(const char* szHttpHeader, const char* szKey);


int					FindWindowWithClassNameAndWindowNameW(const wchar_t *szClassName, const wchar_t* szWndName, bool bPartialClassName, bool bPartialWndName, std::vector<HWND> *pvhwndFind);
int					FindWindowWithClassNameAndWindowNameA(const char *szClassName, const char* szWndName, bool bPartialClassName, bool bPartialWndName, std::vector<HWND> *pvhwndFind);
int					GetWndInDlgWndByNameA(std::vector<HWND>* pvhFind, HWND hParent, const char* szCtrlCName, const char* szCtrlTitle, BOOL bInChildDlg = TRUE);
int					GetWndInDlgWndByNameW(std::vector<HWND>* pvhFind, HWND hParent, const wchar_t* szCtrlCName, const wchar_t* szCtrlTitle, BOOL bInChildDlg = TRUE);

std::string			WcharToUtf8(const wchar_t* szData);
std::string			WcharToChar(const wchar_t* szData);
std::string			CharToUtf8(const char* szData);
std::string			Utf8ToChar(const char* szData);
std::string			TcharToChar(const TCHAR * szData);
std::string			TcharToUtf8(const TCHAR * szData);
tstring				CharToTchar(const char* szData);
tstring				Utf8ToTchar(const char* szData);
tstring				WcharToTchar(const wchar_t* szData);
std::wstring		TcharToWchar(const TCHAR* szData);
std::wstring		CharToWchar(const char* szData);
std::wstring		Utf8ToWchar(const char* szData);

std::string			Unicode2Utf8(const char* unicode);

std::string			Int64ToStringA(__int64 nData);
std::wstring		Int64ToStringW(__int64 nData);
std::wstring		DoubleToStringW(double dData, int nPercision);

BOOL				IsStrHasChineseA(const char* szStr);
BOOL				IsStrHasChineseW(const wchar_t* szStr);

int					GetImgWAndH(const TCHAR* szImgFP, int* pnWidth, int* pnHeight);
int					GetJpgImgWAndH(const TCHAR* szImgFP, int* pnWidth, int* pnHeight);
int					GetPngImgWAndH(const TCHAR* szImgFP, int* pnWidth, int* pnHeight);
int					GetFileSizeA(const char* szFileP, int* pnSize);
int					GetFileSizeW(const wchar_t* szFileP, int* pnSize);

MyPlatform::E_BookType			GetBookTypeByFilePathA(const char* szFilePath);
MyPlatform::E_BookType			GetBookTypeByFilePathW(const wchar_t* szFilePath);

int					GetSafeProcAddress(const HMODULE hDll, const char* szFName, FARPROC* pFunc);

std::string			ChangeUrlToNet(const char* szUrl);
std::string			URLEncode(const char* szURL);
std::string			URLEncode(const std::string &sIn); 
std::string			URLDecode(const char* szUrl);

double				mywtof(const wchar_t* szSrc);
bool				DestoryDirectory(const TCHAR* pDir);

BOOL				FindProcessIDFromName(const TCHAR *szProcessName, std::vector<DWORD>& rvdwID);
BOOL				FindProcessFromName(const TCHAR *szProcessName);
int					KillProcessFromName(const TCHAR *szProcessName);
void				DisableSetUnhandledExceptionFilter();
int					ExceptionWriteDump(struct _EXCEPTION_POINTERS *ExceptionInfo);

ULONGLONG			GetTimeStampMillisecond(void);

/// com
int					GetDispatchTypeName(IDispatchPtr _dispatch, BSTR *pbstrName);


bool				SetDlgItemDouble(HWND hWnd, int nIDDlgItem, double dValue, int nPrecision);
double				GetDlgItemDouble(HWND hWnd, int nIDDlgItem);

CONST tstring ResultsFolder = _T("Results");

class CThreadMutex
{
public:
	CThreadMutex()
	{
		INITIALIZE_MUTEX(&m_hTMutex);
	}
	~CThreadMutex()
	{
		DESTROY_MUTEX(&m_hTMutex);
	}
	THREAD_MUTEX* GetAddress()
	{
		return &m_hTMutex;
	}
private:
	THREAD_MUTEX m_hTMutex;
};

class CMyMutexControl
{
public:
	CMyMutexControl(THREAD_MUTEX* ptTMutex)
	{
		m_ptTMutex = ptTMutex;
		LOCK_MUTEX(m_ptTMutex);
	}
	~CMyMutexControl()
	{
		UNLOCK_MUTEX(m_ptTMutex);
	}
private:
	THREAD_MUTEX* m_ptTMutex;
};

class CAutoFree
{
public:
	CAutoFree(void** ppMemAddress)
	{
		m_ppMemAddress = ppMemAddress;
	}
	~CAutoFree()
	{
		FREE(*m_ppMemAddress);
	}
private:
	void** m_ppMemAddress;
};

};
 
#endif


