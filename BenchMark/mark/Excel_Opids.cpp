#include "stdafx.h"
#include "..\Excel_Opids.h"


CExcel_Opids::CExcel_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT, DESC_OPEN_DOCUMENT + _T("excel_book_03_temp.xlsx")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 50, DESC_OPEN_DOCUMENT + _T("excel_book_03.xlsx")));
	
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SELECT_CHART_RANGE, DESC_SELECT_CHART_RANGE + _T("�ܵ����û����ҹ�˾��Ʒ���˽�̶�")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_RANGE, DESC_FILL_RANGE + _T("Excel:��Ʒ��֪��:�������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 10, DESC_ADD_CHART + _T("Excel:����ͼ��:��Ʒ��֪��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 20, DESC_ADD_CHART + _T("Excel:����ͼ��:�����Ⱥ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 30, DESC_ADD_CHART + _T("Excel:����ͼ��:����������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_CALCULATION_TEST, DESC_CALCULATION_TEST + _T("�������")));
}

CExcel_Opids::~CExcel_Opids()
{
}

tstring CExcel_Opids::DESC_TYPE_MARKER = _T("Office::Excel");
tstring CExcel_Opids::DESC_OPEN_APPLICATION = _T("�򿪳���(Open Excel Application)");
tstring CExcel_Opids::DESC_OPEN_DOCUMENT = _T("���ĵ�(Open Excel Document)");

tstring CExcel_Opids::DESC_ADD_DOCUMENT = _T("OfficeExcel:�����ĵ�(Add Excel Document)");

tstring CExcel_Opids::DESC_ADD_CHART = _T("OfficeExcel:����ͼ��(Add Chart)");
tstring CExcel_Opids::DESC_FILL_CHART = _T("OfficeExcel:�������(Fill Chart)");

tstring CExcel_Opids::DESC_CLOSE_DOCUMENT = _T("OfficeExcel:�ر��ĵ�(Close Document)");
tstring CExcel_Opids::DESC_SELECT_CHART_RANGE = _T("OfficeExcel:ѡ������(Select Chart Range)");
tstring CExcel_Opids::DESC_CALCULATION_TEST = _T("OfficeExcel:�������(Calculation Test)");
tstring CExcel_Opids::DESC_FILL_RANGE = _T("OfficeExcel:�������(Fill Range)");


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