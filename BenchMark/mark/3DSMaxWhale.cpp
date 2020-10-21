#include "stdafx.h"
#include "MarkObject.h"
#include "3DSMax.h"
#include "3DSMaxWhale.h"
#include "3DSMaxWhale_Opids.h"
C3DSMaxWhaleMark::C3DSMaxWhaleMark() 
	: C3DSMaxMark()
{
	LogInfo(_T("Invoke C3DSMaxWhaleMark().."));

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("3dsmax-¾¨ÓãäÖÈ¾"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax\\jingyu.max"), m_szSourceMaterialFP);
	m_eMarkType = E_MARK_3DSMAX_WHALE;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Calculation;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("bb15a93c02194420aa5433be8a34f614")); //latest md5
	//SetMD5PassWd(std::string("90337dc0fc90738311731a6fcd95f65c"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

C3DSMaxWhaleMark::~C3DSMaxWhaleMark()
{
	LogInfo(_T("Invoke ~C3DSMaxWhaleMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int C3DSMaxWhaleMark::run()
{
	int nRet = 0;
	LogInfo(_T("run 1"));
	DeleteFile(m_szVideoFP);
	TakeMeasurement(new CCounterContent(C3DSMaxWhale_Opids::DESC_TYPE_MARKER, C3DSMaxWhale_Opids::DESC_RUN),
		[&]( )->int {
		LogInfo(_T("run 2"));
		nRet = C3DSMaxMark::run();
		return BENCH_MARK_OK;
	});
	LogInfo(_T("run 3"));
	DeleteFile(m_szVideoFP);

	return nRet;
}

int C3DSMaxWhaleMark::wirteScript()
{
	FILE* fp = NULL;

	LogInfo(_T("wirteScript 1"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax\\render3d.ms"), m_szScriptFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\3dsMax\\whalerender.avi"), m_szVideoFP);
	_tfopen_s(&fp, m_szScriptFP, _T("wb"));
	if (NULL == fp)
	{
		return -1;
	}
	LogInfo(_T("wirteScript 2"));
	TCHAR szWhaleFPTemp[_MAX_PATH * 2] = { 0 };
	TCHAR szVideoFPTemp[_MAX_PATH * 2] = { 0 };

	_tcscpy_s(szWhaleFPTemp, _ARRAYSIZE(szWhaleFPTemp), m_szSourceMaterialFP);
	_tcscpy_s(szVideoFPTemp, _ARRAYSIZE(szVideoFPTemp), m_szVideoFP);
	changePathDoubleSplit(szWhaleFPTemp, _ARRAYSIZE(szWhaleFPTemp));
	changePathDoubleSplit(szVideoFPTemp, _ARRAYSIZE(szVideoFPTemp));

	CString strContent;
	strContent.Format(_T("loadMaxFile \"%s\" useFileUnits:true\r\n\r\n")\
		_T("renderers.current = Default_Scanline_Renderer ()\r\n")\
		_T("RenderSceneDialog.close()\r\n")\
		_T("rendTimeType = 3\r\n")\
		_T("rendStart = 1f\r\n")\
		_T("rendEnd = 100f\r\n")\
		_T("rendFileNumberBase = 0\r\n")\
		_T("renderWidth = 4096\r\n")\
		_T("renderHeight = 3112\r\n")\
		_T("rendSaveFile = true\r\n")\
		_T("rendOutputFilename = \"%s\"\r\n")\
		_T("RenderSceneDialog.open()\r\n")\
		_T("RenderSceneDialog.commit()\r\n")\
		_T("renderSceneDialog.update()\r\n")\
		_T("RenderSceneDialog.close()\r\n")\
		_T("\r\n")\
		_T("max quick render\r\n")\
		_T("setSaveRequired false\r\n")\
		_T("quitMAX()\r\n")\
		_T("\r\n"), szWhaleFPTemp, szVideoFPTemp);
	std::string& rstrAnsiContent = MyPlatform::TcharToChar(strContent.GetBuffer(0));
	fwrite(rstrAnsiContent.c_str(), rstrAnsiContent.size(), sizeof(char), fp);
	//LogInfo({ _T("wirteScript 3"), MyPlatform::CharToTchar(rstrAnsiContent.c_str()) });
	fclose(fp);
	return BENCH_MARK_OK;
}
