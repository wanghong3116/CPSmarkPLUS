#include "stdafx.h"
#include "3DSMaxWhale_Opids.h"


C3DSMaxWhale_Opids::C3DSMaxWhale_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_RUN + 10, DESC_RUN));
 
	 
 }


C3DSMaxWhale_Opids::~C3DSMaxWhale_Opids()
{
}
tstring C3DSMaxWhale_Opids::DESC_TYPE_MARKER = _T("3DSMaxWhale");
tstring C3DSMaxWhale_Opids::DESC_OPEN_APPLICATION = _T("3DSMaxWhale::打开程序(Open Application)");
tstring C3DSMaxWhale_Opids::DESC_OPEN_DOCUMENT = _T("3DSMaxWhale::打开文档(Open Application And Document)");
int C3DSMaxWhale_Opids::OPID_TYPE_MARKER = 100000;
int C3DSMaxWhale_Opids::OPID_OPEN_APPLICATION = 100100;
int	C3DSMaxWhale_Opids::OPID_OPEN_DOCUMENT = 100200;

tstring C3DSMaxWhale_Opids::DESC_RUN = _T("3DSMaxWhale::Run");
int C3DSMaxWhale_Opids::OPID_RUN = 100300;
