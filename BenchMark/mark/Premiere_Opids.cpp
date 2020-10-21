#include "stdafx.h"
#include "Premiere_Opids.h"


CPremiere_Opids::CPremiere_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_RUN + 10, DESC_RUN));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + 10, DESC_OPEN_DOCUMENT));
}


CPremiere_Opids::~CPremiere_Opids()
{
}
tstring CPremiere_Opids::DESC_TYPE_MARKER = _T("Premiere");
tstring CPremiere_Opids::DESC_OPEN_APPLICATION = _T("Premiere::打开程序(Open Application)");
tstring CPremiere_Opids::DESC_OPEN_DOCUMENT = _T("Premiere::打开文档(Open Application And Document)");

int CPremiere_Opids::OPID_TYPE_MARKER = 130000;
int CPremiere_Opids::OPID_OPEN_APPLICATION = 130100;
int	CPremiere_Opids::OPID_OPEN_DOCUMENT = 130200;

tstring CPremiere_Opids::DESC_RUN = _T("Premiere::Run");
int CPremiere_Opids::OPID_RUN = 130300;
