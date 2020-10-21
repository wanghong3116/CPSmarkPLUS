#pragma once


class COfficeExcelMark : public CMarkObject
{
public:
	COfficeExcelMark();
	~COfficeExcelMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int ExcelDemo2();
	int browseChartByCursor(HWND hWndExcel);
	int clickMenuAndMoveCursor(CPoint& rpointBase, CSize& rsizeMenuOffset, CArray<CSize>& rarraysizeMove);
	UINT64 CalculateDemo();
private:
	Excel::_ApplicationPtr g_ApplicationPtr;
	Excel::RangePtr FillRange(std::function<void(Excel::RangePtr)> action, std::tstring RangeIndex, std::tstring Title, std::tstring Value2, Excel::_WorkbookPtr oWorkbook, Excel::_WorksheetPtr oWorksheet);
	void  AddChartWithRangeAndAction(Action action, Excel::XlChartType ChartType, float ShapeLeft, float ShapeRight, std::tstring RangeIndex, std::tstring Title, std::tstring ChartTitle, std::tstring seriesName, Excel::_WorkbookPtr oWorkbook, Excel::_WorksheetPtr oWorksheet);
	Excel::RangePtr FillChart(std::function<void(Excel::RangePtr)>  action, std::tstring RangeIndex, std::tstring RangeValue, Excel::_WorkbookPtr oWorkbook, Excel::_WorksheetPtr oWorksheet);
	Excel::RangePtr SelectChartRange(std::tstring RangeIndex, std::tstring RangeValue, Excel::_WorksheetPtr oWorksheet);
};


