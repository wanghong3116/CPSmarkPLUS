#include "stdafx.h"
#include "MarkObject.h"

#include "Photoshop.h"
#include "AdobeRegisterWindow.h"
#include "PhotoShop_Opids.h"

CPhotoshopMark::CPhotoshopMark()
{
	LogInfo(_T("Invoke CPhotoshopMark().."));

	dwTimerNode3 = 600;
	_tcscpy_s(m_szDescript, _T("Photoshop-海报制作"));
	m_eMarkType = E_MARK_PHOTOSHOP;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("423a7bda6b578ac770ff721c6b036323")); //latest md5
	//SetMD5PassWd(std::string("c0f484c77e58d1311c1c25b172013a88"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

CPhotoshopMark::~CPhotoshopMark()
{
	LogInfo(_T("Invoke ~CPhotoshopMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CPhotoshopMark::init()
{
	CoInitialize(NULL);
	//MyPlatform::KillProcessFromName(_T("Photoshop.exe"));
	SleepAndRecord(2 * dwTimerNode3);
		bool bStartSuccess = false;

	std::thread threadCloseStartWnd([=](bool& bSuccess)->void {

		HWND hWnd = NULL;
		while (true)
		{
			Sleep(10);
			if (bSuccess)
			{
				break;
			}

			hWnd = FindWindow(_T("PSDialogBox"), _T("Adobe Photoshop"));
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0xA, BN_CLICKED), NULL);
		}
	}, ref(bStartSuccess));

	TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_OPEN_APPLICATION),
		[&]()->int {
		AdobeRegisterWindow wndRegister(this);
		wndRegister.RegisterWindowHandler([&]()->int
		{
			if (S_OK != m_pApp.CreateInstance(_T("Photoshop.Application")))
			{
				LogError({ _T("Photoshop没有安装") });
				bStartSuccess = true;
				threadCloseStartWnd.join();
				return -1;
			}
			return BENCH_MARK_OK;
		});
		return BENCH_MARK_OK;
	});
	bStartSuccess = true;
	threadCloseStartWnd.join();
	try
	{
		int nCount = m_pApp->Documents->Count;

		for (int nDocIndex = 0; nDocIndex < nCount; nDocIndex++)
		{
			m_pApp->Documents->Item[1]->Close((long)Photoshop::PsSaveOptions::psDoNotSaveChanges);
		}

	}
	catch (_com_error& err)
	{
		err.ErrorMessage();
		LogError({ _T("关闭已经打开的工程出现异常") });
		return -2;
	}
	SleepAndRecord(2 * 6 * 1000);
	return BENCH_MARK_OK;
}

int CPhotoshopMark::run()
{
	int nRet = BENCH_MARK_OK;

	 	if (BENCH_MARK_OK != makeCrayfishPoster())
		{
			return -1;
		}
		if (BENCH_MARK_OK != makePeoplePoster())
		{
			return -2;
		}
 
	//MyPlatform::KillProcessFromName(_T("Photoshop.exe"));
	return nRet;
}

int CPhotoshopMark::destroy()
{
	LogInfo(_T("Invoke CPhotoshopMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	//MyPlatform::KillProcessFromName(_T("Photoshop.exe"));
	CoUninitialize();
	SleepAndRecord(2 * 6 * 1000);
	return BENCH_MARK_OK;
}

int CPhotoshopMark::makeCrayfishPoster()
{
	Photoshop::DocumentPtr pDoc = NULL;
	try
	{
		try
		{
			TCHAR szFirstFP[_MAX_PATH] = { 0 };
			TCHAR pszSourceMeterial[4][_MAX_PATH] = { 0 };
			TCHAR pszSourceMeterialShortName[4][_MAX_PATH] = { _T("Resources\\Photoshop\\小龙虾\\小龙虾er.psd"), _T("Resources\\Photoshop\\小龙虾\\小龙虾san.psd"), _T("Resources\\Photoshop\\小龙虾\\小龙虾si.psd"),_T("Resources\\Photoshop\\小龙虾\\小龙虾wu.psd") };
			MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\小龙虾\\小龙虾yi.psd"), szFirstFP);
			TakeMeasurement(new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_OPEN_DOCUMENT + _T("Resources\\Photoshop\\小龙虾\\小龙虾yi.psd")),
				[&]()->int {

			if (NULL == (pDoc = m_pApp->Open(szFirstFP)))
			{
				throw - 1;
			}
			return BENCH_MARK_OK; });
			MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\小龙虾\\小龙虾er.psd"), pszSourceMeterial[0]);
			MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\小龙虾\\小龙虾san.psd"), pszSourceMeterial[1]);
			MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\小龙虾\\小龙虾si.psd"), pszSourceMeterial[2]);
			MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\小龙虾\\小龙虾wu.psd"), pszSourceMeterial[3]);
			int nCount = _ARRAYSIZE(pszSourceMeterial);
			for (int nIdx = 0; nIdx < nCount; nIdx++)
			{
				LogInfo({ _T("正在打开文件："), PathFindFileName(pszSourceMeterial[nIdx]) });
				Photoshop::DocumentPtr pDocTemp;
				TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_OPEN_DOCUMENT + pszSourceMeterialShortName[nIdx]),
					[&]()->int {

				pDocTemp = m_pApp->Open(pszSourceMeterial[nIdx]);
				SleepAndRecord(dwTimerNode3);
				return BENCH_MARK_OK; });

				Photoshop::LayersPtr pLayers = pDocTemp->Layers;
				Photoshop::LayerSetPtr pLayerSet = NULL;
				int nLayerCount = pLayers->Count;
				for (int nLayerIdx = 1; nLayerIdx <= nLayerCount; nLayerIdx++)
				{

					IDispatchPtr pDispatch = pLayers->GetItem((long)nLayerIdx);

					if (NULL != (pLayerSet = pDispatch))
					{
						m_pApp->ActiveDocument = pDoc;
						SleepAndRecord(dwTimerNode3);
						Photoshop::LayerSetPtr pSrcLayerSet = pDoc->LayerSets->GetItem((long)1);
						m_pApp->ActiveDocument = pDocTemp;

						LogInfo(_T("Photoshop::复制内容"));
						TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_DUPLICATE + pszSourceMeterialShortName[nIdx]),
							[&]()->int {
							SleepAndRecord(dwTimerNode3);
						pLayerSet->Duplicate(pSrcLayerSet.GetInterfacePtr(), (long)Photoshop::psPlaceAfter);
						SleepAndRecord(dwTimerNode3);
						return BENCH_MARK_OK; });
					}

				}

				LogInfo({ _T("正在关闭文件："), PathFindFileName(pszSourceMeterial[nIdx]) });
				SleepAndRecord(dwTimerNode3);
				pDocTemp->Close((long)Photoshop::PsSaveOptions::psDoNotSaveChanges);
				SleepAndRecord(dwTimerNode3);
				pDocTemp->Release();
				pDocTemp = NULL;
			}

			LogInfo(_T("Photoshop::移动布局"));
			Photoshop::LayerSetPtr layersetFirst;
			Photoshop::LayerSetsPtr pLayerSets;
			Photoshop::LayerSetPtr layerset;
			TakeMeasurement( new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_MOVE_LAYER + _T("A")),
				[&]()->int {
			pLayerSets = pDoc->LayerSets;
			pLayerSets->GetItem((long)3)->Move(pLayerSets->GetItem((long)1), Photoshop::PsElementPlacement::psPlaceBefore);
			SleepAndRecord(dwTimerNode3);
			pLayerSets->GetItem((long)5)->Move(pLayerSets->GetItem((long)4), Photoshop::PsElementPlacement::psPlaceBefore);
			SleepAndRecord(dwTimerNode3);
			pLayerSets->GetItem((long)3)->Move(pLayerSets->GetItem((long)5), Photoshop::PsElementPlacement::psPlaceAfter);
			SleepAndRecord(dwTimerNode3);
			return BENCH_MARK_OK; });
			{
				layersetFirst = pLayerSets->GetItem((long)1);
				layersetFirst->Visible = VARIANT_TRUE;
				SleepAndRecord(dwTimerNode3);

				LogInfo(_T("Photoshop::移动位置"));
				TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_MOVE_POSITION ),
					[&]()->int {
					for (int nLoop = 0; nLoop < 10; nLoop++)
				{
					layersetFirst->Translate((double)0.05, (double)0.05);
					layersetFirst->Visible = VARIANT_TRUE;
					SleepAndRecord(dwTimerNode3);
					m_pApp->ActiveDocument = pDoc;
				}
				return BENCH_MARK_OK; });
				layersetFirst->Release();
				layersetFirst = NULL;
			}

			{
				layerset = pLayerSets->GetItem((long)3);
				layerset->BlendMode = Photoshop::psNormalBlend;
				LogInfo(_T("Photoshop::显示"));
				SleepAndRecord(dwTimerNode3);
				layerset->Release();
				layerset = NULL;
			}

			{
				Photoshop::LayerSetPtr layersetZong = pLayerSets->GetItem((long)5);
				layersetZong->LayerSets->GetItem((long)1)->Visible = VARIANT_TRUE;
				SleepAndRecord(dwTimerNode3);

				Photoshop::LayerSetPtr layersetDi = layersetZong->LayerSets->GetItem((long)3);
				Photoshop::ArtLayerPtr altLayer = layersetDi->Layers->GetItem((long)1);
				altLayer->Visible = VARIANT_TRUE;
				SleepAndRecord(dwTimerNode3);
				layersetDi->LayerSets->GetItem((long)1)->Visible = VARIANT_TRUE;
				altLayer->Release();
				altLayer = NULL;

				altLayer = layersetDi->Layers->GetItem((long)7);

				LogInfo(_T("Photoshop::显示小龙虾"));
				for (double dOpacity = 0; dOpacity <= 100; dOpacity += 5)
				{
					altLayer->FillOpacity = dOpacity;
					SleepAndRecord(dwTimerNode3);
				}

			}

			LogInfo(_T("Photoshop::移动布局"));
			try
			{
				HWND hWnd = FindWindow(_T("PSDialogBox"), _T("Adobe Photoshop"));
				TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_MOVE_LAYER + _T("B")),
					[&]()->int {
					//Photoshop::LayerSetsPtr pLayerSets = pDoc->LayerSets;
				{
					//MessageBox(hWnd, _T("Before Add LayerSet"),_T("Info"), MB_OK);
					//LogInfo(_T("1"));
					SleepAndRecord(dwTimerNode3);
					Photoshop::LayerSetPtr pNewLayerSet = pLayerSets->Add();
					//MessageBox(hWnd, _T("After Add LayerSet"), _T("Info"), MB_OK);
					//LogInfo(_T("2"));
					SleepAndRecord(dwTimerNode3);
					Photoshop::LayerSetPtr pNewFirst = pNewLayerSet->LayerSets->Add();
					//LogInfo(_T("3"));
					SleepAndRecord(dwTimerNode3);
					for (int nLayerSetIdx = 1; nLayerSetIdx <= 5; nLayerSetIdx++)
					{
						pLayerSets->GetItem((long)2)->MoveBefore(pNewFirst);
					//	LogInfo(_T("4"));
						SleepAndRecord(dwTimerNode3);
					}
					//LogInfo(_T("5"));
					pNewFirst->Delete();
					pNewFirst->Release();
					pNewFirst = NULL;
					//LogInfo(_T("6"));
				}
				return BENCH_MARK_OK; });
			}
			catch (_com_error& err)
			{
				LogError({ _T("CPhotoshopMark::makeCrayfishPoster Move Layer error"),  err.ErrorMessage() });
			}
			catch (exception ex)
			{
				LogError( {_T("CPhotoshopMark::makeCrayfishPoster Move Layer general error"),  MyPlatform::CharToTchar(ex.what()).c_str() });
			}
			double dOneWidth = pDoc->Width;
			double dOneHeight = pDoc->Height;
			LogInfo(_T("Photoshop::创建大图文件"));
			TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_CREATE_A_LARGE_PICTURE),
				[&]()->int {
					{
				Photoshop::DocumentPtr pDocNew = m_pApp->Documents->Add((double)pDoc->Width, (double)pDoc->Height * 3, (double)150, _bstr_t(_T("最终版小龙虾")));
				SleepAndRecord(dwTimerNode3);
				Photoshop::LayerSetsPtr pNewLayersets = pDocNew->LayerSets;

				Photoshop::LayerSetPtr pTempLayerSet = pNewLayersets->Add();
				SleepAndRecord(dwTimerNode3);

				Photoshop::LayerSetPtr pSrcLayerSet = pDoc->LayerSets->GetItem((long)1);
				m_pApp->ActiveDocument = pDoc;
				pSrcLayerSet->Duplicate(pTempLayerSet.GetInterfacePtr(), (long)Photoshop::psPlaceAfter);
				SleepAndRecord(dwTimerNode3);

				pLayerSets->Release();
				pLayerSets = NULL;
				pDoc->Close((long)Photoshop::PsSaveOptions::psDoNotSaveChanges);
				pDoc->Release();
				pDoc = NULL;
				m_pApp->ActiveDocument = pDocNew;

				pTempLayerSet->Delete();
				pDoc = pDocNew;
				pLayerSets = pNewLayersets;
			}
			return BENCH_MARK_OK; });

			LogInfo(_T("Photoshop::排列文件"));
			TakeMeasurement( new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_ORDER_FILES),
				[&]()->int {
			{
				Photoshop::LayerSetPtr layerSetFirst = pLayerSets->GetItem(1);

				for (int nDuplicateIdx = 1; nDuplicateIdx < 3; nDuplicateIdx++)
				{
					layerSetFirst->Duplicate(layerSetFirst.GetInterfacePtr(), (long)Photoshop::psPlaceAfter);
					Photoshop::LayerSetPtr layerSetSecond = pLayerSets->GetItem(2);

					layerSetSecond->Translate((double)0, (double)dOneHeight * nDuplicateIdx);
					SleepAndRecord(dwTimerNode3);
				}
			}
			return BENCH_MARK_OK; 
		});
			
			TakeMeasurement( new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_MERGE_INTO_A_LARGE_PICTURE),
				[&]()->int {
			LogInfo(_T("Photoshop::合并为一张大图"));
			{

				pDoc->LayerSets->GetItem((long)1)->Merge();
				SleepAndRecord(dwTimerNode3);
				pDoc->LayerSets->GetItem((long)1)->Merge();
				SleepAndRecord(dwTimerNode3);
				pDoc->LayerSets->GetItem((long)1)->Merge();
				SleepAndRecord(dwTimerNode3);
				pDoc->Flatten();
				SleepAndRecord(dwTimerNode3);
			}
			return BENCH_MARK_OK; });

			 
			LogInfo(_T("Photoshop::关闭大图"));
			/// 关闭
			pDoc->Close((long)Photoshop::PsSaveOptions::psDoNotSaveChanges);
			SleepAndRecord(dwTimerNode3);
			pDoc->Release();
			pDoc = NULL;
			m_pApp->Purge(Photoshop::PsPurgeTarget::psAllCaches);
			throw BENCH_MARK_OK;
		}
		catch (_com_error& err)
		{
			LogError({ _T("CPhotoshopMark::makeCrayfishPoster throw error="),  err.ErrorMessage() });
			throw - 2;
		}
	}
	catch (int nError)
	{
		return nError;
	}

	return BENCH_MARK_OK;
}

int CPhotoshopMark::makePeoplePoster()
{
	TCHAR szSourceFP[_MAX_PATH] = { 0 };
	Photoshop::DocumentPtr pDocBg = NULL;
	Photoshop::ArtLayerPtr pArtLayer = NULL;
	try
	{
		LogInfo(_T("Photoshop::打开背景图片"));
		TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_OPEN_PICTURE + _T("Photoshop::打开背景图片")),
			[&]()->int {
			MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\bg.png"), szSourceFP);
		pDocBg = m_pApp->Open(szSourceFP);
		SleepAndRecord(dwTimerNode3);
		return BENCH_MARK_OK; });
		{
			Photoshop::DocumentPtr pDocTemp = NULL;

			LogInfo(_T("Photoshop::打开美女图片"));
			TakeMeasurement(  new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_OPEN_PICTURE + _T("Photoshop::打开美女图片")),
				[&]()->int {
				MyPlatform::GetFullPathUnderExe(_T("Resources\\Photoshop\\bingbing.png"), szSourceFP);
			pDocTemp = m_pApp->Open(szSourceFP);
			SleepAndRecord(dwTimerNode3);
			m_pApp->ActiveDocument = pDocTemp;
			SleepAndRecord(dwTimerNode3);
			Photoshop::ArtLayerPtr pSrcArtLayer = pDocTemp->ArtLayers->GetItem((long)1);
			m_pApp->ActiveDocument = pDocBg;
			SleepAndRecord(dwTimerNode3);
			pArtLayer = pDocBg->ArtLayers->GetItem((long)1);
			SleepAndRecord(dwTimerNode3);
			m_pApp->ActiveDocument = pDocTemp;
			SleepAndRecord(dwTimerNode3);
			pSrcArtLayer->Duplicate(pArtLayer.GetInterfacePtr(), (long)Photoshop::psPlaceBefore);
			SleepAndRecord(dwTimerNode3);
			pDocTemp->Close((long)Photoshop::PsSaveOptions::psDoNotSaveChanges);
			SleepAndRecord(dwTimerNode3);
			pDocTemp->Release();
			pDocTemp = NULL;
			return BENCH_MARK_OK; });
		}

		LogInfo(_T("Photoshop::虚化背景..."));
		TakeMeasurement( new CCounterContent(CPhotoShop_Opids::DESC_TYPE_MARKER, CPhotoShop_Opids::DESC_VIRTUALIZE_BACKGROUND),
			[&]()->int {
		SleepAndRecord( dwTimerNode3);
		int nFilterCount = g_ini.GetInt(CFG_PHOTOSHOP, CFG_PHOTOSHOP_FILTER_COUNT, 5);
		int nFilterValue = g_ini.GetInt(CFG_PHOTOSHOP, CFG_PHOTOSHOP_FILTER_VALUE, 40);
		for (int nIndex = 0; nIndex < nFilterCount; nIndex += 1)
		{
			LogInfo(_T("Photoshop::最小值滤镜"));
			pArtLayer->ApplyMinimum(nFilterValue);
			SleepAndRecord( dwTimerNode3);
 		//	LogInfo(_T("Photoshop::最大值滤镜"));
			//pArtLayer->ApplyMaximum(30);
 		//	SleepAndRecord(2 * dwTimerNode3 * 2);
			/*
			LogInfo(_T("Photoshop::中值滤镜"));
			pArtLayer->ApplyMedianNoise(40);
			SleepAndRecord(2 * dwTimerNode3 * 2);
			*/
		}
		return BENCH_MARK_OK; 
	});
		LogInfo(_T("Photoshop::关闭工作台..."));
		SleepAndRecord( 2 * dwTimerNode3 );
		pDocBg->Close((long)Photoshop::PsSaveOptions::psDoNotSaveChanges);
		pDocBg->Release();
		pDocBg = NULL;

		m_pApp->Quit();
		//m_pApp->DoJavaScript(_T("executeAction(app.charIDToTypeID('quit'), undefined, DialogModes.NO);"));
	}
	catch (_com_error& err)
	{
		LogError({ _T("CPhotoshopMark::makePeoplePoster throw error=[%S]"), err.ErrorMessage() });
		return -1;
	}

	return BENCH_MARK_OK;
}

