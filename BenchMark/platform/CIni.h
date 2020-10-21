
#ifndef _CINI_H__7F007F5C_5CBC_4766_AF4F_830668EACAC7
#define _CINI_H__7F007F5C_5CBC_4766_AF4F_830668EACAC7

#include "PlatformHeader.h"

namespace MyPlatform
{
class CIni
{
#define FILE_INIA  "PARAMETER.INI"
#define FILE_INIW  L"PARAMETER.INI"

public:
	CIni();
	CIni(const TCHAR* szPath, BOOL bDir = FALSE);

	BOOL	IsIniFileExist();

	int		SetInt(const TCHAR* szApp, const TCHAR* szKey, int nData);
	int		GetInt(const TCHAR* szApp, const TCHAR* szKey, int nData = 0);
	int		SetDouble(const TCHAR* szApp, const TCHAR* szKey, double dData);
	double	GetDouble(const TCHAR* szApp, const TCHAR* szKey, double dDefault = 0);
	int     SetString(const TCHAR* szApp, const TCHAR* szKey, const TCHAR* szData);
	int     GetString(const TCHAR* szApp, const TCHAR* szKey, TCHAR* szData, int nDataLen, const TCHAR* szDefalut = NULL);
	int		SafeInt(const TCHAR* szApp, const TCHAR* szKey, int nDefault);
	double	SafeDouble(const TCHAR* szApp, const TCHAR* szKey, double dDefault);
	int		SafeString(const TCHAR* szApp, const TCHAR* szKey, const TCHAR* szDefault);

	int		SetIntW(const wchar_t* szApp, const wchar_t* szKey, int nData);
	int		GetIntW(const wchar_t* szApp, const wchar_t* szKey, int nData);
	int		SetIntA(const char* szApp, const char* szKey, int nData);
	int		GetIntA(const char* szApp, const char* szKey, int nDefault = 0);

	int		SetDoubleW(const wchar_t* szApp, const wchar_t* szKey, double dData);
	double	GetDoubleW(const wchar_t* szApp, const wchar_t* szKey, double dDefault = 0);
	int		SetDoubleA(const char* szApp, const char* szKey, double dData);
	double	GetDoubleA(const char* szApp, const char* szKey, double dDefault = 0);

	int     SetStringA(const char* szApp, const char* szKey, const char* szData);
	int     GetStringA(const char* szApp, const char* szKey, char* szData, int nDataLen, const char* szDefalut = NULL);
	int     SetStringW(const wchar_t* szApp, const wchar_t* szKey, const wchar_t* szData);
	int     GetStringW(const wchar_t* szApp, const wchar_t* szKey, wchar_t* szData, int nDataLen, const wchar_t* szDefalut = NULL);
	

	int		SafeIntW(const wchar_t* szApp, const wchar_t* szKey, int nDefault);
	double	SafeDoubleW(const wchar_t* szApp, const wchar_t* szKey, double dDefault);
	int		SafeStringW(const wchar_t* szApp, const wchar_t* szKey, const wchar_t* szDefault);
	int		SafeIntA(const char* szApp, const char* szKey, int nDefault);
	double	SafeDoubleA(const char* szApp, const char* szKey, double dDefault);
	int		SafeStringA(const char* szApp, const char* szKey, const char* szDefault);

	int		DeleteAppKeyA(const char* szApp);
	int		DeleteAppKeyW(const wchar_t* szApp);
	int		DeleteAppKey(const TCHAR* szApp);
	int		DeleteIniFile();
	
	const TCHAR* GetFilePath();
private:
	char	m_szFileA[_MAX_PATH];
	wchar_t m_szFileW[_MAX_PATH];
};
};
#endif

