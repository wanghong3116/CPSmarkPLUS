#include "stdafx.h"
#include "..\Excel_Opids.h"


CExcel_Opids::CExcel_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT, DESC_OPEN_DOCUMENT + _T("excel_book_03_temp.xlsx")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 50, DESC_OPEN_DOCUMENT + _T("excel_book_03.xlsx")));
	
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SELECT_CHART_RANGE, DESC_SELECT_CHART_RANGE + _T("受调查用户对我公司产品的了解程度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_RANGE, DESC_FILL_RANGE + _T("Excel:产品认知度:填充数据")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 10, DESC_ADD_CHART + _T("Excel:生成图表:产品认知度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 20, DESC_ADD_CHART + _T("Excel:生成图表:针对人群调查")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 30, DESC_ADD_CHART + _T("Excel:生成图表:针对区域调查")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_CALCULATION_TEST, DESC_CALCULATION_TEST + _T("计算测试")));
}

CExcel_Opids::~CExcel_Opids()
{
}

tstring CExcel_Opids::DESC_TYPE_MARKER = _T("Office::Excel");
tstring CExcel_Opids::DESC_OPEN_APPLICATION = _T("打开程序(Open Excel Application)");
tstring CExcel_Opids::DESC_OPEN_DOCUMENT = _T("打开文档(Open Excel Document)");

tstring CExcel_Opids::DESC_ADD_DOCUMENT = _T("OfficeExcel:插入文档(Add Excel Document)");

tstring CExcel_Opids::DESC_ADD_CHART = _T("OfficeExcel:插入图表(Add Chart)");
tstring CExcel_Opids::DESC_FILL_CHART = _T("OfficeExcel:填充数据(Fill Chart)");

tstring CExcel_Opids::DESC_CLOSE_DOCUMENT = _T("OfficeExcel:关闭文档(Close Document)");
tstring CExcel_Opids::DESC_SELECT_CHART_RANGE = _T("OfficeExcel:选择区域(Select Chart Range)");
tstring CExcel_Opids::DESC_CALCULATION_TEST = _T("OfficeExcel:计算测试(Calculation Test)");
tstring CExcel_Opids::DESC_FILL_RANGE = _T("OfficeExcel:填充区域(Fill Range)");


int CExcel_Opids::OPID_TYPE_MARKER = 30000;

int CExcel_Opids::OPID_OPEN_APPLICATION = 30100;
int	CExcel_Opids::OPID_OPEN_DOCUMENT = 30200;

int CExcel_Opids::OPID_ADD_DOCUMENT = 30300;

int CExcel_Opids::OPID_ADD_CHART = 30600;
int CExcel_Opids::OPID_FILL_CHART = 30700;

int CExcel_Opids::OPID_CLOSE_DOCUMENT = 31100;
int CExcel_Opids::OPID_SELECT_CHART_RANGE = 31200;
int CExcel_Opids::OPID_CALCULATION_TEST = 31300;
int CExcel_Opids::OPID_FILL_RANGE = 31400;