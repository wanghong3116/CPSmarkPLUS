#include "stdafx.h"
#include "MarkObject.h"
#include "OpenCL.h"

COpenCLMark::COpenCLMark()
{
	_tcscpy_s(m_szDescript, _T("OPENCL 目标识别"));
	m_eMarkType = E_MARK_CAD;
	m_eMarkClassify = E_MARK_CLASSIFY_OFFICE;
	
}

COpenCLMark::~COpenCLMark()
{

}

int COpenCLMark::init()
{
	MyPlatform::GetFullPathUnderExe(_T("OpenCL\\tools.bat"), m_szBatFP);

	if (0 != _taccess(m_szBatFP, 0))
	{
		return -1;
	}

	return BENCH_MARK_OK;
}

int COpenCLMark::run()
{

	ULONGLONG ullCurTime = MyPlatform::GetTimeStampMillisecond();

	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("Opencl:汽车识别。。。"));
	int nRet = runProcessAndWait(m_szBatFP, MyPlatform::GetFilePathDir(m_szBatFP).c_str());
	m_nRunTime = MyPlatform::GetTimeStampMillisecond() - ullCurTime;
	return nRet;
}

int COpenCLMark::destroy()
{

	return BENCH_MARK_OK;
}
