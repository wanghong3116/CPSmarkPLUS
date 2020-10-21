
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "PlatformHeader.h"
#include "BenchMarkDefine.h"
#include "BenchMarkEnum.h"
#include "BenchMarkStruct.h"

#include <iostream>
#include <thread>

#include "Photoshop/PSscriptingsupport.tlh"

extern MyPlatform::CIni g_ini;


#if 0
#pragma region Import the type libraries

#import "libid:2DF8D04C-5BFA-101B-BDE5-00AA0044DE52" \
	rename("RGB", "MSORGB") \
	rename("DocumentProperties", "MSODocumentProperties")
// [-or-]
//#import "C:\\Program Files\\Common Files\\Microsoft Shared\\OFFICE12\\MSO.DLL" \
//	rename("RGB", "MSORGB") \
//	rename("DocumentProperties", "MSODocumentProperties")
// [-or-]
//#import "MSO.DLL" \
//	rename("RGB", "MSORGB") \
//	rename("DocumentProperties", "MSODocumentProperties")

using namespace Office;

#import "libid:0002E157-0000-0000-C000-000000000046"
// [-or-]
//#import "C:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.OLB"
// [-or-]
//#import "VBE6EXT.OLB"

using namespace VBIDE;

#import "libid:00020905-0000-0000-C000-000000000046" \
	rename("ExitWindows", "WordExitWindows") \
	rename("FindText", "WordFindText")


using namespace VBIDE;
#import "libid:00020813-0000-0000-C000-000000000046" \
	rename("DialogBox", "ExcelDialogBox") \
	rename("RGB", "ExcelRGB") \
	rename("CopyFile", "ExcelCopyFile") \
	rename("ReplaceText", "ExcelReplaceText") \
	no_auto_exclude

// [-or-]
//#import "C:\\Program Files\\Microsoft Office\\Office12\\MSWORD.OLB" \
//	rename("ExitWindows", "WordExitWindows") \
//	rename("FindText", "WordFindText")
//#import "MSWORD.OLB" \
//	rename("ExitWindows", "WordExitWindows") \
//	rename("FindText", "WordFindText")

#import "libid:91493440-5A91-11CF-8700-00AA0060263B" \
	rename("RGB", "VisioRGB") 
// [-or-] 
//#import "C:\\Program Files\\Microsoft Office\\Office12\\MSPPT.OLB" \ 
//	rename("RGB", "VisioRGB") 
// [-or-] 
//#import "MSPPT.OLB" \ 
//	rename("RGB", "VisioRGB") 

#import "libid:00062FFF-0000-0000-C000-000000000046" \
    rename("CopyFile", "OutlookCopyFile") \
    rename("PlaySound", "OutlookPlaySound") \
	rename ("GetOrganizer", "OutlookGetOrganizer")
// [-or-]
//#import "C:\\Program Files\\Microsoft Office\\Office12\\MSOUTL.OLB"    \
//    rename("CopyFile", "OutlookCopyFile") \
//    rename("PlaySound", "OutlookPlaySound")

#pragma endregion

#import "D:\Program Files\Adobe\Acrobat 10.0\Acrobat\acrobat.tlb"
#import "C:\Program Files\Common Files\Autodesk Shared\acax22enu.tlb"
#else
#include "Office/MSO.tlh"
#include "Office/VBE6EXT.tlh"
#include "Office/MSWORD.tlh"
#include "Office/EXCEL.tlh"
#include "Office/MSPPT.tlh"
#include "Office/MSOUTL.tlh"
#include "acrobat/acrobat.tlh"
#include "AutoCAD/acax22enu.tlh"

#endif

#include "../DuiLib/UIlib.h"

#include "Mark.h"
#include "MarkFrame.h"
#include "MarkAuplus.h"
#include "MarkAuplusUDF.h"

