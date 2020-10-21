#include "stdafx.h"
#include "..\PPT_Opids.h"


CPPT_Opids::CPPT_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_DOCUMENT, DESC_ADD_DOCUMENT + +_T("temp.thmx")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 1, DESC_ADD_PAGE + _T("�û�������Ŀ�ܽᱨ��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 5, DESC_ADD_PAGE + _T("����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 5, DESC_FILL_PAGE + _T("����")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 10, DESC_ADD_PAGE + _T("��Ŀ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 10, DESC_FILL_PAGE + _T("��Ŀ����")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 15, DESC_ADD_PAGE + _T("��Ŀ���Ա")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 15, DESC_FILL_PAGE + _T("��Ŀ���Ա")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 20, DESC_ADD_PAGE + _T("�ʾ��źͻ���")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 20, DESC_FILL_PAGE + _T("�ʾ��źͻ���")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 25, DESC_ADD_PAGE + _T("�ʾ��źͻ���")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 25, DESC_FILL_PAGE + _T("�ʾ��źͻ���")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 30, DESC_ADD_PAGE + _T("���ݷ���")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 35, DESC_ADD_PAGE + _T("�û��������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 35, DESC_FILL_PAGE + _T("�û��������")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE + 10, DESC_ADD_TABLE + _T("����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE + 10, DESC_FILL_TABLE + _T("����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE + 20, DESC_ADD_TABLE + _T("����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE + 20, DESC_FILL_TABLE + _T("����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 40, DESC_ADD_PAGE + _T("�ܵ�����������ֲ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 40, DESC_FILL_PAGE + _T("�ܵ�����������ֲ����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 10, DESC_ADD_CHART + _T("�ܵ�����������ֲ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART + 10, DESC_FILL_CHART + _T("�ܵ�����������ֲ����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 45, DESC_ADD_PAGE + _T("�û��ĵ���ֲ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 45, DESC_FILL_PAGE + _T("�û��ĵ���ֲ����")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 20, DESC_ADD_CHART + _T("�ܵ�����������ֲ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART + 20, DESC_FILL_CHART + _T("�ܵ�����������ֲ����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 50, DESC_ADD_PAGE + _T("�Բ�Ʒ���˽��:Table")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 50, DESC_FILL_PAGE + _T("�Բ�Ʒ���˽��:Table")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE + 30, DESC_ADD_TABLE + _T("�Բ�Ʒ���˽��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE + 30, DESC_FILL_TABLE + _T("�Բ�Ʒ���˽��")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 55, DESC_ADD_PAGE + _T("�Բ�Ʒ���˽��:Chart")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 55, DESC_FILL_PAGE + _T("�Բ�Ʒ���˽��:Chart")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 30, DESC_ADD_CHART + _T("�Բ�Ʒ���˽��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART + 30, DESC_FILL_CHART + _T("�Բ�Ʒ���˽��")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 60, DESC_ADD_PAGE + _T("����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 60, DESC_FILL_PAGE + _T("����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PICTURE + 10, DESC_ADD_PICTURE + _T("ppt-1.jpg")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PICTURE + 50, DESC_ADD_PICTURE + _T("ppt-2.jpg")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 65, DESC_ADD_PAGE + _T("�û�������Ŀ����")));

}

CPPT_Opids::~CPPT_Opids()
{
}
 
tstring CPPT_Opids::DESC_TYPE_MARKER = _T("Office::PPT");
tstring CPPT_Opids::DESC_OPEN_APPLICATION = _T("�򿪳���(Open PPT Application)");
tstring CPPT_Opids::DESC_OPEN_DOCUMENT = _T("���ĵ�(Open PPT Document)");
tstring CPPT_Opids::DESC_ADD_DOCUMENT = _T("OfficePowerPoint:�����ĵ�(Add PPT Document)");

tstring CPPT_Opids::DESC_ADD_PAGE = _T("OfficePowerPoint:����ҳ(Add Page)");
tstring CPPT_Opids::DESC_FILL_PAGE = _T("OfficePowerPoint:���ҳ(Fill Page)");

tstring CPPT_Opids::DESC_ADD_CHART = _T("OfficePowerPoint:����ͼ��(Add Chart)");
tstring CPPT_Opids::DESC_FILL_CHART = _T("OfficePowerPoint:�������(Fill Chart)");

tstring CPPT_Opids::DESC_FILL_TABLE = _T("OfficePowerPoint:�����(Fill Table)");
tstring CPPT_Opids::DESC_ADD_TABLE = _T("OfficePowerPoint:������(Add Table)");
tstring CPPT_Opids::DESC_CLOSE_DOCUMENT = _T("OfficePowerPoint:�ر��ĵ�(Close Document)");
tstring CPPT_Opids::DESC_ADD_PICTURE = _T("OfficePowerPoint:����ͼƬ(Add Picture)");

int CPPT_Opids::OPID_TYPE_MARKER = 51000;

int CPPT_Opids::OPID_OPEN_APPLICATION = 50100;
int	CPPT_Opids::OPID_OPEN_DOCUMENT = 50200;

int CPPT_Opids::OPID_ADD_DOCUMENT = 50300;

int CPPT_Opids::OPID_ADD_PAGE = 50400;
int CPPT_Opids::OPID_FILL_PAGE = 50500;

int CPPT_Opids::OPID_ADD_CHART = 50600;
int CPPT_Opids::OPID_FILL_CHART = 50700;

int CPPT_Opids::OPID_FILL_TABLE = 50800;
int CPPT_Opids::OPID_ADD_TABLE = 50900;
int CPPT_Opids::OPID_ADD_PICTURE = 51000;
int CPPT_Opids::OPID_CLOSE_DOCUMENT = 51100;
