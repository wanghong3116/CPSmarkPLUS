#include <stdafx.h>
#include <Word_Opids.h>

CWord_Opids::CWord_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 10, DESC_OPEN_DOCUMENT + _T("temp1.docx")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_DOCUMENT + 50, DESC_ADD_DOCUMENT + _T("产品调查报告_temp.doc")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_TITLE, DESC_SET_TITLE + _T("用户调查项目总结报告")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_CHAR_FORM, DESC_SET_CHAR_FORM + _T("天宝银号市场部·2016年6月")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_FONTFAMILY, DESC_SET_FONTFAMILY));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_FONT, DESC_SET_FONT));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 10, DESC_SET_LEVEL_1_TITLE_FORM + _T("1. 用户调查项目概述")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.1 项目名称和目标")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 2, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.2 项目时间")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 3, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.3 项目组成员")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 4, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.4 调查方法")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 1, DESC_SET_LEVEL_3_TITLE_FORM + _T("1.4.1 样本选择")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 2, DESC_SET_LEVEL_3_TITLE_FORM + _T("1.4.2 问卷设计")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 3, DESC_SET_LEVEL_3_TITLE_FORM + _T("1.4.3 问卷发放和回收")));
	// second run
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 30, DESC_SET_LEVEL_1_TITLE_FORM + _T("2. 数据分析")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 5, DESC_SET_LEVEL_2_TITLE_FORM + _T("2.1 用户基本情况")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 6, DESC_SET_LEVEL_2_TITLE_FORM + _T("2.2 产品认知度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 7, DESC_SET_LEVEL_2_TITLE_FORM + _T("2.3 产品和服务满意度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 8, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.4 调查方法")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 4, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.2.1 按用户年龄细分的产品认知度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 5, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.2.2 按用户地域细分的产品认知度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 6, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.3.1 按年龄细分的产品和服务满意度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 7, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.3.2 按地域细分的产品和服务满意度")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_INPUT_TEXT, DESC_INPUT_TEXT + _T("此次调查主要在天宝银号设有分支机构")));
	 
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PICTURE, DESC_ADD_PICTURE + _T("code-sbg.jpg")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE, DESC_ADD_TABLE + _T("code-table-2.21")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE, DESC_FILL_TABLE + _T("code-table-2.21")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART, DESC_ADD_CHART + _T("code-chart-2.3")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART, DESC_FILL_CHART + _T("code-chart-2.3")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 30 * 2, DESC_SET_LEVEL_1_TITLE_FORM + _T("3. 调查结论")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 30 * 3, DESC_SET_LEVEL_1_TITLE_FORM + _T("4. 结语")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_CONTENT, DESC_CONTENT + _T("目录")));
}
 
CWord_Opids::~CWord_Opids()
{
}
tstring CWord_Opids::DESC_TYPE_MARKER = _T("Office::Word");
tstring CWord_Opids::DESC_OPEN_APPLICATION = _T("OfficeWord:打开程序(Open Application)");
tstring CWord_Opids::DESC_OPEN_DOCUMENT = _T("OfficeWord:打开文档(Open Document)");
tstring CWord_Opids::DESC_ADD_DOCUMENT = _T("OfficeWord:插入文档(Add Document)");
tstring CWord_Opids::DESC_SET_TITLE = _T("OfficeWord:设置标题(Set Title)");
tstring CWord_Opids::DESC_SET_CHAR_FORM = _T("OfficeWord:设置字体格式(Set Char Form)");
tstring CWord_Opids::DESC_SET_FONTFAMILY = _T("OfficeWord:设置字号(Set Font Family)");
tstring CWord_Opids::DESC_SET_FONT = _T("OfficeWord:设置字体(Set Font)");
tstring CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM = _T("OfficeWord:设置一级标题格式(Set Level 1 Title Format)");
tstring CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM = _T("OfficeWord:设置二级标题格式(Set Level 2 Title Format)");
tstring CWord_Opids::DESC_INPUT_TEXT = _T("OfficeWord:输入文字(Input Characters)");
tstring CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM = _T("OfficeWord:设置三级标题格式(Set Level 3 Title Format)");
tstring CWord_Opids::DESC_ADD_CHART = _T("OfficeWord:插入图表(Add Chart)");
tstring CWord_Opids::DESC_ADD_PICTURE = _T("OfficeWord:插入图片(Add Piture)");
tstring CWord_Opids::DESC_FILL_CHART = _T("OfficeWord:填充数据(Fill Chart)");
tstring CWord_Opids::DESC_FILL_TABLE = _T("OfficeWord:填充表格(File Table)");
tstring CWord_Opids::DESC_CLOSE_DOCUMENT = _T("OfficeWord:关闭文档(Close Document)");
tstring CWord_Opids::DESC_ADD_TABLE = _T("OfficeWord:插入表格(Add Table)");
tstring CWord_Opids::DESC_CONTENT = _T("OfficeWord:目录(Content)");

int CWord_Opids::OPID_TYPE_MARKER = 40000;

int CWord_Opids::OPID_OPEN_APPLICATION = 40100;
int	CWord_Opids::OPID_OPEN_DOCUMENT = 40200;

int CWord_Opids::OPID_SET_TITLE = 40300;
int CWord_Opids::OPID_SET_CHAR_FORM = 40400;
int CWord_Opids::OPID_SET_FONTFAMILY = 40500;
int CWord_Opids::OPID_SET_FONT = 40600;
int CWord_Opids::OPID_SET_LEVEL_2_TITLE_FORM = 40700;
int CWord_Opids::OPID_INPUT_TEXT = 40800;
int CWord_Opids::OPID_SET_LEVEL_3_TITLE_FORM = 40900;
int CWord_Opids::OPID_INVESTIGATION_RESULT = 41000;
int CWord_Opids::OPID_ADD_CHART = 41300;
int CWord_Opids::OPID_ADD_PICTURE = 41400;
int CWord_Opids::OPID_FILL_CHART = 41500;
int CWord_Opids::OPID_SET_LEVEL_1_TITLE_FORM = 41600;
int CWord_Opids::OPID_FILL_TABLE = 41700;
int CWord_Opids::OPID_CLOSE_DOCUMENT = 41800;
int CWord_Opids::OPID_ADD_TABLE = 41900;
int	CWord_Opids::OPID_ADD_DOCUMENT = 42000;
int	CWord_Opids::OPID_CONTENT = 42100;



