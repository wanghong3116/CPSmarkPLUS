#include "stdafx.h"
#include "..\PPT_Opids.h"


CPPT_Opids::CPPT_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_DOCUMENT, DESC_ADD_DOCUMENT + +_T("temp.thmx")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 1, DESC_ADD_PAGE + _T("用户调查项目总结报告")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 5, DESC_ADD_PAGE + _T("概述")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 5, DESC_FILL_PAGE + _T("概述")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 10, DESC_ADD_PAGE + _T("项目进度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 10, DESC_FILL_PAGE + _T("项目进度")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 15, DESC_ADD_PAGE + _T("项目组成员")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 15, DESC_FILL_PAGE + _T("项目组成员")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 20, DESC_ADD_PAGE + _T("问卷发放和回收")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 20, DESC_FILL_PAGE + _T("问卷发放和回收")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 25, DESC_ADD_PAGE + _T("问卷发放和回收")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 25, DESC_FILL_PAGE + _T("问卷发放和回收")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 30, DESC_ADD_PAGE + _T("数据分析")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 35, DESC_ADD_PAGE + _T("用户基本情况")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 35, DESC_FILL_PAGE + _T("用户基本情况")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE + 10, DESC_ADD_TABLE + _T("年龄")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE + 10, DESC_FILL_TABLE + _T("年龄")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE + 20, DESC_ADD_TABLE + _T("区域")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE + 20, DESC_FILL_TABLE + _T("区域")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 40, DESC_ADD_PAGE + _T("受调查对象的年龄分布情况")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 40, DESC_FILL_PAGE + _T("受调查对象的年龄分布情况")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 10, DESC_ADD_CHART + _T("受调查对象的年龄分布情况")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART + 10, DESC_FILL_CHART + _T("受调查对象的年龄分布情况")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 45, DESC_ADD_PAGE + _T("用户的地域分布情况")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 45, DESC_FILL_PAGE + _T("用户的地域分布情况")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 20, DESC_ADD_CHART + _T("受调查对象的区域分布情况")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART + 20, DESC_FILL_CHART + _T("受调查对象的区域分布情况")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 50, DESC_ADD_PAGE + _T("对产品的了解度:Table")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 50, DESC_FILL_PAGE + _T("对产品的了解度:Table")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE + 30, DESC_ADD_TABLE + _T("对产品的了解度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE + 30, DESC_FILL_TABLE + _T("对产品的了解度")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 55, DESC_ADD_PAGE + _T("对产品的了解度:Chart")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 55, DESC_FILL_PAGE + _T("对产品的了解度:Chart")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART + 30, DESC_ADD_CHART + _T("对产品的了解度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART + 30, DESC_FILL_CHART + _T("对产品的了解度")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 60, DESC_ADD_PAGE + _T("结语")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_PAGE + 60, DESC_FILL_PAGE + _T("结语")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PICTURE + 10, DESC_ADD_PICTURE + _T("ppt-1.jpg")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PICTURE + 50, DESC_ADD_PICTURE + _T("ppt-2.jpg")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PAGE + 65, DESC_ADD_PAGE + _T("用户调查项目概述")));

}

CPPT_Opids::~CPPT_Opids()
{
}
 
tstring CPPT_Opids::DESC_TYPE_MARKER = _T("Office::PPT");
tstring CPPT_Opids::DESC_OPEN_APPLICATION = _T("打开程序(Open PPT Application)");
tstring CPPT_Opids::DESC_OPEN_DOCUMENT = _T("打开文档(Open PPT Document)");
tstring CPPT_Opids::DESC_ADD_DOCUMENT = _T("OfficePowerPoint:插入文档(Add PPT Document)");

tstring CPPT_Opids::DESC_ADD_PAGE = _T("OfficePowerPoint:插入页(Add Page)");
tstring CPPT_Opids::DESC_FILL_PAGE = _T("OfficePowerPoint:填充页(Fill Page)");

tstring CPPT_Opids::DESC_ADD_CHART = _T("OfficePowerPoint:插入图表(Add Chart)");
tstring CPPT_Opids::DESC_FILL_CHART = _T("OfficePowerPoint:填充数据(Fill Chart)");

tstring CPPT_Opids::DESC_FILL_TABLE = _T("OfficePowerPoint:填充表格(Fill Table)");
tstring CPPT_Opids::DESC_ADD_TABLE = _T("OfficePowerPoint:插入表格(Add Table)");
tstring CPPT_Opids::DESC_CLOSE_DOCUMENT = _T("OfficePowerPoint:关闭文档(Close Document)");
tstring CPPT_Opids::DESC_ADD_PICTURE = _T("OfficePowerPoint:插入图片(Add Picture)");

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
