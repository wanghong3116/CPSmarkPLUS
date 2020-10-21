#include "stdafx.h"
#include "ResultReport.h"
#include "OfficeRegisterWindow.h"
#include <iterator>
const TCHAR* REPORT_DIR = MyPlatform::ResultsFolder.c_str();

using namespace MyPlatform;

ResultReport::ResultReport(const std::vector<MarkWorkRunningInfor>* pvMarkWorkRunInfor, const int pnAchievement[E_MARK_CLASSIFY_END],
	const TCHAR* szCPUName, const TCHAR* szCPUHz, const TCHAR* szCPUNumber, const TCHAR* szMemory, const TCHAR* szGPU,  
	const TCHAR* szOSNameAndVersion, const TCHAR* szPDFFileName
	, CWorkMark* pworkMark, map<EMarkType, std::tstring>* mapItemToName)
{
	m_szPDFFileName = szPDFFileName;
	m_pvMarkWorkRunInfor = pvMarkWorkRunInfor;
	m_pnAchievement = pnAchievement;
	
	m_szCPUName = szCPUName;
	m_szCPUHz = szCPUHz;
	m_szCPUNumber = szCPUNumber;
	m_szMemory = szMemory;
	m_szGPU = szGPU;
	m_pworkMark = pworkMark;
	m_mapItemToName = mapItemToName;
	m_szOSNameAndVersion = szOSNameAndVersion;
	m_pworkMark = pworkMark;
}

ResultReport::~ResultReport()
{

}

int ResultReport::init()
{
	CoInitialize(NULL);
	MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	Sleep(100);
	MyPlatform::GetFullPathUnderExe(_T("skin\\ResultTemplate.pptx"), m_szTemplateFP, ARRARY_COUNT(m_szTemplateFP)); //orig. code line
	
	if (0 != _taccess(m_szTemplateFP, 0))
	{
		return -1;
	}
	
	try {

		OfficeRegisterWindow wndOfficeReg; 
		wndOfficeReg.RegisterWindowHandler([&]()->int {
			HRESULT hr = m_app.CreateInstance(__uuidof(PowerPoint::Application));
			if (FAILED(hr)) {
				m_pworkMark->LogError({ _T("ResultReport::init() Error") });
				return -2;
			}
			//lpLog->I(GetProjectName(), TEXT("PowerPoint.Application is started and visible"));
			m_app->PutVisible(Office::MsoTriState::msoTrue);
			m_app->WindowState = PowerPoint::PpWindowState::ppWindowMaximized;
			return BENCH_MARK_OK;
		});
	
	}
	catch (_com_error& err ) {
		m_pworkMark->LogError(_T("ResultReport::init() Com Error %S"),  err.ErrorMessage());
		return -3;
	}

	return BENCH_MARK_OK;
}


int ResultReport::Generate()
{
	TCHAR szDstPdfFP[_MAX_PATH] = { 0 };
	TCHAR szDir[_MAX_PATH] = { 0 };
	SYSTEMTIME st = { 0 };

	try
	{
		m_presentation = m_app->Presentations->Open(m_szTemplateFP, Office::MsoTriState::msoFalse, Office::MsoTriState::msoFalse, Office::MsoTriState::msoTrue);
		
		long nSlideCount = m_presentation->GetSlides()->GetCount();
		m_pworkMark->LogInfo({ _T("ResultReport::Generate() 3 ") });
		if (nSlideCount != 3)
		{
			return -2;
		}

		m_pworkMark->LogInfo({ _T("ResultReport::SecondPage()  ") });
		SecondPage();
		m_pworkMark->LogInfo({ _T("ResultReport::ThirdPage() ") });
		ThirdPage();
		m_pworkMark->LogInfo({ _T("ResultReport::SaveAs() ") });
		MyPlatform::GetFullPathUnderExe(REPORT_DIR, szDir, _ARRAYSIZE(szDir));
		MyPlatform::CreateFullDirectory(szDir);
		GetLocalTime(&st);
		_stprintf_s(szDstPdfFP, _T("%s\\%s\\%s.pdf"), szDir, m_szPDFFileName, m_szPDFFileName);
		
		m_presentation->SaveAs(szDstPdfFP, PowerPoint::PpSaveAsFileType::ppSaveAsPDF, Office::MsoTriState::msoTrue);
		_stprintf_s(szDstPdfFP, _T("%s\\%d-%d-%d %d-%d-%d.pdf.pptx"), szDir, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		m_pworkMark->LogInfo({ _T("ResultReport::Generate() SaveAs ") });

		//m_presentation->SaveAs(szDstPdfFP, PowerPoint::PpSaveAsFileType::ppSaveAsPowerPoint7, Office::MsoTriState::msoTrue);
		Sleep(120);
		m_presentation->Close();
		
		Sleep(120);
		m_app->Quit();
		m_pworkMark->LogInfo({ _T("ResultReport::SummaryExcelFile ") });
		SummaryExcelFile(szDir);

		Sleep(2000);
	}
	catch (const _com_error& err)
	{
		m_pworkMark->LogError(_T("ResultReport::Generate() Com Error %S"), err.ErrorMessage());
		return -1;
	}


	return BENCH_MARK_OK;
}

int ResultReport::destroy()
{
	MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	CoUninitialize();
	return BENCH_MARK_OK;
}

int ResultReport::SummaryExcelFile(const std::tstring szDir)
{
	try
	{
		m_pworkMark->LogInfo({ _T("ResultReport::SummaryExcelFile") });
		TCHAR szText[_MAX_PATH] = { 0 };
		std::tstring filename = szDir;
		filename = filename.append(_T("\\")).append(m_szPDFFileName).append(_T("\\")).append(_T("SummaryFile_")).append(m_szPDFFileName).append(_T(".csv"));
		m_pworkMark->LogInfo({ _T("ResultReport::SummaryExcelFile Output filename is "), filename });
		
		wcvt_utf16le_bom cvt(1);
		std::wofstream	outfile;
		std::locale wloc(outfile.getloc(), &cvt);
		outfile.imbue(wloc);
		outfile.open(filename, std::wofstream::out | std::wofstream::binary);
		m_pworkMark->cur_col = 1;
		m_pworkMark->cur_row = 1;
		vector<std::tstring> vecRuns = {_T("Runs")};
		for (int run = 1; run <= m_pworkMark->m_nLoopRunTime; run++)
			vecRuns.push_back(MyPlatform::Int64ToStringW(run));
		m_pworkMark->LogInfoToExcel(outfile, vecRuns);

		for (int index = E_MARK_CLASSIFY_END; index >= E_MARK_CLASSIFY_BEG; index--)
		{
			for (size_t nMarkIndex = 0; nMarkIndex < m_pvMarkWorkRunInfor->size(); nMarkIndex++)
			{
				const MarkWorkRunningInfor& mwri = (*m_pvMarkWorkRunInfor)[nMarkIndex];
				if (mwri.eMarkClassify == index && mwri.nRunningTime.size() == m_pworkMark->m_nLoopRunTime)
				{
					vector<std::tstring> vec{ (*m_mapItemToName)[mwri.eMarkType] };
					for (int i = 0; i < m_pworkMark->m_nLoopRunTime; i++)
						vec.push_back(MyPlatform::Int64ToStringW(mwri.nRunningTime[i]));
					m_pworkMark->LogInfoToExcel(outfile, vec);
				}
			}
		}
		if (outfile.is_open())
			outfile.close();
		Sleep(100);
		//Add delete file filename :


	}
	catch (_com_error& err) {
		m_pworkMark->LogError(_T("ResultReport::SummaryExcelFile() Com Error %S"), err.ErrorMessage());
		return -3;
	}
	catch (exception& ex)
	{
		m_pworkMark->LogError(_T("ResultReport::SummaryExcelFile() General Error。%S"), MyPlatform::CharToTchar(ex.what()));
		return -3;
	}
	return BENCH_MARK_OK;
}

int ResultReport::SecondPage()
{
	FillResult(2);


	return BENCH_MARK_OK;
}

int ResultReport::ThirdPage()
{
	PowerPoint::_SlidePtr pageSlider = m_presentation->Slides->Item(_variant_t((long)3));
	PowerPoint::ShapePtr oShapeResult = pageSlider->Shapes->Item(_variant_t((long)2));
	PowerPoint::TablePtr tableResult = oShapeResult->Table;

	pageSlider->Select();
	tableResult->Cell(2, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(m_szCPUName);
	tableResult->Cell(3, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(m_szCPUHz);
	tableResult->Cell(4, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(m_szCPUNumber);
	tableResult->Cell(5, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(m_szMemory);
	tableResult->Cell(6, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(m_szGPU);
	tableResult->Cell(7, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(m_szOSNameAndVersion);
	
	return BENCH_MARK_OK;
}

int ResultReport::FillResult(int nPageNumber)
{
	try
	{
		m_pworkMark->LogInfo(_T("ResultReport::FillResult nPageNumber = %S"),  MyPlatform::Int64ToStringW(nPageNumber) );
		TCHAR szText[_MAX_PATH] = { 0 };
		
		bool isApplicationOnly = false;
		bool isCalculationOnly = false;

		int nFinalAchievement = 0;
		if ((0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation])
			&& (0 == m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application]))
		{
			nFinalAchievement = m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation];
			isCalculationOnly = true;
		}
		else if ((0 == m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation])
			&& (0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application]))
		{
			nFinalAchievement = m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application];
			isApplicationOnly = true;
		}
		else if ((0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation])
			&& (0 != m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application]))
		{
			nFinalAchievement = m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Calculation] *
				m_pnAchievement[E_MARK_CLASSIFY_Comprehensive_Application];

			nFinalAchievement = ::sqrt(nFinalAchievement);
		}

		_tcscpy_s(szText, _ARRAYSIZE(szText), ((tstring)(MyPlatform::Int64ToStringW(nFinalAchievement))).c_str());

		if (isCalculationOnly)
			_tcscat_s(szText, _ARRAYSIZE(szText), _T(" (只是综合计算)"));
		else if (isApplicationOnly)
			_tcscat_s(szText, _ARRAYSIZE(szText), _T(" (只是综合应用)"));
		else 
			_tcscat_s(szText, _ARRAYSIZE(szText), _T(" (综合应用+综合计算)")); //eg. both Application + Calculation modes
			
		OfficeRegisterWindow wndOfficeReg;
		wndOfficeReg.RegisterWindowHandler([&]()->int {
		PowerPoint::_SlidePtr pageSlider = m_presentation->Slides->Item(_variant_t((long)nPageNumber));
		pageSlider->Select();
		 
		/// 成绩
		PowerPoint::ShapePtr oShapeAchievement = pageSlider->Shapes->Item(_variant_t((long)2));
		oShapeAchievement->Table->Cell(2, 3)->Shape->TextFrame->TextRange->Text = _bstr_t(((tstring)(szText)).c_str());
		
		/// 图标
		PowerPoint::ShapePtr oShapeChart = pageSlider->Shapes->AddChart(Office::XlChartType::xl3DColumnClustered, 500, 200, 380, 285);
		oShapeChart->Select(Office::MsoTriState::msoTrue);
		PowerPoint::ChartPtr pptChart = oShapeChart->Chart;
		PowerPoint::ChartDataPtr pptChartData = pptChart->ChartData;
		CStringA tstrRange;
		Excel::_WorkbookPtr excelWorkbook;
		Excel::_WorksheetPtr excelSheets;
		Excel::RangePtr excelRange;
		Excel::ListObjectPtr excelListobj1;
		 	excelWorkbook = (Excel::_WorkbookPtr)pptChartData->Workbook;
			excelSheets = (Excel::_WorksheetPtr)excelWorkbook->Sheets->Item[1]/*->Cells*/;//Excel::SheetsPtr
			tstrRange.Format("%c%i", (CHAR)(3 + 64), 2);;
			excelRange = excelSheets->Cells->GetRange("A1", (const char*)tstrRange);
			excelListobj1 = excelSheets->ListObjects->Item[1];
			excelListobj1->Resize(excelRange);
			wchar_t ppszTableContent[2][3][_MAX_PATH] = {
			{L"", L"基准平台", L"测试平台"},
			{L"性能评分", L"1000", L""},
		};	
		wcscpy_s(ppszTableContent[1][2], _ARRAYSIZE(ppszTableContent[1][2]), std::to_wstring(nFinalAchievement).c_str());
		m_pworkMark->LogInfo( _T("Type is %S "), ((isCalculationOnly || isApplicationOnly) ? 
			(isCalculationOnly ? _T("Comprehensive Calculation Score Only") : _T("Comprehensive Application Score Only") ) 
			: _T("Comprehensive Combined (Application/Calculation) Score")));
		m_pworkMark->LogInfo( _T("Total Runtime is %S "),  MyPlatform::Int64ToStringW(nFinalAchievement));

		tstrRange = "";
		for (int irow = 1; irow <= 2; irow++)
		{
			for (int icolumn = 1; icolumn <= 3; icolumn++)
			{
				tstrRange.Format("%c%i", (CHAR)(icolumn + 64), irow);
				excelRange = excelSheets->Range[(const char*)tstrRange];
				excelRange->Select();
				excelRange->Value2 = _bstr_t(ppszTableContent[irow - 1][icolumn - 1]);
				tstrRange = "";
				Sleep(100);
			}
		}
		excelWorkbook->Close();
		return BENCH_MARK_OK;
		});

		Sleep(100);
	}
	catch (_com_error& err) {
		m_pworkMark->LogError(_T("ResultReport::FillResult() Com Error %S "),  err.ErrorMessage());
		return -3;
	}
	catch (...)
	{
		m_pworkMark->LogError( _T("ResultReport::FillResult() General Error on page %S "), MyPlatform::Int64ToStringW(nPageNumber) );
		return -3;
	}
// 	Word::_ApplicationPtr app2 = wordChart->Application;
// 		PowerPoint::ChartDataPtr pptChartData = pptChart->ChartData;
// 		Excel::_WorkbookPtr excelWorkbook = (Excel::_WorkbookPtr)pptChartData->Workbook;
// 	
// 		wndOfficeRegister.CloseAsync();
// 		SleepAndRecord(500);
// 		wndOfficeRegister.WaitClose();
// 	
// 		Excel::_WorksheetPtr excelSheets = (Excel::_WorksheetPtr)excelWorkbook->Sheets->Item[1]/*->Cells*/;//Excel::SheetsPtr
// 		Excel::RangePtr excelRangeResultValue = excelSheets->Range[(const char*)"C2"];
// 		//excelRangeResultValue->Select();
// 		excelRangeResultValue->Value2 = _bstr_t(m_pnAchievement[eMarkClassify]);
// 		excelWorkbook->Close();
//	SleepAndRecord(500);
	return BENCH_MARK_OK;
}
