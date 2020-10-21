#pragma once

#define DllImport __declspec(dllimport)
#define DllExport __declspec(dllexport)
// #pragma deprecated("\"Mark::MStr::operator+\"" "not support")

#if defined(MARK_STATIC)
#define MAPI
#define MCLS
#define EXPIMP_TEMPLATE
#elif defined(MARK_EXPORTS)
#define MAPI DllExport
#define MCLS DllExport
#define EXPIMP_TEMPLATE
#else
#define MAPI DllImport
#define MCLS DllImport
#define EXPIMP_TEMPLATE extern
#endif

#define MARK_CODE_NODE

namespace Mark{

#define Void void
#define Const const

	typedef char               Char;
	typedef wchar_t            WChar;
	typedef short              Short;
	typedef int                Int;
	typedef long               Long;
	typedef long long          LLong;

	typedef unsigned char      UChar;
	typedef unsigned short     UShort;
	typedef unsigned int       UInt;
	typedef unsigned long      ULong;
	typedef unsigned long long ULLong;

	typedef float              Float;
	typedef double             Double;
	typedef bool               Boolean;

	typedef UChar              Byte;
	typedef UShort             Word;
	typedef UShort             Excel;
	typedef Void              *LPointer;
	typedef LPointer           Handle;

	typedef Char              *LPString;
	typedef Const Char        *LPCString;

	typedef WChar             *LPWString;
	typedef Const WChar       *LPCWString;

#if defined(UNICODE) || defined(_UNICODE)
	typedef WChar              TChar;
	typedef LPWString          LPTString;
	typedef LPCWString         LPCTString;
#else
	typedef Char               TChar;
	typedef LPString           LPTString;
	typedef LPCString          LPCTString;
#endif

};

#include <new>
