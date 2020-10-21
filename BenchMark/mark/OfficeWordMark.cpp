#include "stdafx.h"
#include "MarkObject.h"
#include "OfficeWordMark.h"
#include "OfficeRegisterWindow.h"

using namespace MyPlatform;

COfficeWordMark::COfficeWordMark()
{
	LogInfo(_T("Invoke COfficeWordMark().."));

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Word-办公性能测试"));
	m_eMarkType = E_MARK_OFFICE_WORD;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	SetMD5PassWd(std::string("e32ddbf53aac7513a2b84acd46e565e5")); //latest md5
	//SetMD5PassWd(std::string("ea32cfd3523f4c04f7d398cca8b0f761"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }
 
COfficeWordMark::~COfficeWordMark()
{
	LogInfo(_T("Invoke ~COfficeWordMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}
int COfficeWordMark::init()
{
	//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
	//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	SleepAndRecord(1000 * 1);

	CoInitialize(NULL);

	try
	{
	    Word::_ApplicationPtr oApp;

		MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), m_strRescFP.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
		m_strRescFP.ReleaseBuffer();
		m_strDocFP = m_strRescFP + TEXT("temp1.docx");

		if (0 != _taccess(m_strDocFP, 0))
		{
			LogError({ _T("Office Word资源文件不存在！") });
			return -1;
		}
			TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_OPEN_APPLICATION),
				[&]()->int {
				OfficeRegisterWindow wndOfficeReg(this);
				wndOfficeReg.RegisterWindowHandler([&]()->int {
					HRESULT hr = oApp.CreateInstance(__uuidof(Word::Application));
				if (FAILED(hr)) {
					LogError({ _T("Office Word没有安装！") });
					return -2;
				}

				m_pWordApp.Attach(oApp.Detach());
				m_pWordApp->Visible = VARIANT_TRUE;
				m_pWordApp->WindowState = Word::WdWindowState::wdWindowStateMaximize;
				while (true)
				{
					try
					{
						m_pWordApp->Documents->Add();
						SleepAndRecord(dwTimerNode1);

						m_pWordApp->Documents->Open(&(_variant_t((const TCHAR*)m_strDocFP)));
					}
					catch (_com_error&)
					{
						SleepAndRecord(1);
						continue;
					}

					m_pWordApp->Documents->Close(&_variant_t((long)Word::WdSaveOptions::wdDoNotSaveChanges));

					break;

				}		
				return BENCH_MARK_OK; 
				});

				return BENCH_MARK_OK; });
	
		
	}
	catch (_com_error &err)
	{
		LogError({ _T("Word出现异常，异常原因"), err.ErrorMessage() });
		//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
		return -1;
	};

	return BENCH_MARK_OK;
}

int COfficeWordMark::run()
{
	int nRet = BENCH_MARK_OK;
	m_nRunTime = MyPlatform::GetTimeStampMillisecond();


	nRet = WordDemo2();
	return nRet;
}

int COfficeWordMark::destroy()
{
	LogInfo(_T("Invoke COfficeWordMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
	//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));

	m_nRunTime = MyPlatform::GetTimeStampMillisecond() - m_nRunTime;
	return BENCH_MARK_OK;
}
 
Word::RangePtr COfficeWordMark::FindText(Word::_DocumentPtr lpDoc, const TCHAR* lpText, Word::RangePtr lpLastRange) {
	Word::RangePtr oRangeDoc = lpDoc->Range();
	Word::RangePtr oRangeFind = oRangeDoc->Duplicate;
	if (lpLastRange != 0) {
		oRangeFind->Start = lpLastRange->End;
	}
	oRangeFind->Find->ClearFormatting();
	oRangeFind->Find->Execute(
		&(_variant_t(lpText))
	);
	return oRangeFind;
};

MyPlatform::tstring COfficeWordMark::FileRead(const TCHAR* lpFilePath) {
	FILE *f;
	_tfopen_s(&f, lpFilePath, TEXT("rb"));
	fseek(f, 0, SEEK_END);
	int flen = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<unsigned char> fcont(flen + 2);
	flen = fread(fcont.data(), 1, flen, f);
	fclose(f);
	TCHAR *lpTString = (TCHAR *)fcont.data();
	fcont[flen] = 0;
	fcont[flen + 1] = 0;
	MyPlatform::tstring stdTString = MyPlatform::tstring((MyPlatform::tstring::const_pointer)fcont.data());
	return stdTString;
};
 
void COfficeWordMark::InsertAfter(Word::RangePtr oRange, MyPlatform::tstring tContent, float ms)
{
	const TCHAR* lpctContent = tContent.c_str();
	wchar_t lpcwContent[2] = { 0 };
	if (ms > 0) {
		for (auto i = tContent.begin(); i != tContent.end(); i++) {
			if (i != tContent.begin()) SleepAndRecord((DWORD)ms);
			lpcwContent[0] = *i;
			oRange->InsertAfter(_bstr_t(lpcwContent));
		};
	}
	else {
		oRange->InsertAfter(_bstr_t(lpctContent));
	};
}

void COfficeWordMark::Set1stLevelTitleForm(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring Title, std::tstring fontname)
{ 
	TakeMeasurement( new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM + Title),
		[&]()->int {
	SleepAndRecord(dwTimerNode2);
	oRangeResult = FindText(
		oDoc,
		Title.c_str(),
		oRangeResult);
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Select();
	SleepAndRecord(dwTimerNode1);
	m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
	SleepAndRecord(dwTimerNode1);
	oRangeResult->PutStyle(&(_variant_t(Word::WdBuiltinStyle::wdStyleHeading1)));
	m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphLeft;

	LogInfo(TEXT("设置字号"));
	SleepAndRecord(dwTimerNode2);
	oRangeResult->Font->Size = 16.0f;

	LogInfo(TEXT("设置字体"));
	SleepAndRecord(dwTimerNode2);
	oRangeResult->Font->Name = fontname.c_str();

	SleepAndRecord(dwTimerNode2);
	return BENCH_MARK_OK;
	});
}

void COfficeWordMark::Set2ndLevelTitleForm(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring Title, std::tstring fontname)
{ 
	TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM + Title),
		[&]()->int {
	SleepAndRecord(dwTimerNode2);
	oRangeResult = FindText(
		oDoc,
		Title.c_str(),
		oRangeResult);
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Select();
	SleepAndRecord(dwTimerNode1);
	m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
	SleepAndRecord(dwTimerNode1);
	oRangeResult->PutStyle(&(_variant_t(Word::WdBuiltinStyle::wdStyleHeading2)));
	m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphLeft;

	LogInfo(TEXT("设置字号"));
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Font->Size = 13.0f;

	LogInfo(TEXT("设置字体"));
	SleepAndRecord(dwTimerNode2);
	oRangeResult->Font->Name = fontname.c_str();
	return BENCH_MARK_OK;
	});
}

//Set2ndLevelTitleForm(oRangeResult, m_pWordApp, _T("2.2 产品认知度"), _T("楷体"));
void COfficeWordMark::Set3rdLevelTitleForm(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring Title, std::tstring fontname)
{ 
	TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM + Title),
		[&]()->int {
	SleepAndRecord(dwTimerNode2);
	oRangeResult = FindText(
		oDoc,
		Title.c_str(),
		oRangeResult);
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Select();
	SleepAndRecord(dwTimerNode1);
	m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
	SleepAndRecord(dwTimerNode1);
	oRangeResult->PutStyle(&(_variant_t(Word::WdBuiltinStyle::wdStyleHeading3)));
	m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphLeft;

	LogInfo(TEXT("设置字号"));
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Font->Size = 11.0f;

	LogInfo(TEXT("设置字体"));
	SleepAndRecord(dwTimerNode2);
	oRangeResult->Font->Name = fontname.c_str();
	return BENCH_MARK_OK;
	});
}

Word::InlineShapePtr COfficeWordMark::AddChart(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, Word::InlineShapePtr wordShape, std::tstring chartname, Office::XlChartType type)
{
	TakeMeasurement( new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_ADD_CHART + chartname),
		[&]()->int {
	SleepAndRecord(dwTimerNode2);
	oRangeResult = FindText(
		oDoc,
		chartname.c_str(),
		oRangeResult);
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Select();
	SleepAndRecord(dwTimerNode1);
	m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
	oRangeResult->Delete();
	SleepAndRecord(dwTimerNode1);
	wordShape = oDoc->InlineShapes->AddChart(type, &(_variant_t((long)DISP_E_PARAMNOTFOUND, VT_ERROR)));
	return BENCH_MARK_OK;
	});
	return wordShape;
}

Word::SelectionPtr COfficeWordMark::AddTable(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, Word::TablesPtr wordTables, Word::TablePtr wordTable, Word::SelectionPtr wordSelection, std::tstring tablename, int nrow, int ncolumn)
{ 
	TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_ADD_TABLE + tablename),
		[&]()->int {

	SleepAndRecord(dwTimerNode2);
	oRangeResult = FindText(
		oDoc,
		tablename.c_str(),
		oRangeResult);
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Select();
	SleepAndRecord(dwTimerNode1);
	m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
	oRangeResult->Delete();
	SleepAndRecord(dwTimerNode1);

	wordTables = oDoc->Tables;
	wordTable = wordTables->Add(oRangeResult, nrow, ncolumn, &_variant_t((short)Word::WdDefaultTableBehavior::wdWord9TableBehavior), &_variant_t((short)Word::WdAutoFitBehavior::wdAutoFitFixed));
	wordSelection = m_pWordApp->Selection;

	return BENCH_MARK_OK;
	});
	return wordSelection;
}
void COfficeWordMark::FillTable(Word::SelectionPtr pwordSelection, CHAR **strs, int nrow, int ncolumn, std::tstring op_name)
{ 
	TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_FILL_TABLE + op_name),
		[&]()->int {

	for (int irow = 0; irow < nrow; irow++)
	{
		for (int icolumn = 0; icolumn < ncolumn; icolumn++)
		{
			SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
			pwordSelection->TypeText(_bstr_t(*strs++));
			pwordSelection->MoveRight(&_variant_t((short)Word::WdUnits::wdCharacter), &_variant_t((short)1), &_variant_t((short)0));
		}
		if (irow < nrow - 1)
		{
			pwordSelection->MoveDown(&_variant_t((short)Word::WdUnits::wdLine), &_variant_t((short)1), &_variant_t((short)0)); //向下移动鼠标到下一行
			pwordSelection->MoveLeft(&_variant_t((short)Word::WdUnits::wdCharacter), &_variant_t((short)ncolumn - 1), &_variant_t((short)0));//向左移动鼠标到第1列
		}
	}
	return BENCH_MARK_OK;
	});
}

Void COfficeWordMark::FillChart(Word::InlineShapePtr wordShape, CHAR **strs, int nrow, int ncolumn, std::tstring op_name)
{ 
	SleepAndRecord(dwTimerNode2);
	 
	CStringA tstrRange;
	//Word::_ApplicationPtr app1 = wordShape->Application;
	Word::ChartPtr wordChart;
	//Word::_ApplicationPtr app2 = wordChart->Application;
	Word::ChartDataPtr wordChartData;
	Excel::_WorksheetPtr excelSheets;
	Excel::_WorkbookPtr excelWorkbook;
	Excel::RangePtr excelRange;
	Excel::ListObjectPtr excelListobj1;
	 TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_FILL_CHART + op_name),
		[&]()->int {
		 OfficeRegisterWindow wndOfficeReg(this);
		 wndOfficeReg.RegisterWindowHandler([&]()->int {
			 wordChart = wordShape->Chart;
			 //Word::_ApplicationPtr app2 = wordChart->Application;
			 wordChartData = wordChart->ChartData;
			 excelWorkbook = (Excel::_WorkbookPtr)wordChartData->Workbook;
			 excelSheets = (Excel::_WorksheetPtr)excelWorkbook->Sheets->Item[1]/*->Cells*/;//Excel::SheetsPtr
			 tstrRange.Format("%c%i", (CHAR)(ncolumn + 64), nrow);;
			 excelRange = excelSheets->Cells->GetRange("A1", (const char*)tstrRange);
			 excelListobj1 = excelSheets->ListObjects->Item[1];
			 excelListobj1->Resize(excelRange);
			 return BENCH_MARK_OK;
		 });
	tstrRange = "";
	for (int irow = 1; irow <= nrow; irow++)
	{
		for (int icolumn = 1; icolumn <= ncolumn; icolumn++)
		{
			SleepAndRecord((DWORD)CMarkObject::dwTimerNode);
			tstrRange.Format("%c%i", (CHAR)(icolumn + 64), irow);
			excelRange = excelSheets->Range[(const char*)tstrRange];
			SleepAndRecord((DWORD)CMarkObject::dwTimerNode);
			excelRange->Select();
			SleepAndRecord((DWORD)CMarkObject::dwTimerNode);
			excelRange->Value2 = _bstr_t(*strs++);
			SleepAndRecord((DWORD)CMarkObject::dwTimerNode);
			tstrRange = "";
			SleepAndRecord((DWORD)CMarkObject::dwTimerNode);
		}
	}

	SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
	excelWorkbook->Close();
	SleepAndRecord((DWORD)CMarkObject::dwTimerNode);

	wordShape->Select();
	wordShape->Range->Font->Size = 12;
	wordShape->Range->Font->Name = "华文行楷";

	if (wordChart->HasTitle)
	{
		wordChart->ChartTitle->Font->Italic = true;
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
		wordChart->ChartTitle->Font->Size = 12;
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
		wordChart->ChartTitle->Font->Color = RGB(100, 100, 100);
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
		wordChart->ChartTitle->Font->Name = "华文行楷";
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
	}

	Word::ShapePtr shape = wordShape->ConvertToShape();
	Word::WrapFormatPtr wrapp = shape->GetWrapFormat();
	if (ncolumn < 4)
	{
		shape->Height = 150;
		shape->Width = 280;
		//shape->Font->Size = 12;
		//shape->Font->Name = "华文行楷";
	}
	else
	{
		shape->Height = 260;
		shape->Width = 350;
		//shape->Font->Size = 10;
		//shape->Font->Name = "华文行楷";
	}
	shape->Height = 300;
	shape->Width = 400;
	SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);
	wrapp->PutType(Word::wdWrapInline);
	shape->Left = 100;
	shape->Top = 100;

	return BENCH_MARK_OK;
	});
}

void COfficeWordMark::AddPicture(Word::_ApplicationPtr m_pWordApp, Word::_DocumentPtr oDoc, Word::RangePtr oRangeResult, std::tstring picturename, std::tstring lpPathFile, std::tstring  strRescFP, std::tstring picturepathname)
{
	LogInfo(TEXT("插入图片")); 
	
	TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_ADD_PICTURE + picturepathname),
		[&]()->int {

	oRangeResult = FindText(
		oDoc,
		picturename.c_str(),
		oRangeResult);
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Select();
	SleepAndRecord(dwTimerNode1);
	oRangeResult->Delete();
	SleepAndRecord(dwTimerNode1);
	lpPathFile = strRescFP + picturepathname.c_str();
	SleepAndRecord(dwTimerNode1);
	Word::InlineShapePtr m_picShape = oDoc->InlineShapes->AddPicture(
		_bstr_t(lpPathFile.c_str()),
		&(_variant_t(false)),
		&(_variant_t(true)),
		&(_variant_t((long)DISP_E_PARAMNOTFOUND, VT_ERROR)));

	SleepAndRecord(dwTimerNode);
	m_picShape->Width = 150.0f;
	SleepAndRecord(dwTimerNode);
	m_picShape->Height = 120.0f;
	SleepAndRecord(dwTimerNode);
	m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphCenter;
	return BENCH_MARK_OK;
	});
}
int COfficeWordMark::WordDemo2()
{
	MyPlatform::tstring lpPathFile;

	try
	{
		Word::DocumentsPtr oDocs;
		Word::_DocumentPtr oDoc;
		Word::SelectionPtr wordSelection;
		Word::TablesPtr wordTables;
		Word::TablePtr wordTable;
		Word::ChartPtr wordChart;
		Word::InlineShapePtr wordShape;

		TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_OPEN_DOCUMENT + _T("temp1.docx")),
			[&]()->int {
			// Open Document
			oDocs = m_pWordApp->Documents;
			oDoc = oDocs->Open(&(_variant_t((const TCHAR*)m_strDocFP)));

			return BENCH_MARK_OK; });

		LogInfo(_T("OfficeWord:打开word程序"));
		LogInfo(TEXT("打开word程序"));
		//AuUDF::MsOfficeActivationWizardSkip(TEXT("WINWORD\\.EXE"), 3000, 5000);

		Word::RangePtr oRangeResult, oRangeTemp;
		LogInfo(TEXT("插入文档"));
		SleepAndRecord(4 * dwTimerNode3);//timer
		lpPathFile = m_strRescFP + TEXT("temp.txt");
		oRangeResult = FindText(
			oDoc,
			TEXT("code-1"),
			oRangeResult);
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);//timer
		oRangeResult->Select();
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);//timer
		m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);//timer
		oRangeResult->Delete();
		SleepAndRecord((DWORD)CMarkObject::dwTimerNode1);//timer
		oRangeResult->Font->Size = 11.5f;
		oRangeResult->Font->Name = TEXT("华文行楷");
		InsertAfter(oRangeResult, lpPathFile.c_str(), 0);

		//用户调查项目总结报告
		LogInfo(CWord_Opids::DESC_SET_TITLE.c_str());

		TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_SET_TITLE + TEXT("用户调查项目总结报告")),
			[&]()->int {
			LogInfo(TEXT("设置标题"));
			oRangeResult = FindText(
				oDoc,
				TEXT("用户调查项目总结报告"));
			SleepAndRecord(dwTimerNode1);
			oRangeResult->Select();
			SleepAndRecord(dwTimerNode1);
			m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
			SleepAndRecord(dwTimerNode1);
			oRangeResult->PutStyle(&(_variant_t(Word::WdBuiltinStyle::wdStyleNormal)));
			m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphCenter;
			SleepAndRecord(dwTimerNode1);
			oRangeResult->Font->Size = 21.0f;
			oRangeResult->Font->Name = TEXT("华文行楷");

			return BENCH_MARK_OK; });

		//天宝银号市场部·2026年6月
		LogInfo(CWord_Opids::DESC_SET_CHAR_FORM.c_str());

		TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_SET_CHAR_FORM + _T("天宝银号市场部·2016年6月")),
			[&]()->int {

			LogInfo(TEXT("设置字体格式"));
			oRangeResult = FindText(
				oDoc,
				TEXT("天宝银号市场部·2016年6月"),
				oRangeResult);
			SleepAndRecord(dwTimerNode1);
			oRangeResult->Select();
			SleepAndRecord(dwTimerNode1);
			m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
			oRangeResult->PutStyle(&(_variant_t(Word::WdBuiltinStyle::wdStyleNormal)));
			m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphRight;
			SleepAndRecord(dwTimerNode1);
			oRangeResult->Font->Size = 12.0f;
			oRangeResult->Font->Name = TEXT("华文行楷");

			return BENCH_MARK_OK; });
		//1. 用户调查项目概述
		{
			//1. 用户调查项目概述
			LogInfo(TEXT("设置一级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM.c_str());
			Set1stLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1. 用户调查项目概述"), _T("楷体"));


			//1.1 项目名称和目标
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.1 项目名称和目标"), _T("楷体"));


			//1.2 项目时间
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.2 项目时间"), _T("楷体"));

			//1.3 项目组成员
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.3 项目组成员"), _T("楷体"));


			//1.4 调查方法
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.4 调查方法"), _T("楷体"));

			//打字
			LogInfo(CWord_Opids::DESC_INPUT_TEXT.c_str());

			TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_INPUT_TEXT + _T("此次调查主要在天宝银号设有分支机构")),
				[&]()->int {
				LogInfo(TEXT("输入文字"));
				oRangeResult = FindText(
					oDoc,
					TEXT("此次调查主要在天宝银号设有分支机构（分公司或办事处）的省市开展，选择调查对象时结合使用了重点抽样和随机抽样的方法，在设计调查问卷时则强调客观问题和主观问题相结合，以客观问题为主的设计方法。"),
					oRangeResult);
				SleepAndRecord(dwTimerNode1);
				oRangeResult->Select();
				SleepAndRecord(dwTimerNode1);
				m_pWordApp->ActiveWindow->SmallScroll(&_variant_t(12));
				SleepAndRecord(dwTimerNode1);
				oRangeResult->Delete();
				std::wstring text = TEXT("此次调查主要在天宝银号设有分支机构（分公司或办事处）的省市开展，选择调查对象时结合使用了重点抽样和随机抽样的方法，在设计调查问卷时则强调客观问题和主观问题相结合，以客观问题为主的设计方法。");
				InsertAfter(oRangeResult, text);

				return BENCH_MARK_OK; });

			//1.4.1 样本选择
			LogInfo(TEXT("设置三级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.4.1 样本选择"), _T("楷体"));

			//1.4.2 问卷设计
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.4.2 问卷设计"), _T("楷体"));
			LogInfo(TEXT("设置三级标题格式"));

			{
				//insert firstpage bj

				LogInfo(TEXT("插入图片"));
				LogInfo(CWord_Opids::DESC_ADD_PICTURE.c_str());
				AddPicture(m_pWordApp, oDoc, oRangeResult, _T("code-fbg"), lpPathFile, m_strRescFP.GetString(), _T("code-sbg.jpg"));

			}

			//1.4.3 问卷发放和回收
			LogInfo(TEXT("设置三级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("1.4.3 问卷发放和回收"), _T("楷体"));

		};

		//2. 数据分析
		{

			//2. 数据分析
			LogInfo(TEXT("设置一级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM.c_str());
			Set1stLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2. 数据分析"), _T("楷体"));

			//2.1 用户基本情况
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.1 用户基本情况"), _T("楷体"));


			//2.2 产品认知度
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.2 产品认知度"), _T("楷体"));

			{
				//code-table-2.21
				//不知道	知道不了解	了解	非常了解
				//18-30	4675	2675	5675	1675
				//30-50	15000	20000	20000	5000
				//50以上	6325	8325	7325	3325
				//乡镇	4750	5750	6750	3750
				//中型城市	6675	7675	8675	3675
				//大型城市	13575	15575	16575	8575

				LogInfo(TEXT("插入表格"));
				LogInfo(CWord_Opids::DESC_ADD_TABLE.c_str());
				wordSelection = AddTable(m_pWordApp, oDoc, oRangeResult, wordTables, wordTable, wordSelection, _T("code-table-2.21"), 7, 5);

				CHAR *strlist[] = { "",		"不知道","知道不了解","了解","非常了解",
					"18-30",	"4675",	"2675","5675","1675",
					"30-50",	"15000","20000","20000","5000",
					"50以上",	"6325","8325","7325","3325",
					"乡镇",		"4750","5750","8675","3675",
					"中型城市",	"6675","7675","8675","3675",
					"大型城市",	"13575","15575","16575","8575" };
				LogInfo(CWord_Opids::DESC_FILL_TABLE.c_str());
				FillTable(wordSelection, strlist, 7, 5, _T("code-table-2.21"));

				//code-chart-2.3
				LogInfo(TEXT("插入图表"));
				LogInfo(CWord_Opids::DESC_ADD_CHART.c_str());
				wordShape = AddChart(m_pWordApp, oDoc, oRangeResult, wordShape, _T("code-chart-2.3"), Office::XlChartType::xlLineStacked);

				LogInfo(TEXT("填充数据"));
				//SleepAndRecord(dwTimerNode2);
				CHAR *strlist1[] = { " ","受调查用户对我公司产品的了解程度","不知道","26000","知道不了解","31000","了解","33000","非常了解","10000" };
				LogInfo(CWord_Opids::DESC_FILL_CHART.c_str());
				FillChart(wordShape, strlist1, 5, 2, _T("code-chart-2.3"));
			};

			//2.2.1 按用户年龄细分的产品认知度
			LogInfo(TEXT("设置三级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.2.1 按用户年龄细分的产品认知度"), _T("楷体"));

			//2.2.2 按用户地域细分的产品认知度
			LogInfo(TEXT("设置三级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.2.2 按用户地域细分的产品认知度"), _T("楷体"));


			//2.3 产品和服务满意度
			LogInfo(TEXT("设置二级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM.c_str());
			Set2ndLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.3 产品和服务满意度"), _T("楷体"));

			//2.3.1 按年龄细分的产品和服务满意度
			LogInfo(TEXT("设置三级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.3.1 按年龄细分的产品和服务满意度"), _T("楷体"));


			//2.3.2 按地域细分的产品和服务满意度
			LogInfo(TEXT("设置三级标题格式"));
			LogInfo(CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM.c_str());
			Set3rdLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("2.3.2 按地域细分的产品和服务满意度"), _T("楷体"));

			//3. 调查结论
			{
				//3. 调查结论
				LogInfo(TEXT("设置一级标题格式"));
				LogInfo(CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM.c_str());
				Set1stLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("3. 调查结论"), _T("楷体"));

			};

			//4. 结语
			{
				//4. 结语
				LogInfo(TEXT("设置一级标题格式"));
				LogInfo(CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM.c_str());
				Set1stLevelTitleForm(m_pWordApp, oDoc, oRangeResult, _T("4. 结语"), _T("楷体"));

			};

			//5. 目录
			{
				LogInfo(CWord_Opids::DESC_CONTENT.c_str());

				TakeMeasurement(new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_CONTENT + _T("目录")),
					[&]()->int {
					oRangeResult = FindText(
						oDoc,
						TEXT("目录"));
					SleepAndRecord(dwTimerNode1);
					oRangeResult->Select();
					SleepAndRecord(dwTimerNode1);
					oRangeResult->PutStyle(&(_variant_t(Word::WdBuiltinStyle::wdStyleNormal)));
					m_pWordApp->Selection->ParagraphFormat->Alignment = Word::WdParagraphAlignment::wdAlignParagraphCenter;

					LogInfo(TEXT("设置字号"));
					SleepAndRecord(dwTimerNode1);
					oRangeResult->Font->Size = 15.0f;

					LogInfo(TEXT("设置字体"));
					SleepAndRecord(dwTimerNode2);
					oRangeResult->Font->Name = TEXT("华文楷体");

					LogInfo(TEXT("插入目录"));
					SleepAndRecord(dwTimerNode2);
					oRangeResult = FindText(
						oDoc,
						TEXT("目录"));
					SleepAndRecord(dwTimerNode1);
					oRangeResult->MoveStart(&(_variant_t(Word::WdUnits::wdParagraph)), &(_variant_t(1)));

					oRangeResult->Select();
					SleepAndRecord(dwTimerNode1);
					//	g_ApplicationPtr->ActiveWindow->SmallScroll(&_variant_t(50));
					SleepAndRecord(dwTimerNode1);
					Word::TablesOfContentsPtr lpTablesOfContents = oDoc->TablesOfContents->Add(
						oRangeResult,
						&(_variant_t(true)),
						&(_variant_t(1)),
						&(_variant_t(9)),
						&(_variant_t(false)),
						&vtMissing,
						&(_variant_t(true)),
						&(_variant_t(true)),
						&vtMissing,
						&(_variant_t(true)),
						&(_variant_t(true)),
						&(_variant_t(false)));
					SleepAndRecord(dwTimerNode1);
					//	oDoc->TablesOfContents->Format = Word::WdTocFormat::wdTOCClassic;
					SleepAndRecord(dwTimerNode1);
					oDoc->TablesOfContents->Item(1)->TabLeader = Word::WdTabLeader::wdTabLeaderDots;

					return BENCH_MARK_OK; });

			}

			LogInfo(TEXT("关闭文档"));
			LogInfo(CWord_Opids::DESC_CLOSE_DOCUMENT.c_str());

			//TakeMeasurement(CWord_Opids::OPID_CLOSE_DOCUMENT, new CCounterContent(CWord_Opids::DESC_TYPE_MARKER, CWord_Opids::DESC_CLOSE_DOCUMENT + CWord_Opids::DESC_CLOSE_DOCUMENT));

			SleepAndRecord(dwTimerNode2);
			{
				lpPathFile = m_strRescFP + TEXT("产品调查报告_temp.doc");
				SleepAndRecord(dwTimerNode1);
				oDoc->SaveAs(&(_variant_t(lpPathFile.c_str())));
				SleepAndRecord(dwTimerNode1);
				oDoc->Close(&(_variant_t(false)));
			};
			//return BENCH_MARK_OK; });

			//Double tresult = AuTimer().TimerDiff(tbegin);
			LogInfo(TEXT("结束编辑"));
			LogInfo(_T("OfficeWord:结束编辑"));

			m_pWordApp->Quit();
			return BENCH_MARK_OK;
		}
	}
	catch (_com_error &err)
	{
		LogError({_T("执行错误，错误原因"), err.ErrorMessage()
	});
		return -1;
	}
}
 
