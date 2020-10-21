#include <stdafx.h>
#include <Word_Opids.h>

CWord_Opids::CWord_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 10, DESC_OPEN_DOCUMENT + _T("temp1.docx")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_DOCUMENT + 50, DESC_ADD_DOCUMENT + _T("��Ʒ���鱨��_temp.doc")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_TITLE, DESC_SET_TITLE + _T("�û�������Ŀ�ܽᱨ��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_CHAR_FORM, DESC_SET_CHAR_FORM + _T("�챦�����г�����2016��6��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_FONTFAMILY, DESC_SET_FONTFAMILY));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_FONT, DESC_SET_FONT));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 10, DESC_SET_LEVEL_1_TITLE_FORM + _T("1. �û�������Ŀ����")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.1 ��Ŀ���ƺ�Ŀ��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 2, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.2 ��Ŀʱ��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 3, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.3 ��Ŀ���Ա")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 4, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.4 ���鷽��")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 1, DESC_SET_LEVEL_3_TITLE_FORM + _T("1.4.1 ����ѡ��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 2, DESC_SET_LEVEL_3_TITLE_FORM + _T("1.4.2 �ʾ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 3, DESC_SET_LEVEL_3_TITLE_FORM + _T("1.4.3 �ʾ��źͻ���")));
	// second run
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 30, DESC_SET_LEVEL_1_TITLE_FORM + _T("2. ���ݷ���")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 5, DESC_SET_LEVEL_2_TITLE_FORM + _T("2.1 �û��������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 6, DESC_SET_LEVEL_2_TITLE_FORM + _T("2.2 ��Ʒ��֪��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 7, DESC_SET_LEVEL_2_TITLE_FORM + _T("2.3 ��Ʒ�ͷ��������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_2_TITLE_FORM + 10 * 8, DESC_SET_LEVEL_2_TITLE_FORM + _T("1.4 ���鷽��")));

	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 4, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.2.1 ���û�����ϸ�ֵĲ�Ʒ��֪��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 5, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.2.2 ���û�����ϸ�ֵĲ�Ʒ��֪��")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 6, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.3.1 ������ϸ�ֵĲ�Ʒ�ͷ��������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_3_TITLE_FORM + 5 * 7, DESC_SET_LEVEL_3_TITLE_FORM + _T("2.3.2 ������ϸ�ֵĲ�Ʒ�ͷ��������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_INPUT_TEXT, DESC_INPUT_TEXT + _T("�˴ε�����Ҫ���챦�������з�֧����")));
	 
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_PICTURE, DESC_ADD_PICTURE + _T("code-sbg.jpg")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_TABLE, DESC_ADD_TABLE + _T("code-table-2.21")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_TABLE, DESC_FILL_TABLE + _T("code-table-2.21")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_ADD_CHART, DESC_ADD_CHART + _T("code-chart-2.3")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_FILL_CHART, DESC_FILL_CHART + _T("code-chart-2.3")));
	
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 30 * 2, DESC_SET_LEVEL_1_TITLE_FORM + _T("3. �������")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SET_LEVEL_1_TITLE_FORM + 30 * 3, DESC_SET_LEVEL_1_TITLE_FORM + _T("4. ����")));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_CONTENT, DESC_CONTENT + _T("Ŀ¼")));
}
 
CWord_Opids::~CWord_Opids()
{
}
tstring CWord_Opids::DESC_TYPE_MARKER = _T("Office::Word");
tstring CWord_Opids::DESC_OPEN_APPLICATION = _T("OfficeWord:�򿪳���(Open Application)");
tstring CWord_Opids::DESC_OPEN_DOCUMENT = _T("OfficeWord:���ĵ�(Open Document)");
tstring CWord_Opids::DESC_ADD_DOCUMENT = _T("OfficeWord:�����ĵ�(Add Document)");
tstring CWord_Opids::DESC_SET_TITLE = _T("OfficeWord:���ñ���(Set Title)");
tstring CWord_Opids::DESC_SET_CHAR_FORM = _T("OfficeWord:���������ʽ(Set Char Form)");
tstring CWord_Opids::DESC_SET_FONTFAMILY = _T("OfficeWord:�����ֺ�(Set Font Family)");
tstring CWord_Opids::DESC_SET_FONT = _T("OfficeWord:��������(Set Font)");
tstring CWord_Opids::DESC_SET_LEVEL_1_TITLE_FORM = _T("OfficeWord:����һ�������ʽ(Set Level 1 Title Format)");
tstring CWord_Opids::DESC_SET_LEVEL_2_TITLE_FORM = _T("OfficeWord:���ö��������ʽ(Set Level 2 Title Format)");
tstring CWord_Opids::DESC_INPUT_TEXT = _T("OfficeWord:��������(Input Characters)");
tstring CWord_Opids::DESC_SET_LEVEL_3_TITLE_FORM = _T("OfficeWord:�������������ʽ(Set Level 3 Title Format)");
tstring CWord_Opids::DESC_ADD_CHART = _T("OfficeWord:����ͼ��(Add Chart)");
tstring CWord_Opids::DESC_ADD_PICTURE = _T("OfficeWord:����ͼƬ(Add Piture)");
tstring CWord_Opids::DESC_FILL_CHART = _T("OfficeWord:�������(Fill Chart)");
tstring CWord_Opids::DESC_FILL_TABLE = _T("OfficeWord:�����(File Table)");
tstring CWord_Opids::DESC_CLOSE_DOCUMENT = _T("OfficeWord:�ر��ĵ�(Close Document)");
tstring CWord_Opids::DESC_ADD_TABLE = _T("OfficeWord:������(Add Table)");
tstring CWord_Opids::DESC_CONTENT = _T("OfficeWord:Ŀ¼(Content)");

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



