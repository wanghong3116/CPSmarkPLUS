#include "stdafx.h"
#include "AE_Opids.h"


CAE_Opids::CAE_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_RUN + 10, DESC_RUN));
}


CAE_Opids::~CAE_Opids()
{
}
tstring CAE_Opids::DESC_TYPE_MARKER = _T("AE");
tstring CAE_Opids::DESC_OPEN_APPLICATION = _T("AE::�򿪳���(Open Application)");
tstring CAE_Opids::DESC_OPEN_DOCUMENT = _T("AE::���ĵ�(Open Application And Document)");
int CAE_Opids::OPID_TYPE_MARKER = 110000;
int CAE_Opids::OPID_OPEN_APPLICATION = 110100;
int	CAE_Opids::OPID_OPEN_DOCUMENT = 110200;


tstring CAE_Opids::DESC_RUN = _T("AE::Run");
int CAE_Opids::OPID_RUN = 110300;

