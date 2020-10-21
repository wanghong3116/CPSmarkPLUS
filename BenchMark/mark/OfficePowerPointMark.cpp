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

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("PowerPoint-�칫���ܲ���"));
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
		LogError({ _T("Office PPT��PPT��������ʧ�ܣ�����û����ȷ��װ"), (const TCHAR*)err.ErrorMessage() });
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
		LogInfo(TEXT("�����ĵ�����"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Text = lpTexts[0];
		oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
		LogInfo(TEXT("���ñ�������"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Font->Name = TEXT("΢���ź� ");
		LogInfo(TEXT("���ñ��������С"));
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
		LogInfo(TEXT("�����ĵ�������"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Text = lpTexts[1];
		oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
		LogInfo(TEXT("���ñ�������"));
		SleepAndRecord((DWORD)dwTimerNode1);
		oShape->TextFrame->TextRange->Font->Name = TEXT("΢���ź� ");
		LogInfo(TEXT("���ñ��������С"));
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
			oShape->Chart->ChartTitle->Font->Name = "΢���ź�";
			//pptShape->Chart->ChartTitle->Font->Color = 0xF7F5EE;
			if (ncolumn > 2)
			{
				oShape->Chart->ChartTitle->Text = _bstr_t(*strs);
			}
		}
		Office::Font2Ptr oFont = oShape->Chart->Format->TextFrame2->TextRange->Font;
		oFont->Size = 12.0f;
		oFont->NameComplexScript = "΢���ź�";
		oFont->NameFarEast = "΢���ź�";
		oFont->Name = "΢���ź�";
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
			oTextRange->Font->Name = "΢���ź�";
			//PowerPoint::FontPtr
		}
	return BENCH_MARK_OK;
	});
}

PowerPoint::_SlidePtr COfficePowerPointMark::AddPage(int nPage, PowerPoint::_PresentationPtr oPresentation, std::tstring Title)
{
	LogInfo(TEXT("����ҳ��"));
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
			LogInfo(_T("PowerPoint:����Ӧ�ó���"));
			 //	lpLog->T(TEXT("PPT�õ�Ƭ�༭"), TEXT("��ʼ��ʱ"));
			
			PowerPoint::_SlidePtr oSlide = AddPage(nPage, oPresentation, _T("�û�������Ŀ�ܽᱨ��"));
			oSlide->Select();
			oSlide->SlideShowTransition->EntryEffect = PowerPoint::PpEntryEffect::ppEffectFadeSmoothly;
			oSlide->SlideShowTransition->PutAdvanceOnTime(Office::MsoTriState::msoTrue);
			oSlide->SlideShowTransition->AdvanceTime = 3.0f;
			oPresentation->ApplyTemplate(lpPathFile.c_str());
			//oPresentation->ApplyTheme("����");
			{
				
				SleepAndRecord(dwTimerNode1);
				PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);

				//oShape->TextFrame->TextRange->Text = TEXT("�û�������Ŀ�ܽᱨ��");
				wchar_t str[12] = { 0 };
				wcscpy_s(str + 1, 11, TEXT("�û�������Ŀ�ܽᱨ��"));
				for (int i = 0;i < 11;i++)
				{
					str[i] = str[i + 1];
					str[i + 1] = 0;
					oShape->TextFrame->TextRange->Text = str;
					SleepAndRecord(dwTimerNode);
				}
				oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
				LogInfo(_T("PowerPoint:���ñ�������"));
				SleepAndRecord(dwTimerNode1);
				oShape->TextFrame->TextRange->Font->Name = TEXT("΢���ź� ");
				LogInfo(_T("PowerPoint:���ñ��������С"));
				SleepAndRecord(dwTimerNode1);
				oShape->TextFrame->TextRange->Font->Size = 35.0f;
				oShape->TextFrame->TextRange->Font->PutBold(Office::MsoTriState::msoTrue);

				LogInfo(_T("PowerPoint:���ø�����λ��"));
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
															   //LogInfo(TEXT("��������λ��"));
															   //PowerPoint::EffectPtr oEffect = oSlide->TimeLine->MainSequence->AddEffect(oShape, PowerPoint::MsoAnimEffect::msoAnimEffectFade, PowerPoint::MsoAnimateByLevel::msoAnimateLevelNone, PowerPoint::MsoAnimTriggerType::msoAnimTriggerAfterPrevious, -1);
															   //oEffect->PutExit(Office::MsoTriState::msoTrue);
															   //oEffect->Timing->TriggerDelayTime = 3.0f;
															   //SleepAndRecord(1000.0);
			}
			//�챦�����г�����2026��6��
			{
				PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(2);
				//oShape->TextFrame->TextRange->Text = TEXT("�챦�����г�����2016��6��");
				wchar_t str[17] = { 0 };
				wcscpy_s(str + 1, 16, TEXT("�챦�����г�����2016��6��"));
				for (int i = 0;i < 16;i++)
				{
					str[i] = str[i + 1];
					str[i + 1] = 0;
					oShape->TextFrame->TextRange->Text = str;
					SleepAndRecord(dwTimerNode);
				}
				oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
				oShape->TextFrame->TextRange->Font->Name = TEXT("΢���ź� ");
				oShape->TextFrame->TextRange->Font->Size = 16.0f;
				oShape->Width = (float)(fSlideWidth*0.75);
				oShape->Height = 30.0f;
				oShape->Left = fSlideWidth / 4;
				oShape->Top = fSlideHeight - oShape->Height - 5.0f;

			}

		}

		{//2.
			LogInfo(_T("PowerPoint:����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("����"));
			 
			CHAR *text[] = { "����","�û�������Ŀ����\r\n\r\n���ݷ���\r\n\r\n�������\r\n\r\n����" };//Diagram
			FillPage(oSlide, text, _T("����"));
			PowerPoint::ShapePtr oShape = oSlide->Shapes->AddShape(Office::MsoAutoShapeType::msoShapePentagon, 24, 32, 350, 54);
			oShape->Select(Office::MsoTriState::msoTrue);
			oShape->PutShapeStyle(Office::MsoShapeStyleIndex::msoShapeStylePreset32);
			PowerPoint::FillFormatPtr oFormat = oShape->Fill;//= Office::MsoShapeStyleIndex::msoShapeStylePreset10
															 //PowerPoint::ColorFormatPtr oColor = oFormat->ForeColor;//->put_MSORGB(0xff0000);// = 0xff0000;
															 //oColor->SchemeColor;
			oFormat->Transparency = 0.49899f;

		}

		{//3 pic
			LogInfo(_T("PowerPoint:����ҳ��"));
			 
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("�û�������Ŀ����"));
			 
			LogInfo(_T("PowerPoint:�����ĵ����⡢�����λ��"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);
			oShape->TextFrame->TextRange->Text = TEXT("�û�������Ŀ����");
			oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
			 
			oShape->TextFrame->TextRange->Font->Name = TEXT("΢���ź� ");
			 
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

			LogInfo(_T("PowerPoint:����ͼƬ"));
			 
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
			LogInfo(_T("PowerPoint:����ҳ��"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("��Ŀ����"));
			CHAR *text[] = { "��Ŀ����","��Ŀ���ƣ��챦�����г���2026�����Ŀ���û���֪�Ⱥ�����ȵ�����Ŀ\r\n��ĿĿ�꣺ͨ����������ķ�������ȡ������Ҫʡ��Ŀ���û��Խ��ϲ�Ʒ����֪�Ⱥ�����ȵĻ��������Ϊ��˾��Ʒ�з����г���������ҵ�����ṩ��Ϊ�͹ۡ�׼ȷ�����ݻ�����\r\n��ʼʱ�䣺2026��1��1��\r\n����ʱ�䣺2026��5��25��" };
			FillPage(oSlide, text, _T("��Ŀ����"));

		}

		{//5.
			LogInfo(_T("PowerPoint:����ҳ��"));
			 
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("��Ŀ���Ա"));
			 
			CHAR *text[] = { "��Ŀ���Ա","��Ŀ�����ˣ���إ\r\n�����ʾ���ƣ�ŷ��\r\n��������ѡ����Խ�С�������\r\n�û��ɷú��ʾ��ţ��غպա���楶�\r\n�ʾ���պͽ��ͳ�ƣ�������\r\n���ݷ����ͱ��棺�����e�����Һ�" };
			FillPage(oSlide, text, _T("��Ŀ���Ա"));
		}

		 
		{//9.
			LogInfo(TEXT("����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("�ʾ��źͻ���"));
			 
			CHAR *text[] = { "�ʾ��źͻ���","�˴ε�����ʹ�õ��ʾ��źͻ��շ�ʽ������\r\n	���Ųɷ÷�ʽ���źͻ����ʾ�\r\n	��ͷ�ɷ÷�ʽ���źͻ����ʾ�\r\n	�ʼķ�ʽ���źͻ����ʾ�\r\n	�����ʼ���ʽ���źͻ����ʾ�\r\n	���ϵ��鷽ʽ���źͻ����ʾ�\r\n�챦������21��ʡ��ֱϽ�еķֹ�˾����´��г���������֯����λԱ���������������˴˴��ʾ��źͻ��չ������ڴ˱�ʾ��л��" };
			FillPage(oSlide, text, _T("�ʾ��źͻ���"));
		}

		{//11. pic
			LogInfo(TEXT("����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("���ݷ���"));
			 
			LogInfo(_T("PowerPoint:�����ĵ����⡢�����λ��"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape = oSlide->Shapes->Item(1);
			oShape->TextFrame->TextRange->Text = TEXT("���ݷ���");
			oShape->TextFrame->TextRange->Font->Color->PutVisioRGB(0xffffff);
			LogInfo(_T("PowerPoint:���ñ�������"));
			 
			oShape->TextFrame->TextRange->Font->Name = TEXT("΢���ź� ");
			LogInfo(TEXT("���ñ��������С"));
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

			LogInfo(_T("PowerPoint:����ͼƬ"));
			 
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
			LogInfo(_T("PowerPoint:����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("�û��������"));
			 
			CHAR *text1[] = { "�û��������","������Ч�ʾ���100000�ݣ��ֱ����3��Ŀ���û���Ǳ���û����塣��Щ�û�������ֲ������" };
			FillPage(oSlide, text1, _T("�û��������"));

			LogInfo(_T("PowerPoint:����ͼ��"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape1 = AddTable(oSlide, 2, 4, 35, 190, 300, 70, _T("����"));
			oSlide->Select();
			CHAR *text2[] = { "����",	"18-31",	"30-51",	"51����",
				"����",	"14700",	"60000",	"25300" };
			FillTable(oShape1, text2, 2, 4, _T("����"));

			PowerPoint::ShapePtr oShape2 = AddTable(oSlide, 2, 4, 350, 190, 300, 70, _T("����"));
			oSlide->Select();
			CHAR *text3[] = { "����",	"���ͳ���",	"���ͳ���",	"����",
				"����",	"54300",	"26700",	"19000" };
			FillTable(oShape2, text3, 2, 4, _T("����"));

		}

		{//13. chart
			LogInfo(_T("PowerPoint:����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("�ܵ�����������ֲ����"));
			 
			CHAR *text[] = { "�ܵ�����������ֲ����"," " };
			FillPage(oSlide, text, _T("�ܵ�����������ֲ����"));

			LogInfo(_T("PowerPoint:����ͼ��"));
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape =  AddChart(oSlide, Office::XlChartType::xlPie, 35, 110, 300, 300, _T("�ܵ�����������ֲ����"));
			CHAR *text2[] = { " ","�ܵ�����������ֲ����","18-30","14700","30-50","60000","50����","25300" };
			FillChart(oShape, text2, 4, 2, 6, _T("�ܵ�����������ֲ����"));
		}

		{//15. table
			LogInfo(TEXT("����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("�Բ�Ʒ���˽��:Table"));
			 
			CHAR *text1[] = { "�Բ�Ʒ���˽��:","�ܵ����û���Ʒ���˽�̶�" };
			FillPage(oSlide, text1, _T("�Բ�Ʒ���˽��:Table"));

			LogInfo(_T("PowerPoint:������"));
			 
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape1 = AddTable(oSlide, 7, 5, 135, 150, 390, 260, _T("�Բ�Ʒ���˽��"));
			oSlide->Select();
			CHAR *text2[] = { "","��֪��","֪�����˽�","�˽�","�ǳ��˽�",
				"18-30","4675","2675","5675","1675",
				"30-50","15000",	"20000",	"20000",	"5000",
				"50����",	"6325",	"8325",	"7325",	"3325",
				"����",	"4750",	"5750",	"6750",	"3750",
				"���ͳ���",	"6675",	"7675",	"8675",	"3675",
				"���ͳ���",	"13575",	"15575",	"16575",	"8575" };
			FillTable(oShape1, text2, 7, 5, _T("�Բ�Ʒ���˽��"));
		}

		{//16. chart
			LogInfo(_T("PowerPoint:����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("�Բ�Ʒ���˽��:Chart"));
			 
			CHAR *text[] = { "�Բ�Ʒ���˽��","�ܵ����û���Ʒ���˽�̶�" };
			FillPage(oSlide, text, _T("�Բ�Ʒ���˽��:Chart"));
			LogInfo(TEXT("����ͼ��"));
			SleepAndRecord(dwTimerNode1);
			PowerPoint::ShapePtr oShape = AddChart(oSlide, Office::XlChartType::xlLine, 16, 145, 540, 300, _T("�Բ�Ʒ���˽��"));
			CHAR *text2[] = { " ","�û��Բ�Ʒ�˽��","��֪��","26000","���˽�","31000","�˽�","33000","�ǳ��˽�","10000" };
			FillChart(oShape, text2, 4, 2, 9, _T("�Բ�Ʒ���˽��"));
			oShape->Select(Office::MsoTriState::msoTrue);
			oShape->PutShapeStyle(Office::MsoShapeStyleIndex::msoShapeStylePreset20);
			PowerPoint::FillFormatPtr oFormat = oShape->Fill;
			oFormat->Transparency = 0.8099f;

		}
		 
		{//24.
			LogInfo(_T("PowerPoint:����ҳ��"));
			PowerPoint::_SlidePtr oSlide = AddPage(++nPage, oPresentation, _T("����"));
			 
			CHAR *text[] = { "����","�����Ǵ���Ŀ�������̻��Ǵ���Ŀ����������챦�����г���2026�����Ŀ���û���֪�Ⱥ�����ȵ�����Ŀ��ȡ����Ԥ�ڵĳɹ�������Ŀ��õ������Լ����ۿ���Ϊ��˾���߲��ṩ���ݣ�����Ϊ��˾�Ĳ�Ʒ�з����г���������ҵ�����ṩ��������ĿĿ�����ʵ�֡�\r\n�ڽ���������Ŀ�У��������¼���������Ҫ�Ľ���\r\n	���˴����ԣ�\r\n	����\r\n����ĿΪ�г��������˱���ľ��飬������Ŀ��Ա�������˺ܺõĶ������������Ժ��������Ŀ�У��г�������ȡ�ø��õĳɼ���" };
			FillPage(oSlide, text, _T("����"));
		}

 		MyPlatform::tstring savename = lpPathResc + TEXT("��Ʒ���鱨��_temp.ppt");
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
		LogError({ _T("ִ�г��ִ��󣬴���ԭ��"), err.ErrorMessage() });
		return -1;
	}
}


