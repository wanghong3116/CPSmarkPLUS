#include "stdafx.h"
#include "MarkObject.h"
#include "3DSMax.h"
#include "3DSMaxCar.h"

C3DSMaxCarMark::C3DSMaxCarMark()
{
	_tcscpy_s(m_szDescript, _T("3dsmax:Æû³µäÖÈ¾"));
	MyPlatform::GetFullPathUnderExe(_T("3dsMax\\car.max"), m_szSourceMaterialFP);
	//m_eMarkType = E_MARK_3DSMAX_CAR;
	m_eMarkClassify = E_MARK_CLASSIFY_MEDIA;
}

C3DSMaxCarMark::~C3DSMaxCarMark()
{

}

int C3DSMaxCarMark::wirteScript()
{
	FILE* fp = NULL;

	MyPlatform::GetFullPathUnderExe(_T("3dsMax\\renderCar.ms"), m_szScriptFP);
	_tfopen_s(&fp, m_szScriptFP, _T("wb"));
	if (NULL == fp)
	{
		return -1;
	}

	TCHAR szWhaleFPTemp[_MAX_PATH * 2] = { 0 };

	_tcscpy_s(szWhaleFPTemp, _ARRAYSIZE(szWhaleFPTemp), m_szSourceMaterialFP);
	changePathDoubleSplit(szWhaleFPTemp, _ARRAYSIZE(szWhaleFPTemp));

	CString strContent;
	strContent.Format(_T("loadMaxFile \"%s\" useFileUnits:true\r\n\r\n")\
		_T("\r\n")\
		_T("max quick render\r\n")\
		_T("setSaveRequired false\r\n")\
		_T("quitMAX()\r\n")\
		_T("\r\n"), szWhaleFPTemp);

	std::string& rstrAnsiContent = MyPlatform::TcharToChar(strContent.GetBuffer(0));
	fwrite(rstrAnsiContent.c_str(), rstrAnsiContent.size(), sizeof(char), fp);

	fclose(fp);
	return BENCH_MARK_OK;
}
