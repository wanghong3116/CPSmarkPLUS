
#ifndef _PLATFORM_HEADER_H_180528CA_B4A7_4F33_9990_7A87ABAB4439
#define _PLATFORM_HEADER_H_180528CA_B4A7_4F33_9990_7A87ABAB4439

///标准库
#define ISNOTVC6 (_MSC_VER != 1200)

#if _MSC_VER != 1200
#include <shlobj.h>
#include "comdef.h"

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <tchar.h>
#include <string>

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#else

#define _wtof(p)					MyPlatform::mywtof(p)
#define _access						access
#define strcpy_s(a,b,c)				strcpy(a,c)
#define wcscpy_s(a,b,c)				wcscpy(a,c)
#define _tfopen_s(a,b,c)			(*(a) = _tfopen(b,c))
#define strncpy_s(a,b,c,d)			strncpy(a,c,d)
#define localtime_s(a,b)			(memcpy(a, localtime(b), sizeof(struct tm)))
#define fopen_s(a,b,c)				(*(a) = fopen(b,c))
#define _wfopen_s(a,b,c)			(*(a) = _wfopen(b,c))
#define freopen_s(a,b,c,d)			(*(a) = freopen(b,c,d))
#define wcsncpy_s(a,b,c,d)			wcsncpy(a,c,d)
#define stricmp						_stricmp
#define _wcsicmp					wcsicmp
#define swprintf_s					_snwprintf
#define vswprintf_s					_vsnwprintf
#define vsprintf_s					_vsnprintf	
#define sprintf_s					_snprintf
#endif

namespace MyPlatform
{
#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
	typedef std::wstringstream tstringstream;
#else
	typedef std::string tstring;
	typedef std::stringstream tstringstream;
#endif
};




/// 第三方
extern "C"
{
//#include "curl.h"
}

//#include "ximage.h"

/// 自己
#include "InformationTrace.h"
#include "PlatformEnum.h"
#include "PlatformStruct.h"
#include "PlatformDefine.h"
//#include "DefineConfig.h"
#include "CIni.h"
#include "ThreadPacket.h"
#include "PlatformGlobalFunc.h"
//#include "GlobalValue.h"
#include "MyAlgorithm.h"
#include "PlatromPaint.h"

#endif