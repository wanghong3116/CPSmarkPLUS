#include "stdafx.h"
#include "MarkObject.h"
#include "OfficeExcelMark.h"
#include "OfficeRegisterWindow.h"
#include "Excel_Opids.h"

using namespace MyPlatform;
 
COfficeExcelMark::COfficeExcelMark()
{
	LogInfo(_T("Invoke COfficeExcelMark().."));

	TCHAR szZipDir[_MAX_PATH] = { 0 };
	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Excel-办公性能测试"));
	m_eMarkType = E_MARK_OFFICE_EXCEL;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	SetMD5PassWd(std::string("e32ddbf53aac7513a2b84acd46e565e5")); //latest md5
	//SetMD5PassWd(std::string("ea32cfd3523f4c04f7d398cca8b0f761"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

COfficeExcelMark::~COfficeExcelMark()
{
	LogInfo(_T("Invoke ~COfficeExcelMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int COfficeExcelMark::init()
{
	//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	SleepAndRecord(500);
	CoInitialize(NULL);

	try 
	{
		
		Excel::_ApplicationPtr oApp;
			TakeMeasurement(new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_OPEN_APPLICATION),
				[&]()->int {
				OfficeRegisterWindow wndOfficeReg(this);
				wndOfficeReg.RegisterWindowHandler([&]()->int {

				HRESULT hr = oApp.CreateInstance(__uuidof(Excel::Application));
				if (FAILED(hr)) {
					LogError({ _T("Office Excel没有正确安装") });
					return -2;
				}
				g_ApplicationPtr.Attach(oApp.Detach());
				//lpLog->I(GetProjectName(), TEXT("Excel.Application is started and visibale"));
				g_ApplicationPtr->Visible[0] = VARIANT_TRUE;
				g_ApplicationPtr->DisplayAlerts[0] = VARIANT_FALSE;
				g_ApplicationPtr->AlertBeforeOverwriting[0] = VARIANT_FALSE;
				g_ApplicationPtr->WindowState[0] = Excel::XlWindowState::xlMaximized;

				g_ApplicationPtr->Workbooks->Add();
				return BENCH_MARK_OK;
			});
				return BENCH_MARK_OK;
			});
		
		
		
	}
	catch (_com_error &err) {
		LogError({ _T("Excel执行错误，错误原因"), err.ErrorMessage() });
		return -1;
	};

	return BENCH_MARK_OK;
}

int COfficeExcelMark::run()
{
	return ExcelDemo2();
}

int COfficeExcelMark::destroy()
{
	LogInfo(_T("Invoke COfficeExcelMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	CoUninitialize();
	SleepAndRecord(500);
	
	return BENCH_MARK_OK;
}

typedef double Double;
typedef int Int;


extern double dwTimerNode;
extern double dwTimerNode1;
extern double  dwTimerNode2;
extern double  dwTimerNode3;

#ifndef TAG
#define TAG ""
#endif

#ifndef MARK_CODE_NODE
#define MARK_CODE_NODE
#endif
Excel::RangePtr  COfficeExcelMark::SelectChartRange(std::tstring RangeIndex, std::tstring RangeValue,  Excel::_WorksheetPtr oWorksheet)
{
	Excel::RangePtr oRange = oWorksheet->Range[RangeIndex.c_str()];
	TakeMeasurement( new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_SELECT_CHART_RANGE + RangeValue),
	[&]()->int {

	SleepAndRecord(dwTimerNode1);
	oRange->Select();
	oRange->Merge(_variant_t((long)0));
	oRange->Value2 = RangeValue.c_str();
	SleepAndRecord(dwTimerNode);
	oRange->Font->Name = _bstr_t(TEXT("华文楷体"));
	SleepAndRecord(dwTimerNode);
	oRange->Font->Size = 18.0;
	SleepAndRecord(dwTimerNode);
	oRange->Font->Italic = 1;
	SleepAndRecord(dwTimerNode);
	oRange->Font->Bold = 1;
	SleepAndRecord(dwTimerNode);
	oRange->RowHeight = 39.0;
	SleepAndRecord(dwTimerNode);
	return BENCH_MARK_OK;
	});
	return oRange;
}

Excel::RangePtr COfficeExcelMark::FillRange(std::function<void(Excel::RangePtr)> action,  std::tstring RangeIndex, std::tstring Title, std::tstring Value2,  Excel::_WorkbookPtr oWorkbook, Excel::_WorksheetPtr oWorksheet)
{
	Excel::RangePtr oRange = oWorksheet->Range["C32:G32"];
	TakeMeasurement( new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_FILL_RANGE + Title),
	[&]()->int {

	SleepAndRecord(dwTimerNode2);
	oRange->Select();
	oRange->Merge(_variant_t((long)0));
	oRange->Font->Name = _bstr_t(TEXT("华文楷体"));
	oRange->Value2 = Value2.c_str();

	action(oRange);

	oRange = oWorksheet->Range[_variant_t(L"C32:G40")];
	oRange->Select();
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeLeft)->LineStyle = 1;
	SleepAndRecord(dwTimerNode1);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeTop)->LineStyle = 1;
	SleepAndRecord(dwTimerNode1);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeRight)->LineStyle = 1;
	SleepAndRecord(dwTimerNode1);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeBottom)->LineStyle = 1;
	SleepAndRecord(dwTimerNode1);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlInsideVertical)->LineStyle = 1;
	SleepAndRecord(dwTimerNode1);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlInsideHorizontal)->LineStyle = 1;
	SleepAndRecord(dwTimerNode1);
	return BENCH_MARK_OK;
	});
	return oRange;
}

void COfficeExcelMark::AddChartWithRangeAndAction(Action action, Excel::XlChartType ChartType, float ShapeLeft, float ShapeRight, std::tstring RangeIndex, std::tstring Title, std::tstring ChartTitle, std::tstring seriesName, Excel::_WorkbookPtr oWorkbook, Excel::_WorksheetPtr oWorksheet)
{
	TakeMeasurement( new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_ADD_CHART + Title),
		[=]()->int{
	
	SleepAndRecord(dwTimerNode);
	Excel::RangePtr oRange = oWorksheet->Range[_variant_t(RangeIndex.c_str())];
	oRange->Select();
	Excel::_WorksheetPtr oWorksheet2 = oWorkbook->ActiveSheet;
	Excel::ShapePtr oShape = oWorksheet2->Shapes->AddChart();
	oShape->Select();
	Excel::_ChartPtr oChart = oWorkbook->ActiveChart;
	SleepAndRecord(dwTimerNode);

	/// 模拟鼠标点击
	action();

	//browseChartByCursor((HWND)g_ApplicationPtr->ActiveWindow->Hwnd);
	oChart->ChartType = ChartType;
	oChart->SetSourceData(oWorksheet->Range[RangeIndex.c_str()], vtMissing);
	//oChart->Name = "Chart 3.1";
	//oChart->ApplyLayout(9);
	oChart->ApplyLayout(1);
	SleepAndRecord(dwTimerNode);
	oChart->ChartTitle->Select();
	oChart->ChartTitle->Text = ChartTitle.c_str();
	SleepAndRecord(dwTimerNode);
	oChart->ChartArea->Select();
	Excel::SeriesPtr charSeries = (Excel::SeriesPtr)oChart->SeriesCollection(1);//->Name = "=Sheet1!$C$32"
																				//oChart->SeriesCollection;
																				//charSeries->XValues = "=Sheet1!$D$34:$G$34";
																				//charSeries->Values = "=Sheet1!$D$40:$G$40";
	charSeries->Name = seriesName.c_str();
	oShape->Left = ShapeLeft;
	SleepAndRecord(dwTimerNode);
	oShape->Top = ShapeRight;
	SleepAndRecord(dwTimerNode);
	return BENCH_MARK_OK; 
	});
}

Excel::RangePtr COfficeExcelMark::FillChart(std::function<void(Excel::RangePtr)> action, std::tstring RangeIndex, std::tstring RangeValue,  Excel::_WorkbookPtr oWorkbook, Excel::_WorksheetPtr oWorksheet)
{
	Excel::RangePtr oRange = oWorksheet->Range[RangeIndex.c_str()];
	TakeMeasurement( new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_FILL_CHART + RangeValue),
		[&]()->int {
	SleepAndRecord(dwTimerNode);
	oRange->Select();
	oRange->Merge(_variant_t((long)0));
	oRange->Font->Name = _bstr_t(TEXT("华文楷体"));
	oRange->Value2 = RangeValue.c_str();

	action(oRange);

	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeLeft)->LineStyle = 1;
	SleepAndRecord(dwTimerNode);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeTop)->LineStyle = 1;
	SleepAndRecord(dwTimerNode);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeRight)->LineStyle = 1;
	SleepAndRecord(dwTimerNode);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeBottom)->LineStyle = 1;
	SleepAndRecord(dwTimerNode);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlInsideVertical)->LineStyle = 1;
	SleepAndRecord(dwTimerNode);
	oRange->Borders->Get_Default(Excel::XlBordersIndex::xlInsideHorizontal)->LineStyle = 1;
	SleepAndRecord(dwTimerNode);
	//oRange->Font->Name = _bstr_t(TEXT("Calibri"));
	return BENCH_MARK_OK;
	});
	return oRange;
}

int COfficeExcelMark::ExcelDemo2()
{
	CString strRescFP;
	MyPlatform::tstring lpPathFile;
	
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), strRescFP.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
	strRescFP.ReleaseBuffer();
	try
	{
		MyPlatform::tstring lpPathFile = strRescFP + TEXT("excel_book_03.xlsx");
		Excel::_WorkbookPtr oWorkbook;
		Excel::_WorksheetPtr oWorksheet, oWorksheet2;
		Excel::RangePtr oRangeAll, oRange;

		{
			MyPlatform::tstring strNewFile = strRescFP + TEXT("excel_book_03_temp.xlsx");;
			DeleteFile(strNewFile.c_str());
			CopyFile(lpPathFile.c_str(), strNewFile.c_str(), FALSE);
			lpPathFile = strNewFile;
		}
		{
			TakeMeasurement(new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_OPEN_DOCUMENT + _T("excel_book_03_temp.xlsx")),
				[&]()->int {

			LogInfo({ _T("Excel:打开文件"), PathFindFileName(lpPathFile.c_str()) });
			oWorkbook = g_ApplicationPtr->Workbooks->Open(_bstr_t(lpPathFile.c_str()));

			oWorksheet = oWorkbook->Sheets->Item[1];
			oWorksheet->Select();
			return BENCH_MARK_OK; 
			});
	 	}
		{
			oRangeAll = oWorksheet->Cells;
		};

		//创建表头
		LogInfo(_T("Excel:选择工作区域"));
		SleepAndRecord(dwTimerNode2);
		{
			LogInfo(_T("Excel:添加表头"));
			//用户的年龄分布情况

			oRange = oWorksheet->Range["B2:Y94"];
			oRange->Font->Name = _bstr_t(TEXT("华文楷体"));
			oRange->Font->Size = 10.5;
			oRange->RowHeight = 16.5;
			oRange->HorizontalAlignment = _variant_t(Excel::XlHAlign::xlHAlignCenter);
			oRange->VerticalAlignment = _variant_t(Excel::XlVAlign::xlVAlignCenter);
			oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeLeft)->LineStyle = 1;
			SleepAndRecord(dwTimerNode1);
			oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeTop)->LineStyle = 1;
			SleepAndRecord(dwTimerNode1);
			oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeRight)->LineStyle = 1;
			SleepAndRecord(dwTimerNode1);
			oRange->Borders->Get_Default(Excel::XlBordersIndex::xlEdgeBottom)->LineStyle = 1;
			//SleepAndRecord(dwTimer1);
			//oRange->Borders->Get_Default(Excel::XlBordersIndex::xlInsideVertical)->LineStyle = 1;
			//SleepAndRecord(dwTimer1);
			//oRange->Borders->Get_Default(Excel::XlBordersIndex::xlInsideHorizontal)->LineStyle = 1;

			g_ApplicationPtr->ActiveWindow->SmallScroll(&_variant_t(30));
			
			{
				LogInfo(_T("Excel:产品认知度:选择表格区域"));
				oRange = SelectChartRange(_T("C31:H31"), _T("受调查用户对我公司产品的了解程度"), oWorksheet);
				 
				LogInfo(_T("Excel:产品认知度:填充数据"));
				oRange = FillRange([&oWorksheet, this](Excel::RangePtr oRange) {
					{
						CString tstrRange;
						CHAR *tvalue[] = { "分类","不知道","知道不了解","了解","非常了解",
							"18-30","4675","2675","5675","1675",
							"30-50","15000",	"20000",	"20000",	"5000",
							"50以上",	"6325",	"8325",	"7325",	"3325",
							"总计",	"26000",	"31000",	"33000",	"10000",
							"乡镇",	"4750",	"5750",	"6750",	"3750",
							"中型城市",	"6675",	"7675",	"8675",	"3675",
							"大型城市",	"13575",	"15575",	"16575",	"8575" };

						for (Int i = 0; i < 8; i++)
						{
							for (Int j = 0; j < 5; j++)
							{
								tstrRange.Format(TEXT("%c%i"), j + 67, i + 33);
								oRange = oWorksheet->Range[_variant_t(tstrRange)];
								oRange->Select();
								oRange->Value2 = tvalue[i * 5 + j];
								SleepAndRecord(dwTimerNode);
								if (i % 2)
									oRange->Interior->Color = 0xffbb77;
							}
						}
				 	}
				},
				_T("C32:G32"), _T("Excel:产品认知度:填充数据"), _T("产品认知度"),
					oWorkbook, oWorksheet);
				 
				{
					LogInfo(_T("Excel:生成图表:产品认知度"));
					AddChartWithRangeAndAction([=](){
						/// 模拟鼠标点击
						{
							HWND hWndExcel = (HWND)(g_ApplicationPtr)->ActiveWindow->Hwnd;
							CRect rectWindow;

							SetWindowPos(hWndExcel, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
							GetWindowRect(hWndExcel, rectWindow);

							{
								CPoint point(rectWindow.TopLeft());

								point.Offset(CSize(140, 50));
								doLButton(point.x, point.y);
								SleepAndRecord(dwTimerNode1);
							}


							{
								CSize sizeMenuOffset(581, 128);
								CArray<CSize> arraysizeMove;

								arraysizeMove.Add(CSize(598, 190));
								arraysizeMove.Add(CSize(654, 190));
								arraysizeMove.Add(CSize(713, 190));
								arraysizeMove.Add(CSize(598, 275));
								clickMenuAndMoveCursor(rectWindow.TopLeft(), sizeMenuOffset, arraysizeMove);
							}
							{
								CPoint point(rectWindow.TopLeft());

								point.Offset(CSize(140, 50));
								doLButton(point.x, point.y);
								SleepAndRecord(dwTimerNode);
							}
						}
					},
					Excel::XlChartType::xl3DPie, 105.0, 735.0, _T("D37:G37,D33:G33"), _T("Excel:生成图表:产品认知度"), _T("产品认知度"), _T("= Sheet1!$C$32"), oWorkbook, oWorksheet);
					 
				
					LogInfo(_T("Excel:生成图表:针对人群调查"));
					AddChartWithRangeAndAction([&]() {
						/// 模拟鼠标点击
						 
						CRect rectWindow;
						HWND hWndExcel = (HWND)(g_ApplicationPtr)->ActiveWindow->Hwnd;
						SetWindowPos(hWndExcel, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
						GetWindowRect(hWndExcel, rectWindow);

						{
							CPoint point(rectWindow.TopLeft());

							point.Offset(CSize(140, 50));
							doLButton(point.x, point.y);
							SleepAndRecord(500);
						}


						{
							CSize sizeMenuOffset(585, 85);
							CArray<CSize> arraysizeMove;

							arraysizeMove.Add(CSize(600, 150));
							arraysizeMove.Add(CSize(660, 150));
							arraysizeMove.Add(CSize(720, 150));

							arraysizeMove.Add(CSize(600, 220));
							arraysizeMove.Add(CSize(660, 220));
							clickMenuAndMoveCursor(rectWindow.TopLeft(), sizeMenuOffset, arraysizeMove);
						}

						{
							CPoint point(rectWindow.TopLeft());

							point.Offset(CSize(140, 50));
							doLButton(point.x, point.y);
							SleepAndRecord(100);
						}
					},
					Excel::XlChartType::xl3DColumnStacked, 540.0, 735.0, _T("C33:G36"), _T("Excel:生成图表:针对人群调查"),  _T("针对人群调查"), _T(""), oWorkbook, oWorksheet);
					 
				}
			}
			TakeMeasurement( new CCounterContent(CExcel_Opids::DESC_TYPE_MARKER, CExcel_Opids::DESC_CALCULATION_TEST + _T("计算测试")),
				[&]()->int {
			//Add 20 * 20 .
			oRange = oWorksheet->Range["N32:W32"];
			oRange->Select();
			oRange->Merge(_variant_t((long)0));
			oRange->Font->Name = _bstr_t(TEXT("华文楷体"));
			oRange->Value2 = "计算测试";
			CString tstrRange;
			CHAR *tvalue[] = { "999999","999999","999999","999999","999999",
				"888888","888888","888888","888888","888888",
				"777777","777777",	"777777",	"777777",	"777777",
				"666666",	"666666",	"666666",	"666666",	"666666",
				"555555",	"555555",	"555555",	"555555",	"555555",
				"444444",	"444444",	"444444",	"444444",	"444444",
				"555555",	"555555",	"555555",	"555555",	"555555",
				"999999",	"999999",	"999999",	"999999",	"999999" };

			for (Int i = 0; i < 6; i++)
			{
				for (Int j = 0; j < 10; j++)
				{
					tstrRange.Format(TEXT("%c%i"), j + 78, i + 33);
					oRange = oWorksheet->Range[_variant_t(tstrRange)];
					oRange->Select();

					UINT64 unResult = CalculateDemo();
					oRange->Value2 = unResult;

					SleepAndRecord(dwTimerNode);
					if (i % 2)
						oRange->Interior->Color = 0xffbb77;
				}
			}
			return BENCH_MARK_OK;
			});
		};

		SleepAndRecord(dwTimerNode3);

		MyPlatform::tstring savename = strRescFP + TEXT("产品调查报告.xlsx");
		//oWorkbook->Save();
		//oWorkbook->SaveAs(_variant_t(savename.c_str()), _variant_t(Excel::XlFileFormat::xlOpenXMLWorkbook), &vtMissing, &vtMissing, &vtMissing, &vtMissing, Excel::XlSaveAsAccessMode::xlNoChange);
		//oWorkbook->SaveAs(_variant_t(savename.c_str()),//const _variant_t & Filename,
		//					_variant_t(Excel::XlFileFormat::xlOpenXMLWorkbook),//const _variant_t & FileFormat,
		//					&vtMissing,//const _variant_t & Password, 
		//					&vtMissing,//const _variant_t & WriteResPassword,
		//					false,//const _variant_t & ReadOnlyRecommended,
		//					false,//const _variant_t & CreateBackup, 
		//					Excel::XlSaveAsAccessMode::xlNoChange,//enum XlSaveAsAccessMode AccessMode,
		//					&vtMissing,//const _variant_t & ConflictResolution,
		//					&vtMissing,//const _variant_t & AddToMru,
		//					&vtMissing,//const _variant_t & TextCodepage,
		//					&vtMissing,//const _variant_t & TextVisualLayout,
		//					&vtMissing,//const _variant_t & Local,
		//					true//long lcid 
		//);
		g_ApplicationPtr->Quit();
		DeleteFile(lpPathFile.c_str());
		return BENCH_MARK_OK;
	}
	catch (_com_error &err) {
		LogError({ _T("执行出现异常，异常原因"), err.ErrorMessage() });
		return -1;
	};
}

int COfficeExcelMark::browseChartByCursor(HWND hWndExcel)
{
	CRect rectWindow;

	SetWindowPos(hWndExcel, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
	GetWindowRect(hWndExcel, rectWindow);

	{
		CPoint point(rectWindow.TopLeft());

		point.Offset(CSize(140, 50));
		doLButton(point.x, point.y);
		SleepAndRecord(dwTimerNode);
	}
	
	
	{
		CSize sizeMenuOffset(585, 85);
		CArray<CSize> arraysizeMove;

		arraysizeMove.Add(CSize(600, 150));
		arraysizeMove.Add(CSize(660, 150));
		arraysizeMove.Add(CSize(720, 150));

		arraysizeMove.Add(CSize(600, 220));
		arraysizeMove.Add(CSize(660, 220));
		arraysizeMove.Add(CSize(720, 220));
		arraysizeMove.Add(CSize(780, 220));

		arraysizeMove.Add(CSize(600, 310));
		arraysizeMove.Add(CSize(660, 310));
		arraysizeMove.Add(CSize(720, 310));
		
		arraysizeMove.Add(CSize(600, 390));
		arraysizeMove.Add(CSize(660, 390));
		arraysizeMove.Add(CSize(720, 390));

		clickMenuAndMoveCursor(rectWindow.TopLeft(), sizeMenuOffset, arraysizeMove);
	}

	{
		CPoint point(rectWindow.TopLeft());

		point.Offset(CSize(140, 50));
		doLButton(point.x, point.y);
		SleepAndRecord(dwTimerNode);
	}

	{
		CSize sizeMenuOffset(585, 105);
		CArray<CSize> arraysizeMove;

		arraysizeMove.Add(CSize(600, 170));
		arraysizeMove.Add(CSize(660, 170));
		arraysizeMove.Add(CSize(720, 170));
		arraysizeMove.Add(CSize(780, 170));
		arraysizeMove.Add(CSize(830, 170));

		arraysizeMove.Add(CSize(600, 230));

		arraysizeMove.Add(CSize(600, 310));

		arraysizeMove.Add(CSize(600, 385));
		arraysizeMove.Add(CSize(660, 385));
		arraysizeMove.Add(CSize(720, 385));

		arraysizeMove.Add(CSize(600, 465));
		arraysizeMove.Add(CSize(660, 465));
		arraysizeMove.Add(CSize(720, 465));

		clickMenuAndMoveCursor(rectWindow.TopLeft(), sizeMenuOffset, arraysizeMove);
	}

	{
		CPoint point(rectWindow.TopLeft());

		point.Offset(CSize(140, 50));
		doLButton(point.x, point.y);
		SleepAndRecord(dwTimerNode);
	}

	{
		CSize sizeMenuOffset(585, 130);
		CArray<CSize> arraysizeMove;

		arraysizeMove.Add(CSize(600, 195));
		arraysizeMove.Add(CSize(660, 195));
		arraysizeMove.Add(CSize(720, 195));

		arraysizeMove.Add(CSize(600, 275));

		arraysizeMove.Add(CSize(600, 355));

		clickMenuAndMoveCursor(rectWindow.TopLeft(), sizeMenuOffset, arraysizeMove);
	}

	{
		CPoint point(rectWindow.TopLeft());

		point.Offset(CSize(140, 50));
		doLButton(point.x, point.y);
		SleepAndRecord(dwTimerNode);
	}

	return BENCH_MARK_OK;
}

int COfficeExcelMark::clickMenuAndMoveCursor(CPoint& rpointBase, CSize& rsizeMenuOffset, CArray<CSize>& rarraysizeMove)
{
	CPoint pointClick;

	pointClick = rpointBase;
	pointClick.Offset(rsizeMenuOffset);
	doLButton(pointClick.x, pointClick.y);
	SleepAndRecord(dwTimerNode1);

	int nArrayCount = rarraysizeMove.GetSize();
	for (int nIdx = 0; nIdx < nArrayCount; nIdx++)
	{
		CPoint pointTemp(rpointBase);

		pointTemp.Offset(rarraysizeMove[nIdx]);
		SetCursorPos(pointTemp.x, pointTemp.y);
		SleepAndRecord(dwTimerNode1);

		SetCursorPos(pointTemp.x + 1, pointTemp.y + 1);
		SleepAndRecord(dwTimerNode1);
	}

	return BENCH_MARK_OK;
}

UINT64 COfficeExcelMark::CalculateDemo()
{	
	int count = 10000;
	UINT64 arr[10000] = { 0 };

	UINT64 unResult = 0xff;
	UINT64 nSourceVal = 0xff;
	UINT64 nSourceTemp1 = 0xCC;
	UINT64 nSourceTemp2 = 0xCC;

	for (int index = 0; index < 10000; index++) {
		for (int i = 0; i < count; i++) {
			arr[i] = i + 1;
		}
		for (int i = 0; i < count; i++) {
			unResult = unResult * arr[i];
		}
		for (int i = 0; i < count; i++) {
			unResult = unResult / arr[i];
		}
		for (int i = 0; i < count; i++) {
			unResult = unResult % arr[i];
		}
		for (int i = 0; i < count; i++) {
			unResult = unResult - arr[i];
		}
	}

	return unResult;
}

/*
document.ready = function() {
	setTimeout(function() {
		var count = 10000;
		var num = 1;
		var arr = [];
		var nSourceTemp1 = 3000000;
		var unResult = 0;
		for (let i = 0; i < count; i++) {
			arr[i] = nSourceTemp1 + i;
		}
		for (let i = 0; i < count; i++) {
			unResult = unResult * arr[i];
		}
		for (let i = 0; i < count; i++) {
			unResult = unResult / arr[i];
		}
		for (let i = 0; i < count; i++) {
			unResult = unResult % arr[i];
		}
		for (let i = 0; i < count; i++) {
			unResult = unResult - arr[i];
		}
	}, 1000 * 60)
}
*/

