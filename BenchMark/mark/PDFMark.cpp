#include "stdafx.h"
#include "MarkObject.h"
#include "PDFMark.h"
#include "OfficeRegisterWindow.h"
#include "PDF_Opids.h"
#include "PPT_Opids.h"

CPDFMark::CPDFMark()
{
	LogInfo(_T("Invoke CPDFMark().."));

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Acrobat-PDF转换"));
	m_eMarkType = E_MARK_PDF;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("54782997f1ac2d27142ba5e090debc3f")); //latest md5
	//SetMD5PassWd(std::string("02288fddf2b55610c9d833d3cd4b0245"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\pdf.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

CPDFMark::~CPDFMark()
{
	LogInfo(_T("Invoke ~CPDFMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\pdf\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CPDFMark::init()
{
	HRESULT hr = S_OK;
	
	CoInitialize(NULL);
	//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	SleepAndRecord(dwTimerNode2);
	OfficeRegisterWindow wndOfficeReg(this);

	TakeMeasurement( new CCounterContent(CPDF_Opids::DESC_TYPE_MARKER, CPDF_Opids::DESC_OPEN_APPLICATION),
		[&]()->int {
		wndOfficeReg.RegisterWindowHandler([&]()->int {
		 hr = m_app.CreateInstance(__uuidof(PowerPoint::Application));
		 if (S_OK != hr)
		 {
			 LogError({ _T("初始化POWERPNT失败,可能未正确安装对应的版本\r\n") });
			 return -1;
		 }
		 try {
			 m_app->PutVisible(Office::MsoTriState::msoTrue);
			 m_app->WindowState = PowerPoint::PpWindowState::ppWindowMaximized;

		 }
		 catch (_com_error &err) {
			 LogError({ _T("Office PPT：PPT程序启动失败，可能没有正确安装"), (const TCHAR*)err.ErrorMessage() });
			 return -3;
		 }
		 return BENCH_MARK_OK;
		 });
	 	return BENCH_MARK_OK; 
	});
		
	
	TakeMeasurement(  new CCounterContent(CPDF_Opids::DESC_TYPE_MARKER, CPDF_Opids::DESC_OPEN_APPLICATION_ACROBAT),
		[&]()->int {
		MyPlatform::GetFullPathUnderExe(_T("Resources\\pdf\\ppt.pptx"), m_szPPTFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\pdf\\ppt_exportpdf.pdf"), m_szDstPdfFP);
	if (0 != _taccess(m_szPPTFP, 0))
	{
		LogError({ _T("没有找到要转换的PPT文件，可能被意外删除"), m_szPPTFP  });
		return -4;
	}

	//MyPlatform::KillProcessFromName(_T("Acrobat.exe"));
	SleepAndRecord(dwTimerNode2);
	 	hr = m_pAcroApp.CreateInstance(_T("AcroExch.App"));
		if (S_OK != hr)
		{
			LogError({ _T("初始化Acrobat失败,可能未正确安装对应的版本\r\n") });
			return -5;
		}
		hr = m_pAcroAVDoc.CreateInstance(_T("AcroExch.AVDoc"));
		if (S_OK != hr)
		{
			LogError({ _T("初始化Acrobat失败,可能未正确安装对应的版本\r\n") });
			return -6;
		}
	 return BENCH_MARK_OK; });

	
	SleepAndRecord(dwTimerNode2);

	return BENCH_MARK_OK;
}

int CPDFMark::run()
{
	 	if (BENCH_MARK_OK != convertPPTToPdf())
		{
			return -1;
		}
	/*	if (BENCH_MARK_OK != openPdfAndDisplay())
		{
			return -2;
		}
	*/
	 	
	return BENCH_MARK_OK;
}

int CPDFMark::destroy()
{
	LogInfo(_T("Invoke CPDFMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\pdf\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	//MyPlatform::KillProcessFromName(_T("Acrobat.exe"));
	SleepAndRecord(dwTimerNode2 * 2);
	//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	SleepAndRecord(dwTimerNode2 * 2);
	return BENCH_MARK_OK;
}

int CPDFMark::convertPPTToPdf()
{
	try
	{
		m_app->PutVisible(Office::MsoTriState::msoTrue);
		m_app->WindowState = PowerPoint::PpWindowState::ppWindowMaximized;
		PowerPoint::_PresentationPtr oPresentation;
		PowerPoint::SlidesPtr oSlides;
		TakeMeasurement(  new CCounterContent(CPDF_Opids::DESC_TYPE_MARKER, CPDF_Opids::DESC_OPEN_DOCUMENT),
			[&]()->int {
		DeleteFile(m_szDstPdfFP);
		LogInfo({ _T("PDFMake:打开文件"), PathFindFileName(m_szPPTFP) });
		oPresentation = m_app->Presentations->Open(m_szPPTFP, Office::MsoTriState::msoFalse, Office::MsoTriState::msoFalse, Office::MsoTriState::msoTrue);
		SleepAndRecord(dwTimerNode3);
		return BENCH_MARK_OK; });

		oSlides = oPresentation->Slides;
		int nSlides = oSlides->Count, nLimit = 20;
		for (int i = 1; i <= nSlides && i <= nLimit; i++) {
			LogInfo({ _T("PDFMake:显示第"), MyPlatform::Int64ToStringW(i + 1), _T("页") });
			VARIANT var;
			VariantInit(&var);
			oSlides->raw__Index(i, &var);
			PowerPoint::_SlidePtr oSlide = var.pdispVal;
			oSlide->Select();
			VariantClear(&var);
			SleepAndRecord(dwTimerNode1);
		}
		LogInfo(_T("PDFMake:保存为PDF"));
		TakeMeasurement( new CCounterContent(CPDF_Opids::DESC_TYPE_MARKER, CPDF_Opids::DESC_SAVE_AS_PDF_DOCUMENT),
			[&]()->int {
		oPresentation->SaveAs(m_szDstPdfFP, PowerPoint::PpSaveAsFileType::ppSaveAsPDF, Office::MsoTriState::msoTrue);
		SleepAndRecord(dwTimerNode3);
		return BENCH_MARK_OK; 
	});
		oPresentation->Close();
		SleepAndRecord(dwTimerNode3);
		m_app->Quit();
		//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	}
	catch (_com_error& err)
	{
		LogError({ _T("PDF convertPPTToPdf error"), err.ErrorMessage() });
		return -1;
	}

	return BENCH_MARK_OK;
}

int CPDFMark::openPdfAndDisplay()
{
	HRESULT hr = S_OK;

	try
	{
		LogInfo({ _T("PDFMake:Acrobat打开文件"), PathFindFileName(m_szDstPdfFP) });
		VARIANT_BOOL bResult = m_pAcroAVDoc->Open(m_szDstPdfFP, _T(""));
		if (VARIANT_FALSE == bResult)
		{
			return -2;
		}
		SleepAndRecord(dwTimerNode2);
		m_pAcroApp->Show();
		SleepAndRecord(dwTimerNode2);
		Acrobat::CAcroPDDocPtr pPDDoc = m_pAcroAVDoc->GetPDDoc();
		long lNum = pPDDoc->GetNumPages();
		Acrobat::CAcroAVPageViewPtr pView = m_pAcroAVDoc->GetAVPageView();
		for (long lPageIndex = 0; lPageIndex < lNum && lPageIndex < 20; lPageIndex++)
		{
			LogInfo({ _T("PDFMake:Acrobat显示第"), MyPlatform::Int64ToStringW(lPageIndex + 1), _T("页") });
			pView->GoTo(lPageIndex);
			SleepAndRecord(dwTimerNode2);
		}
		pPDDoc->Release();
		pPDDoc = NULL;
		pView->Release();
		pView = NULL;
		m_pAcroAVDoc->Close(1);
		SleepAndRecord(dwTimerNode2);
		m_pAcroAVDoc->Release();
		m_pAcroAVDoc = NULL;

		m_pAcroApp->CloseAllDocs();
		m_pAcroApp->Hide();
		bResult = m_pAcroApp->Exit();
		m_pAcroApp->Release();
		m_pAcroApp = NULL;
		//MyPlatform::KillProcessFromName(_T("Acrobat.exe"));

	}
	catch (_com_error& err)
	{
		LogError({ _T("PDF openPdfAndDisplay error"),  err.ErrorMessage() });
		return -1;
	}

	return BENCH_MARK_OK;
}
