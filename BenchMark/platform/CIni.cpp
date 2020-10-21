#include "stdafx.h"
#include "CIni.h"
namespace MyPlatform
{
CIni::CIni()
{
	memset(m_szFileA, 0, sizeof(m_szFileA));
	GetModuleFileNameA(GetModuleHandleA(NULL), m_szFileA, MAX_PATH);

	memset(m_szFileW, 0, sizeof(m_szFileW));
	GetModuleFileNameW(GetModuleHandleW(NULL), m_szFileW, MAX_PATH);

	sprintf_s(strrchr(m_szFileA, '\\') + 1, MAX_PATH - strlen(m_szFileA), "%s", FILE_INIA);
	swprintf_s(wcsrchr(m_szFileW, L'\\') + 1, MAX_PATH - wcslen(m_szFileW),  L"%s", FILE_INIW);
}

CIni::CIni( const TCHAR* szPath, BOOL bDir /*= FALSE*/ )
{
	if (bDir)
	{
		sprintf_s(m_szFileA, sizeof(m_szFileA) / sizeof(m_szFileA[0]), "%s\\%s", TcharToChar(szPath).c_str(), FILE_INIA);
		swprintf_s(m_szFileW, sizeof(m_szFileW) / sizeof(m_szFileW[0]), L"%s\\%s", TcharToWchar(szPath).c_str(), FILE_INIW);
	}
	else
	{
		strcpy_s(m_szFileA, sizeof(m_szFileA) / sizeof(m_szFileA[0]), TcharToChar(szPath).c_str());
		wcscpy_s(m_szFileW, sizeof(m_szFileW) / sizeof(m_szFileW[0]), TcharToWchar(szPath).c_str());
	}
}

int CIni::SetIntA( const char* szApp, const char* szKey, int nData )
{
	char szData[MAX_PATH] = {0};
	sprintf_s(szData, MAX_PATH, "%d", nData);
	return WritePrivateProfileStringA(szApp, szKey, szData, m_szFileA);
}

int CIni::GetIntA( const char* szApp, const char* szKey, int nDefault )
{
	return GetPrivateProfileIntA(szApp, szKey, nDefault, m_szFileA);
}

int CIni::SetDoubleA( const char* szApp, const char* szKey, double dData )
{
	char szData[MAX_PATH] = {0};
	sprintf_s(szData, MAX_PATH, "%lf", dData);
	return WritePrivateProfileStringA(szApp, szKey, szData, m_szFileA);
}

double CIni::GetDoubleA( const char* szApp, const char* szKey, double dDefault /*= 0*/ )
{
	char szDefault[MAX_PATH] = {0};
	char szData[MAX_PATH] = {0};
	sprintf_s(szDefault, MAX_PATH, "%lf", dDefault);
	GetPrivateProfileStringA(szApp, szKey, szDefault, szData, MAX_PATH, m_szFileA);
	return atof(szData);
}

int CIni::SetStringA( const char* szApp, const char* szKey, const char* szData )
{
	return WritePrivateProfileStringA(szApp, szKey, szData, m_szFileA);
}

int CIni::GetStringA( const char* szApp, const char* szKey, char* szData, int nDataLen, const char* szDefalut /*= NULL*/ )
{
	GetPrivateProfileStringA(szApp, szKey, szDefalut, szData, nDataLen, m_szFileA);
	return 1;
}

int CIni::SafeIntA( const char* szApp, const char* szKey, int nDefault )
{
	int nValue = 0;
	nValue = GetIntA(szApp, szKey, nDefault);
	SetIntA(szApp, szKey, nValue);
	return	nValue;
}

double CIni::SafeDoubleA( const char* szApp, const char* szKey, double dDefault )
{
	double dValue = 0.0;
	dValue = GetDoubleA(szApp, szKey, dDefault);
	SetDoubleA(szApp, szKey, dValue);
	return	dValue;
}

int CIni::SafeStringA( const char* szApp, const char* szKey, const char* szDefault )
{
	char	szValue[MAX_PATH] = {0};
	GetStringA(szApp, szKey, szValue, sizeof(szValue), szDefault);
	SetStringA(szApp, szKey, szValue);
	return	1;
}

int CIni::SetIntW( const wchar_t* szApp, const wchar_t* szKey, int nData )
{
	wchar_t szData[MAX_PATH] = {0};
	swprintf_s(szData, MAX_PATH, L"%d", nData);
	return WritePrivateProfileStringW(szApp, szKey, szData, m_szFileW);
}

int CIni::GetIntW( const wchar_t* szApp, const wchar_t* szKey, int nDefault )
{
	return GetPrivateProfileIntW(szApp, szKey, nDefault, m_szFileW);
}

int CIni::SetDoubleW( const wchar_t* szApp, const wchar_t* szKey, double dData )
{
	wchar_t szData[MAX_PATH] = {0};
	swprintf_s(szData, MAX_PATH, L"%lf", dData);
	return WritePrivateProfileStringW(szApp, szKey, szData, m_szFileW);
}

double CIni::GetDoubleW( const wchar_t* szApp, const wchar_t* szKey, double dDefault /*= 0*/ )
{
	wchar_t szDefault[MAX_PATH] = {0};
	wchar_t szData[MAX_PATH] = {0};

	swprintf_s(szDefault, MAX_PATH, L"%lf", dDefault);
	GetPrivateProfileStringW(szApp, szKey, szDefault, szData, MAX_PATH, m_szFileW);
	return (_wtof(szData));
}

int CIni::SetStringW( const wchar_t* szApp, const wchar_t* szKey, const wchar_t* szData )
{
	return WritePrivateProfileStringW(szApp, szKey, szData, m_szFileW);
}

int CIni::GetStringW( const wchar_t* szApp, const wchar_t* szKey, wchar_t* szData, int nDataLen, const wchar_t* szDefalut /*= NULL*/ )
{
	GetPrivateProfileStringW(szApp, szKey, szDefalut, szData, nDataLen, m_szFileW);
	return 1;
}

int CIni::SafeIntW( const wchar_t* szApp, const wchar_t* szKey, int nDefault )
{
	int nValue = 0;
	nValue = GetIntW(szApp, szKey, nDefault);
	SetIntW(szApp, szKey, nValue);
	return	nValue;
}

double CIni::SafeDoubleW( const wchar_t* szApp, const wchar_t* szKey, double dDefault )
{
	double dValue = 0.0;
	dValue = GetDoubleW(szApp, szKey, dDefault);
	SetDoubleW(szApp, szKey, dValue);
	return	dValue;
}

int CIni::SafeStringW( const wchar_t* szApp, const wchar_t* szKey, const wchar_t* szDefault )
{
	wchar_t	szValue[MAX_PATH] = {0};
	GetStringW(szApp, szKey, szValue, sizeof(szValue), szDefault);
	SetStringW(szApp, szKey, szValue);
	return	1;
}

int CIni::SetInt( const TCHAR* szApp, const TCHAR* szKey, int nData )
{
#ifdef _UNICODE
	return SetIntW(szApp, szKey, nData);
#else
	return SetIntA(szApp, szKey, nData);
#endif
}

int CIni::GetInt( const TCHAR* szApp, const TCHAR* szKey, int nData )
{
#ifdef _UNICODE
	return GetIntW(szApp, szKey, nData);
#else
	return GetIntA(szApp, szKey, nData);
#endif
}

int CIni::SetDouble( const TCHAR* szApp, const TCHAR* szKey, double dData )
{
#ifdef _UNICODE
	return SetDoubleW(szApp, szKey, dData);
#else
	return SetDoubleA(szApp, szKey, dData);
#endif
}

double CIni::GetDouble( const TCHAR* szApp, const TCHAR* szKey, double dDefault /*= 0*/ )
{
#ifdef _UNICODE
	return GetDoubleW(szApp, szKey, dDefault);
#else
	return GetDoubleA(szApp, szKey, dDefault);
#endif
}

int CIni::SetString( const TCHAR* szApp, const TCHAR* szKey, const TCHAR* szData )
{
#ifdef _UNICODE
	return SetStringW(szApp, szKey, szData);
#else
	return SetStringA(szApp, szKey, szData);
#endif
}

int CIni::GetString( const TCHAR* szApp, const TCHAR* szKey, TCHAR* szData, int nDataLen, const TCHAR* szDefalut /*= NULL*/ )
{
#ifdef _UNICODE
	return GetStringW(szApp, szKey, szData, nDataLen, szDefalut);
#else
	return GetStringA(szApp, szKey, szData, nDataLen, szDefalut);
#endif
}

int CIni::SafeInt( const TCHAR* szApp, const TCHAR* szKey, int nDefault )
{
#ifdef _UNICODE
	return SafeIntW(szApp, szKey, nDefault);
#else
	return SafeIntA(szApp, szKey, nDefault);
#endif
}

double CIni::SafeDouble( const TCHAR* szApp, const TCHAR* szKey, double dDefault )
{
#ifdef _UNICODE
	return SafeDoubleW(szApp, szKey, dDefault);
#else
	return SafeDoubleA(szApp, szKey, dDefault);
#endif
}

int CIni::SafeString( const TCHAR* szApp, const TCHAR* szKey, const TCHAR* szDefault )
{
#ifdef _UNICODE
	return SafeStringW(szApp, szKey, szDefault);
#else
	return SafeStringA(szApp, szKey, szDefault);
#endif
}

int CIni::DeleteAppKeyA( const char* szApp )
{
	return ::WritePrivateProfileStringA(szApp, NULL, NULL, m_szFileA);
}

int CIni::DeleteAppKeyW( const wchar_t* szApp )
{
	return ::WritePrivateProfileStringW(szApp, NULL, NULL, m_szFileW);
}

int CIni::DeleteAppKey( const TCHAR* szApp)
{
#ifdef _UNICODE
	return DeleteAppKeyW(szApp);
#else
	return DeleteAppKeyA(szApp);
#endif
}

BOOL CIni::IsIniFileExist()
{
	return 0 == _waccess(m_szFileW, 0);
}

int CIni::DeleteIniFile()
{
	return DeleteFileW(m_szFileW);
}

const TCHAR* CIni::GetFilePath()
{
#ifdef UNICODE
	return m_szFileW;
#else
	return m_szFileA;
#endif
}

};

