
#ifndef PLATFORM_STRUCT_H_180528CA_B4A7_4F33_9990_7A87ABAB4439
#define PLATFORM_STRUCT_H_180528CA_B4A7_4F33_9990_7A87ABAB4439

#include "PlatformDefine.h"
namespace MyPlatform
{
	struct T_FontReplace
	{
		std::string strSrc;
		BOOL bDelBracket;
		std::string strDst;
	};


	struct T_MyPaperInfo
	{
		tstring pstrInfo[E_PI_END];
	};

	struct T_DetialColumnInfo
	{
		E_PaperInfo ePInfo;
		BOOL		bVisible;
	};


	struct T_CheckFileInfo
	{
		tstring strFilePath;
		bool isZipChild;
		int nZipChildCount;
		int nZipChildIndex;
		tstring strZipFP;

		int nIndexInTable;
	
		int nFileId;
	};
};

#endif



