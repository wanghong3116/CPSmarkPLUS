#include "stdafx.h"
#include "MarkObject.h"
#include "AutoCADMark.h"
#include "AutoCAD_Opids.h"
CAutoCADMark::CAutoCADMark()
{
	LogInfo(_T("Invoke CAutoCADMark().."));

	_tcscpy_s(m_szDescript, _T("AutoCAD-布线图"));
	m_eMarkType = E_MARK_CAD;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("04f1e456d1c065dae603d80310a430d0")); //latest md5
	//SetMD5PassWd(std::string("915e36b445ea2ea410d08a40c3fddf24"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AutoCADResource.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

CAutoCADMark::~CAutoCADMark()
{
	LogInfo(_T("Invoke ~CAutoCADMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AutoCADResource\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CAutoCADMark::init()
{
	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	//MyPlatform::KillProcessFromName(_T("acad.exe"));
	//MyPlatform::KillProcessFromName(_T("LMU.exe"));
	SleepAndRecord(2 * 500);

	CoInitialize(NULL);
	HRESULT hr = S_OK;

	MyPlatform::GetFullPathUnderExe(_T("Resources\\AutoCADResource\\1.dwg"), m_szResourceFP);
	if (0 != _taccess(m_szResourceFP, 0))
	{
		LogError({ _T("资源文件丢失") });
		return -3;
	}

	m_hThreadClose = CreateThread(NULL, 0, ThreadProcCloseWindow, this, 0, 0);
	
	TakeMeasurement(new CCounterContent(CAutoCAD_Opids::DESC_TYPE_MARKER, CAutoCAD_Opids::DESC_OPEN_APPLICATION),
		[&]()->int {
		 if (S_OK != (hr = m_app.CreateInstance(_T("AutoCAD.Application.22"))))
 		{
			 LogError({ _T("未能启动AutoCAD，可能未安装") });
			return -1;
		}
	return BENCH_MARK_OK; 
	});
	SleepAndRecord(dwTimerNode1);

	{
		for (int nIdx = 0; nIdx < 3; nIdx++)
		{
			try
			{
				m_app->Visible = VARIANT_TRUE;
				m_app->WindowLeft = 0;
				m_app->WindowTop = 0;
				m_app->Width = 1920;
				m_app->Height = 1080;
				SleepAndRecord(2 * dwTimerNode1);
			}
			catch (_com_error& err)
			{
				LogError({ _T("显示CAD进程失败,无法显示AutoCAD"), err.ErrorMessage() });
				SleepAndRecord(2 * dwTimerNode1);
				continue;
			}
		}
		try
		{
			if (VARIANT_TRUE != m_app->Visible)
			{
				LogError({ _T("显示CAD进程失败,无法显示AutoCAD") });
				return -3;
			}
		}
		catch (_com_error& err)
		{
			LogError({ _T("显示CAD进程失败"), err.ErrorMessage() });
			return -2;
		}
	}
	

	return BENCH_MARK_OK;
}

int CAutoCADMark::RunCADMark()
{
	AutoCAD::AcColor pcolorAll[] = {
		AutoCAD::acRed,
		AutoCAD::acYellow,
		AutoCAD::acCyan,
		AutoCAD::acMagenta,
		AutoCAD::acWhite,
		AutoCAD::acByLayer,
	};

	try
	{
		AutoCAD::IAcadDocumentPtr pDoc = NULL;
		AutoCAD::IAcadLayersPtr pLayerArray = NULL;
		int nTry = 0;
		bool bUpdate = false;
		int nCount = 0;
		int nColorCount _ARRAYSIZE(pcolorAll);
		
		TakeMeasurement(new CCounterContent(CAutoCAD_Opids::DESC_TYPE_MARKER, CAutoCAD_Opids::DESC_OPEN_DOCUMENT),
			[&]()->int {
		
		for (; nTry < 3; nTry++)
		{
			try
			{
				pDoc = m_app->Documents->Open(m_szResourceFP);
				pLayerArray = pDoc->Layers;
				nCount = pLayerArray->Count;
				break;
			}
			catch (_com_error& err)
			{
				SleepAndRecord(dwTimerNode2);
				LogError({ _T("显示CAD进程失败"),  err.ErrorMessage() });
			}
		}
		if (nTry >= 3)
		{
			LogError({ _T("无法打开测试文件") });
			return -2;
		}
		return BENCH_MARK_OK; 
		});

		LogInfo(_T("AutoCAD::房间走线图绘制"));
		TakeMeasurement( new CCounterContent(CAutoCAD_Opids::DESC_TYPE_MARKER, CAutoCAD_Opids::DESC_DRAWING),
			[&]()->int {
		for (int nIdx = nCount; nIdx >= 10; nIdx--)
		{
			//MyPlatform::KillProcessFromName(_T("AcBrowserHost.exe"));
			SleepAndRecord(dwTimerNode3);
			try
			{
				bUpdate = false;
				AutoCAD::IAcadLayerPtr pLayer = pLayerArray->Item((long)nIdx);
				SleepAndRecord(2 * dwTimerNode);
				if (VARIANT_TRUE == pLayer->LayerOn)
				{
					continue;
				}

				pLayer->LayerOn = VARIANT_TRUE;
				SleepAndRecord(2 * dwTimerNode );
				m_app->Update();
				SleepAndRecord(2 * dwTimerNode * 2);
				bUpdate = true;
				AutoCAD::AcColor colorSrc = pLayer->color;
				nTry = 0;
				//continue;
				//pLayer->Freeze = VARIANT_FALSE;

				for (int nColorIdx = 0; nColorIdx < nColorCount; nColorIdx++)
				{
					try
					{
						pLayer->color = pcolorAll[nColorIdx];
						m_app->Update();
						SleepAndRecord(4 * dwTimerNode3);
					}
					catch (_com_error& err)
					{
						LogError({ _T("Update Layer Color"), (const TCHAR*)err.ErrorMessage() });
					}

				}
				pLayer->color = colorSrc;
			}
			catch (_com_error& err)
			{
				LogError({ _T("显示CAD进程失败"), (const TCHAR*)err.ErrorMessage() });
				if (bUpdate)
				{
					continue;
				}

				_bstr_t bstr = err.ErrorMessage();
				int a = 0;
				SleepAndRecord(2 * dwTimerNode3);
				nTry++;
				if (nTry <= 3)
				{
					nIdx++;
				}
				else
				{
					nTry = 0;
				}
			}

		}
		return BENCH_MARK_OK;
		});

		TakeMeasurement( new CCounterContent(CAutoCAD_Opids::DESC_TYPE_MARKER, CAutoCAD_Opids::DESC_SCALING),
			[&]()->int {
			for (double dScale = 8; dScale < 30; dScale += 1)
		{
			try
			{
				m_app->ZoomScaled(dScale, AutoCAD::AcZoomScaleType::acZoomScaledAbsolute);
				m_app->Update();
				SleepAndRecord(dwTimerNode3);
			}
			catch (_com_error& err)
			{
				LogError({ _T("显示CAD进程失败"),  err.ErrorMessage() });
				SleepAndRecord(dwTimerNode2);
			}

		}

		for (double dScale = 8; dScale > 1; dScale -= 0.5)
		{
			try
			{
				m_app->ZoomScaled(dScale, AutoCAD::AcZoomScaleType::acZoomScaledAbsolute);
				m_app->Update();
				SleepAndRecord(2 * dwTimerNode3);
			}
			catch (_com_error& err)
			{
				LogError({ _T("显示CAD进程失败"),  err.ErrorMessage() });
				SleepAndRecord(2 * dwTimerNode2);
			}
		}
		return BENCH_MARK_OK; 
		});

		pDoc->Close(VARIANT_FALSE);
		m_app->Quit();
	}
	catch (_com_error& err)
	{

		LogError({ _T("测试中出现异常"),  err.ErrorMessage() });
		MessageBox(NULL, (std::tstring(_T("测试中出现异常")) + (std::tstring)(CString(err.ErrorMessage()))).c_str(), _T(""), MB_OK);

		return -1;
	}

	//MyPlatform::KillProcessFromName(_T("acad.exe"));
	return BENCH_MARK_OK;
}


int CAutoCADMark::run()
{
	int nResult =  RunCADMark();
	 
	return nResult;
}

int CAutoCADMark::destroy()
{
	LogInfo(_T("Invoke CAutoCADMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\AutoCADResource\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	//MyPlatform::KillProcessFromName(_T("acad.exe"));
	CloseHandle(m_hThreadClose);
	m_hThreadClose = NULL;
	CoUninitialize();
	SleepAndRecord(20 * dwTimerNode3);
	return BENCH_MARK_OK;
}

DWORD WINAPI CAutoCADMark::ThreadProcCloseWindow(_In_ LPVOID lpParameter)
{
	CAutoCADMark* pThis = (CAutoCADMark*)lpParameter;
	for (DWORD dwTime = 0; dwTime < 1000 * 60; dwTime += 10)
	{
		{
			HWND hWnd = NULL;

			if (NULL != (hWnd = FindWindow(_T("#32770"), _T("Autodesk 客户参与计划"))))
			{
				pThis->SleepAndRecord(1000);
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}

		}

		{
			std::vector<HWND> vhWnd;						
			MyPlatform::FindWindowWithClassNameAndWindowName(_T("HwndWrapper[DefaultDomain;;"), _T("AutoCAD - 禁用硬件加速"), true, false, &vhWnd);
			if (vhWnd.size() > 0)
			{
				pThis->SleepAndRecord(1000);
				SendMessage(vhWnd[0], WM_CLOSE, 0, 0);
			}
// 
// 			if (NULL != (hWnd = FindWindow(_T("HwndWrapper[DefaultDomain;;2813d6a3-cf51-40fa-8f3c-cec931d0a653]"), _T("AutoCAD - 禁用硬件加速"))))
// 			{
// 				SleepAndRecord(2 * 500);
// 				SendMessage(hWnd, WM_CLOSE, 0, 0);
// 			}
		}

		Sleep(10);
	}


	return TRUE;
}

