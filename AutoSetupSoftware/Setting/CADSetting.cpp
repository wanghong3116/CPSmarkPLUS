#include "stdafx.h"
#include "CADSetting.h"

CADSetting::CADSetting()
	: AutoDeskSetting(_T("AfxMDIFrame140u"), _T("Autodesk AutoCAD 2018"), _T(" /product ACAD /language \"zh-CN\""))
{

}

CADSetting::~CADSetting()
{

}

int CADSetting::Init()
{
	TCHAR szDMaxFP[_MAX_PATH] = { 0 };
	DWORD dwType = 0, dwSize = 0;

	dwSize = sizeof(szDMaxFP);
	RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Classes\\autocad\\DefaultIcon"), _T(""), RRF_RT_REG_SZ | 0x00010000, &dwType, szDMaxFP, &dwSize);
	*(szDMaxFP + _tcslen(szDMaxFP) - 1) = '\0';
	if (0 != _taccess(szDMaxFP + 1, 0))
	{
		return -1;
	}
	_tcscpy_s(m_szExeFP, _ARRAYSIZE(m_szExeFP), szDMaxFP + 1);
	
	return 0;

}

int CADSetting::Set()
{
	const int c_nMaxTime = 1000 * 60;
	int nRunTime = 0;

	if (0 != Init())
	{
		printf("无法找到软件目录\r\n");
		return -1;
	}

	StartApp();
	const int c_nMaxWaitMainWndTime = (int)(1000 * 60);
	int nWaitTime = 0;
	while (nRunTime < c_nMaxTime && nWaitTime < c_nMaxWaitMainWndTime)
	{
		if (IsFindMainWindowAndVisible())
		{
			nWaitTime += 1000;
		}
		else
		{
			nWaitTime = 0;
		}

		ClickSerialNumber();
		ClickPermit();
		ClickRun();
		ClickDataCollect();
		Sleep(1000);
		nRunTime += 1000;
	}
	Sleep(1000 * 2);
	Destroy();

	Sleep(1000 * 2);
	 

	return 0;
}

int CADSetting::Destroy()
{
	Sleep(1000);
	MyPlatform::KillProcessFromName(_T("acad.exe"));
	MyPlatform::KillProcessFromName(_T("LMU.exe"));
	Sleep(1000);
	return 0;
}
