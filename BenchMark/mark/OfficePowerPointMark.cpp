#include "stdafx.h"
#include "MarkObject.h"
#include "OfficePowerPointMark.h"
#include "OfficeRegisterWindow.h"
#include "PPT_Opids.h"

using namespace MyPlatform;

COfficePowerPointMark::COfficePowerPointMark()
{
	LogInfo(_T("Invoke COfficePowerPointMark().."));

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("PowerPoint-办公性能测试"));
	m_eMarkType = E_MARK_OFFICE_POWERPOINT;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	SetMD5PassWd(std::string("e32ddbf53aac7513a2b84acd46e565e5")); //latest md5
	//SetMD5PassWd(std::string("ea32cfd3523f4c04f7d398cca8b0f761"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

COfficePowerPointMark::~COfficePowerPointMark()
{
	LogInfo(_T("Invoke ~COfficePowerPointMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int COfficePowerPointMark::init()
{
	CoInitialize(NULL);

	MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	SleepAndRecord(dwTimerNode1);

	try {
		
		TakeMeasurement(new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_OPEN_APPLICATION),
			[&]() 
		{
			OfficeRegisterWindow wndOfficeReg(this);
			wndOfficeReg.RegisterWindowHandler([&]()->int {
				HRESULT hr = g_ApplicationPtr.CreateInstance(__uuidof(PowerPoint::Application));
			if (FAILED(hr)) {
				return -2;
			}
			//lpLog->I(GetProjectName(), TEXT("PowerPoint.Application is started and visibale"));
			g_ApplicationPtr->PutVisible(Office::MsoTriState::msoTrue);
			g_ApplicationPtr->WindowState = PowerPoint::PpWindowState::ppWindowMaximized;
			return BENCH_MARK_OK; 
			});
			return BENCH_MARK_OK;
		});
		 
	}
	catch (_com_error &err) {
		LogError({ _T("Office PPT：PPT程序启动失败，可能没有正确安装"), (const TCHAR*)err.ErrorMessage() });
		return -1;
	}

	return BENCH_MARK_OK;
}

int COfficePowerPointMark::run()
{
	return PPTDemo2();
}

int COfficePowerPointMark::destroy()
{
	LogInfo(_T("Invoke COfficePowerPointMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	CoUninitialize();
	return BENCH_MARK_OK;
}


typedef double Double;
typedef int Int;
typedef float Float;


extern double dwTimerNode;
extern double dwTimerNode1;
extern double  dwTimerNode2;
extern double  dwTimerNode3;
 
void COfficePowerPointMark::FillPage(PowerPoint::_SlidePtr oSlide, CHAR **lpTexts, std::tstring Title)
{
	TakeMeasurement(new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_FILL_PAGE + Title),
		[=]()-> int {

	oSlide->SlideShowTransition->EntryEffect = PowerPoint::PpEntryEffect::ppEffectFadeSmoothly;
	oSlide->SlideShowTransition->PutAdvanceOnTime(Office::MsoTriState::msoTrue);
	oSlide->SlideShowTransition->AdvanceTime = 3.0f;

	{
		PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);
		LogInfo(TEXT("输入文档标题"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Text = lpTexts[0];
		oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
		LogInfo(TEXT("设置标题字体"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Font->Name = TEXT("微软雅黑 ");
		LogInfo(TEXT("设置标题字体大小"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Font->Size = 40.0f;

		//oShape->Width = fSlideWidth;
		//oShape->Height = 60.0f;
		//oShape->Left = 0.0f;
		//oShape->Top = 20.0f;//(fSlideHeight - oShape->Height) / 2.0f;
		//oShape->Reflection->Transparency = 0.5f;
		//oShape->Reflection->Size = 55.0f;
		//SleepAndRecord(1000.0);
		//oShape->Reflection->Blur = 0.5f;

	}

	{
		PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(2);
		LogInfo(TEXT("输入文档副标题"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Text = lpTexts[1];
		oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
		LogInfo(TEXT("设置标题字体"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Font->Name = TEXT("微软雅黑 ");
		LogInfo(TEXT("设置标题字体大小"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Font->Size = 20.0f;
		//oShape->Width = fSlideWidth-65*2;
		//oShape->Height = fSlideHeight*0.55;
		//oShape->Left = 65;
		//oShape->Top = fSlideHeight*0.25;
	}
	return BENCH_MARK_OK;
	});
}
void COfficePowerPointMark::FillChart(PowerPoint::ShapePtr oShape, CHAR **strs, int nrow, int ncolumn, long oLayout, std::tstring Title)
{
		 
		CStringA tstrRange;
		//Word::_ApplicationPtr app1 = wordShape->Application;
		PowerPoint::ChartPtr pptChart;
		//Word::_ApplicationPtr app2 = wordChart->Application;
		PowerPoint::ChartDataPtr pptChartData;
		Excel::_WorkbookPtr excelWorkbook;
		Excel::_WorksheetPtr excelSheets;
		Excel::RangePtr excelRange;
		Excel::ListObjectPtr excelListobj1;
		TakeMeasurement(new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_FILL_CHART + Title),
			[&]()->int {
			OfficeRegisterWindow wndOfficeReg(this);
			wndOfficeReg.RegisterWindowHandler([&]()->int {
				pptChart = oShape->Chart;
				pptChartData = pptChart->ChartData;
				excelWorkbook = (Excel::_WorkbookPtr)pptChartData->Workbook;
				excelSheets = (Excel::_WorksheetPtr)excelWorkbook->Sheets->Item[1]/*->Cells*/;//Excel::SheetsPtr
				tstrRange.Format("%c%i", (CHAR)(ncolumn + 64), nrow);;
				excelRange = excelSheets->Cells->GetRange("A1", (const char*)tstrRange);
				excelListobj1 = excelSheets->ListObjects->Item[1];
				excelListobj1->Resize(excelRange);
				return BENCH_MARK_OK;
			});
		
		tstrRange = "";
		for (Int irow = 1; irow <= nrow; irow++)
		{
			for (Int icolumn = 1; icolumn <= ncolumn; icolumn++)
			{
				tstrRange.Format("%c%i", (CHAR)(icolumn + 64), irow);
				excelRange = excelSheets->Range[(const char*)tstrRange];
				SleepAndRecord((DWORD)dwTimerNode);
				excelRange->Select();
				SleepAndRecord((DWORD)dwTimerNode);
				excelRange->Value2 = _bstr_t(*strs++);
				SleepAndRecord((DWORD)dwTimerNode);
				tstrRange = "";
				SleepAndRecord((DWORD)dwTimerNode);
			}
		}

		SleepAndRecord((DWORD)dwTimerNode1);
		excelWorkbook->Close();
		SleepAndRecord((DWORD)dwTimerNode1);

		oShape->Select(Office::MsoTriState::msoCTrue);

		oShape->Chart->ApplyLayout(oLayout);

		if (oShape->Chart->HasTitle)//title
		{
			oShape->Chart->ChartTitle->Select();
			oShape->Chart->ChartTitle->Font->Size = 14.0f;
			oShape->Chart->ChartTitle->Font->Name = "微软雅黑";
			//pptShape->Chart->ChartTitle->Font->Color = 0xF7F5EE;
			if (ncolumn > 2)
			{
				oShape->Chart->ChartTitle->Text = _bstr_t(*strs);
			}
		}
		Office::Font2Ptr oFont = oShape->Chart->Format->TextFrame2->TextRange->Font;
		oFont->Size = 12.0f;
		oFont->NameComplexScript = "微软雅黑";
		oFont->NameFarEast = "微软雅黑";
		oFont->Name = "微软雅黑";
		return BENCH_MARK_OK;
	});
	 
}
void COfficePowerPointMark::FillTable(PowerPoint::ShapePtr pptShape, CHAR **strs, int nrow, int ncolumn, std::tstring Title)
{
	TakeMeasurement( new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_FILL_TABLE + Title),
		[&]()->int {
	for (Int irow = 1; irow <= nrow; irow++)
		for (Int icolumn = 1; icolumn <= ncolumn; icolumn++)
		{
			SleepAndRecord((DWORD)dwTimerNode);
			PowerPoint::TextRangePtr oTextRange = pptShape->Table->Cell(irow, icolumn)->Shape->TextFrame->TextRange;//->Text = *strs++;
			oTextRange->Text = *strs++;
			SleepAndRecord((DWORD)dwTimerNode);
			oTextRange->Font->Size = 12.0;
			SleepAndRecord((DWORD)dwTimerNode);
			oTextRange->Font->Name = "微软雅黑";
			//PowerPoint::FontPtr
		}
	return BENCH_MARK_OK;
	});
}

PowerPoint::_SlidePtr COfficePowerPointMark::AddPage(int nPage, PowerPoint::_PresentationPtr oPresentation, std::tstring Title)
{
	LogInfo(TEXT("插入页面"));
	PowerPoint::_SlidePtr oSlide;
	TakeMeasurement( new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_ADD_PAGE + Title),
		[&]()->int {
	SleepAndRecord(dwTimerNode1);
	oSlide = oPresentation->Slides->Add(nPage, PowerPoint::PpSlideLayout::ppLayoutText);
	oSlide->Select();
	return BENCH_MARK_OK;
	});
	return oSlide;
 }

PowerPoint::ShapePtr COfficePowerPointMark::AddChart(PowerPoint::_SlidePtr oSlide, Office::XlChartType chartType,  float top, float bottom, float left, float right,  std::tstring Title)
{
	PowerPoint::ShapePtr oShape;
	TakeMeasurement(new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_ADD_CHART + Title),
		[&]()->int {
		oShape = oSlide->Shapes->AddChart(chartType, top, bottom, left, right);
		return BENCH_MARK_OK;
	});
	return oShape;
}

PowerPoint::ShapePtr COfficePowerPointMark::AddTable(PowerPoint::_SlidePtr oSlide, int nrow, int ncolumn, float top, float bottom, float left, float right, std::tstring Title)
{
	PowerPoint::ShapePtr oShape;
	TakeMeasurement( new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_ADD_TABLE + Title),
		[&]()->int {
		oShape = oSlide->Shapes->AddTable(nrow, ncolumn, top, bottom, left, right);
		return BENCH_MARK_OK;
	});
	return oShape;
}

PowerPoint::ShapePtr COfficePowerPointMark::AddPicture(PowerPoint::_SlidePtr oSlide, std::tstring lpPathFile, Office::MsoTriState state1, Office::MsoTriState state2, float top, float bottom, float left, float right, std::tstring Title)
{
	PowerPoint::ShapePtr oShape;
	TakeMeasurement(new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_ADD_PICTURE + Title),
		[&]()->int {
	oShape = oSlide->Shapes->AddPicture(lpPathFile.c_str(),
	state1,
	state2,
	top, bottom, left, right);
	return BENCH_MARK_OK;
	});
	return oShape;
}

int COfficePowerPointMark::PPTDemo2()
{
	bool _runmode = g_ApplicationPtr == NULL;
	
	Double fResult = 0.0;

	CString lpPathResc;
	MyPlatform::tstring lpPathFile;
	
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), lpPathResc.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
	lpPathResc.ReleaseBuffer();
	lpPathFile = lpPathResc + TEXT("temp.thmx");

	try
	{
		PowerPoint::_PresentationPtr oPresentation;
		Int nPage = 1;
		Float fSlideWidth, fSlideHeight; 
		TakeMeasurement(new CCounterContent(CPPT_Opids::DESC_TYPE_MARKER, CPPT_Opids::DESC_ADD_DOCUMENT + _T("temp.thmx")),
			[&]()->int {
		
		oPresentation = g_ApplicationPtr->Presentations->Add(Office::MsoTriState::msoTrue);
 		oPresentation->PageSetup->SlideSize = PowerPoint::PpSlideSizeType::ppSlideSizeOnScreen16x10;
		
		fSlideWidth = oPresentation->PageSetup->SlideWidth;
		fSlideHeight = oPresentation->PageSetup->SlideHeight;
		return BENCH_MARK_OK; });


		//1.
		{
			LogInfo(_T("PowerPoint:启动应用程序"));
			 //	lpLog->T(TEXT("PPT幻灯片编辑"), TEXT("开始计时"));
			
			PowerPoint::_SlidePtr oSlide = AddPage(nPage, oPresentation, _T("用户调查项目总结报告"));
			oSlide->Select();
			oSlide->SlideShowTransition->EntryEffect = PowerPoint::PpEntryEffect::ppEffectFadeSmoothly;
			oSlide->SlideShowTransition->PutAdvanceOnTime(Office::MsoTriState::msoTrue);
			oSlide->SlideShowTransition->AdvanceTime = 3.0f;
			oPresentation->ApplyTemplate(lpPathFile.c_str());
			//oPresentation->ApplyTheme("顶峰");
			{
				
				SleepAndRecord(dwTimerNode1);
				PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);

				//oShape->TextFrame->TextRange->Text = TEXT("用户调查项目总结报告");
				wchar_t str[12] = { 0 };
				wcscpy_s(str + 1, 11, TEXT("用户调查项目总结报告"));
				for (int i = 0;i < 11;i++)
				{
					str[i] = str[i + 1];
					str[i + 1] = 0;
					oShape->TextFrame->TextRange->Text = str;
					SleepAndRecord(dwTimerNode);
				}
				oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
				LogInfo(_T("PowerPoint:设置标题字体"));
				SleepAndRecord(dwTimerNode1);
				oShape->TextFrame->TextRange->Font->Name = TEXT("微软雅黑 ");
				LogInfo(_T("PowerPoint:设置标题字体大小"));
				SleepAndRecord(dwTimerNode1);
				oShape->TextFrame->TextRange->Font->Size = 35.0f;
				oShape->TextFrame->TextRange->Font->PutBold(Office::MsoTriState::msoTrue);

				LogInfo(_T("PowerPoint:设置副标题位置"));
				SleepAndRecord(dwTimerNode1);
				oShape->Width = fSlideWidth;
				oShape->Height = 50.0f;
				oShape->Left = 0.0f;
				oShape->Top = fSlideHeight - 30.0f * 2 - 60.0f;//(fSlideHeight - oShape->Height) / 2.0f;
															   //oShape->Reflection->Transparency = 0.5f;
															   //oShape->Reflection->Size = 55.0f;
															   //SleepAndRecord(1000.0);
															   //oShape->Reflection->Blur = 0.5f;
															   //SleepAndRecord(1000.0);
															   //LogInfo(TEXT("调整标题位置"));
															   //PowerPoint::EffectPtr oEffect = oSlide->TimeLine->MainSequence->AddEffect(oShape, PowerPoint::MsoAnimEffect::msoAnimEffectFade, PowerPoint::MsoAnimateByLevel::msoAnimateLevelNone, PowerPoint::MsoAnimTriggerType::msoAnimTriggerAfterPrevious, -1);
															   //oEffect->PutExit(Office::MsoTriState::msoTrue);
															   //oEffect->Timing->TriggerDelayTime = 3.0f;
															   //SleepAndRecord(1000.0);
			}
			//天宝银号市场部·2026年6月
			{
				PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(2);
				//oShape->TextFrame->TextRange->Text = TEXT("天宝银号市场部·2016年6月");
				wchar_t str[17] = { 0 };
				wcscpy_s(str + 1, 16, TEXT("天宝银号市场部·2016年6月"));
				for (int i = 0;i < 16;i++)
				{
					str[i] = str[i + 1];
					str[i + 1] = 0;
					oShape->TextFrame->TextRange->Text = str;
					SleepAndRecord(dwTimerNode);
				}
				oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
				oShape->TextFrame->TextRange->Font->Name = TEXT("微软雅黑 ");
				oShape->TextFrame->TextRange->Font->Size = 16.0f;
				oShape->Width = (float)(fSlideWidth*0.75);
				oShape->Height = 30.0f;
				oShape->Left = fSlideWidth / 4;
				oShape->Top = fSlideHeight - oShape->Height - 5.0f;

			}

		}

		{//2.
			LogInfo(_T("PowerPoint:插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("概述"));
			 
			CHAR *text[] = { "概述","用户调查项目概述\r\n\r\n数据分析\r\n\r\n调查结论\r\n\r\n结语" };//Diagram
			FillPage(oSlide, text, _T("概述"));
			PowerPoint::ShapePtr oShape = oSlide->Shapes->AddShape(Office::MsoAutoShapeType::msoShapePentagon, 24, 32, 350, 54);
			oShape->Select(Office::MsoTriState::msoTrue);
			oShape->PutShapeStyle(Office::MsoShapeStyleIndex::msoShapeStylePreset32);
			PowerPoint::FillFormatPtr oFormat = oShape->Fill;//= Office::MsoShapeStyleIndex::msoShapeStylePreset10
															 //PowerPoint::ColorFormatPtr oColor = oFormat->ForeColor;//->put_MSORGB(0xff0000);// = 0xff0000;
															 //oColor->SchemeColor;
			oFormat->Transparency = 0.49899f;

		}

		{//3 pic
			LogInfo(_T("PowerPoint:插入页面"));
			 
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("用户调查项目概述"));
			 
			LogInfo(_T("PowerPoint:插入文档标题、字体和位置"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);
			oShape->TextFrame->TextRange->Text = TEXT("用户调查项目概述");
			oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
			 
			oShape->TextFrame->TextRange->Font->Name = TEXT("微软雅黑 ");
			 
			oShape->TextFrame->TextRange->Font->Size = 30.0f;
			oShape->TextFrame->TextRange->Font->PutBold(Office::MsoTriState::msoTrue);
			oShape->Width = 240.0f;
			oShape->Height = 45.0f;
			oShape->Left = 460.0f;
			oShape->Top = 385.0f;//(fSlideHeight - oShape->Height) / 2.0f;

			oShape = oSlide->Shapes->Item(2);
			oShape->Width = 0;
			oShape->Height = 0;
			oShape->Left = 0;
			oShape->Top = 0;
			oShape->TextFrame->TextRange->Text = TEXT(" ");

			LogInfo(_T("PowerPoint:插入图片"));
			 
			SleepAndRecord(dwTimerNode1);
			lpPathFile.clear();
			lpPathFile = lpPathResc + TEXT("ppt-1.jpg");
			oShape = AddPicture(oSlide, 
				lpPathFile.c_str(),
				Office::MsoTriState::msoFalse,
				Office::MsoTriState::msoTrue,
				30, 90, 400, 340, _T("ppt-1.jpg"));
			 
		}

		{//4.
			LogInfo(_T("PowerPoint:插入页面"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("项目进度"));
			CHAR *text[] = { "项目进度","项目名称：天宝银号市场部2026年国内目标用户认知度和满意度调查项目\r\n项目目标：通过抽样调查的方法，获取国内主要省市目标用户对金蚁产品的认知度和满意度的基本情况，为公司产品研发、市场宣传和商业运作提供较为客观、准确的数据基础。\r\n开始时间：2026年1月1日\r\n结束时间：2026年5月25日" };
			FillPage(oSlide, text, _T("项目进度"));

		}

		{//5.
			LogInfo(_T("PowerPoint:插入页面"));
			 
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("项目组成员"));
			 
			CHAR *text[] = { "项目组成员","项目责任人：刘廿\r\n调查问卷设计：欧华\r\n调查样本选择：逾越中、余永勇\r\n用户采访和问卷发放：贺赫赫、伍妤娥\r\n问卷回收和结果统计：胡焦忠\r\n数据分析和报告：屈建俥、狄忠海" };
			FillPage(oSlide, text, _T("项目组成员"));
		}

		 
		{//9.
			LogInfo(TEXT("插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("问卷发放和回收"));
			 
			CHAR *text[] = { "问卷发放和回收","此次调查中使用的问卷发放和回收方式包括：\r\n	上门采访方式发放和回收问卷\r\n	街头采访方式发放和回收问卷\r\n	邮寄方式发放和回收问卷\r\n	电子邮件方式发放和回收问卷\r\n	网上调查方式发放和回收问卷\r\n天宝银号在21个省、直辖市的分公司或办事处市场负责人组织本单位员工积极参与或配合了此次问卷发放和回收工作。在此表示感谢。" };
			FillPage(oSlide, text, _T("问卷发放和回收"));
		}

		{//11. pic
			LogInfo(TEXT("插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("数据分析"));
			 
			LogInfo(_T("PowerPoint:插入文档标题、字体和位置"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);
			oShape->TextFrame->TextRange->Text = TEXT("数据分析");
			oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
			LogInfo(_T("PowerPoint:设置标题字体"));
			 
			oShape->TextFrame->TextRange->Font->Name = TEXT("微软雅黑 ");
			LogInfo(TEXT("设置标题字体大小"));
			oShape->TextFrame->TextRange->Font->Size = 30.0f;
			oShape->TextFrame->TextRange->Font->PutBold(Office::MsoTriState::msoTrue);
			oShape->Width = 120.0f;
			oShape->Height = 45.0f;
			oShape->Left = 580.0f;
			oShape->Top = 385.0f;//(fSlideHeight - oShape->Height) / 2.0f;

			oShape = oSlide->Shapes->Item(2);
			oShape->Width = 0;
			oShape->Height = 0;
			oShape->Left = 0;
			oShape->Top = 0;
			oShape->TextFrame->TextRange->Text = TEXT(" ");

			LogInfo(_T("PowerPoint:插入图片"));
			 
			SleepAndRecord(dwTimerNode1);
			lpPathFile.clear();
			lpPathFile = lpPathResc + TEXT("ppt-2.jpg");
			oShape = AddPicture(oSlide,
				lpPathFile.c_str(),
				Office::MsoTriState::msoFalse,
				Office::MsoTriState::msoTrue,
				30, 90, 530, 340, _T("ppt-2.jpg"));
		}

		{//12.table
			LogInfo(_T("PowerPoint:插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("用户基本情况"));
			 
			CHAR *text1[] = { "用户基本情况","回收有效问卷共计100000份，分别代表3个目标用户或潜在用户个体。这些用户的年龄分布情况如" };
			FillPage(oSlide, text1, _T("用户基本情况"));

			LogInfo(_T("PowerPoint:插入图表"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape1 = AddTable(oSlide, 2, 4, 35, 190, 300, 70, _T("年龄"));
			oSlide->Select();
			CHAR *text2[] = { "年龄",	"18-31",	"30-51",	"51以上",
				"份数",	"14700",	"60000",	"25300" };
			FillTable(oShape1, text2, 2, 4, _T("年龄"));

			PowerPoint::ShapePtr oShape2 = AddTable(oSlide, 2, 4, 350, 190, 300, 70, _T("区域"));
			oSlide->Select();
			CHAR *text3[] = { "区域",	"大型城市",	"中型城市",	"乡镇",
				"份数",	"54300",	"26700",	"19000" };
			FillTable(oShape2, text3, 2, 4, _T("区域"));

		}

		{//13. chart
			LogInfo(_T("PowerPoint:插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("受调查对象的年龄分布情况"));
			 
			CHAR *text[] = { "受调查对象的年龄分布情况"," " };
			FillPage(oSlide, text, _T("受调查对象的年龄分布情况"));

			LogInfo(_T("PowerPoint:插入图表"));
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape =  AddChart(oSlide, Office::XlChartType::xlPie, 35, 110, 300, 300, _T("受调查对象的年龄分布情况"));
			CHAR *text2[] = { " ","受调查对象的年龄分布情况","18-30","14700","30-50","60000","50以上","25300" };
			FillChart(oShape, text2, 4, 2, 6, _T("受调查对象的年龄分布情况"));
		}

		{//15. table
			LogInfo(TEXT("插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("对产品的了解度:Table"));
			 
			CHAR *text1[] = { "对产品的了解度:","受调查用户产品的了解程度" };
			FillPage(oSlide, text1, _T("对产品的了解度:Table"));

			LogInfo(_T("PowerPoint:插入表格"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape1 = AddTable(oSlide, 7, 5, 135, 150, 390, 260, _T("对产品的了解度"));
			oSlide->Select();
			CHAR *text2[] = { "","不知道","知道不了解","了解","非常了解",
				"18-30","4675","2675","5675","1675",
				"30-50","15000",	"20000",	"20000",	"5000",
				"50以上",	"6325",	"8325",	"7325",	"3325",
				"乡镇",	"4750",	"5750",	"6750",	"3750",
				"中型城市",	"6675",	"7675",	"8675",	"3675",
				"大型城市",	"13575",	"15575",	"16575",	"8575" };
			FillTable(oShape1, text2, 7, 5, _T("对产品的了解度"));
		}

		{//16. chart
			LogInfo(_T("PowerPoint:插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("对产品的了解度:Chart"));
			 
			CHAR *text[] = { "对产品的了解度","受调查用户产品的了解程度" };
			FillPage(oSlide, text, _T("对产品的了解度:Chart"));
			LogInfo(TEXT("插入图表"));
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape = AddChart(oSlide, Office::XlChartType::xlLine, 16, 145, 540, 300, _T("对产品的了解度"));
			CHAR *text2[] = { " ","用户对产品了解度","不知道","26000","不了解","31000","了解","33000","非常了解","10000" };
			FillChart(oShape, text2, 4, 2, 9, _T("对产品的了解度"));
			oShape->Select(Office::MsoTriState::msoTrue);
			oShape->PutShapeStyle(Office::MsoShapeStyleIndex::msoShapeStylePreset20);
			PowerPoint::FillFormatPtr oFormat = oShape->Fill;
			oFormat->Transparency = 0.8099f;

		}
		 
		{//24.
			LogInfo(_T("PowerPoint:插入页面"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("结语"));
			 
			CHAR *text[] = { "结语","无论是从项目运作过程还是从项目结果看来，天宝银号市场部2026年国内目标用户认知度和满意度调查项目都取得了预期的成果，此项目获得的数据以及结论可以为公司决策层提供依据，可以为公司的产品研发、市场宣传和商业运作提供帮助，项目目标基本实现。\r\n在今后的类似项目中，还有以下几个方面需要改进：\r\n	（此处从略）\r\n	……\r\n此项目为市场部积累了宝贵的经验，参与项目的员工接受了很好的锻炼。相信在以后的类似项目中，市场部还会取得更好的成绩。" };
			FillPage(oSlide, text, _T("结语"));
		}

 		MyPlatform::tstring savename = lpPathResc + TEXT("产品调查报告_temp.ppt");
 		DeleteFile(savename.c_str());
		savename += _T(".pdf");
 		//oPresentation->SaveAs(savename.c_str(), PowerPoint::PpSaveAsFileType::ppSaveAsPresentation, Office::MsoTriState::msoTrue);
		//add save pdf file format.  2019-11-07
		//oPresentation->SaveAs(savename.c_str(), PowerPoint::PpSaveAsFileType::ppSaveAsPDF, Office::MsoTriState::msoTrue);
		
		/* do not need constant run time
		oPresentation->SlideShowSettings->Run();

		for (int nIdx = 0; nIdx < 3; nIdx++)
		{
			try
			{
				PowerPoint::SlideShowWindowsPtr oSlideShowWindows = g_ApplicationPtr->SlideShowWindows;
				SleepAndRecord(dwTimerNode1);
				while (oSlideShowWindows->GetCount() >= 1 && oSlideShowWindows->Item(1)->View->State != PowerPoint::PpSlideShowState::ppSlideShowDone) {
					SleepAndRecord(dwTimerNode1);
				};
				SleepAndRecord(dwTimerNode3);
				oPresentation->SlideShowWindow->View->Exit();
				break;
			}
			catch (_com_error &err)
			{
				SleepAndRecord(dwTimerNode1);
				if (2 == nIdx)
				{
					throw err;
				}
			}

		}
		*/
		// close
		SleepAndRecord(dwTimerNode3 * 2);
		oPresentation->Close();
		g_ApplicationPtr->Quit();

		return BENCH_MARK_OK;
	}
	catch (_com_error &err)
	{
		LogError({ _T("执行出现错误，错误原因"), err.ErrorMessage() });
		return -1;
	}
}


