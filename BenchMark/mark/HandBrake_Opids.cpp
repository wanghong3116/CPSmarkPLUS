#include "stdafx.h"
#include "HandBrake_Opids.h"

CHandBrake_Opids::CHandBrake_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_RUN + 10, DESC_RUN));
}

CHandBrake_Opids::~CHandBrake_Opids()
{
}

tstring CHandBrake_Opids::DESC_TYPE_MARKER = _T("HandBrake");
tstring CHandBrake_Opids::DESC_OPEN_APPLICATION = _T("HandBrake::打开程序(Open Application)");
tstring CHandBrake_Opids::DESC_OPEN_DOCUMENT = _T("HandBrake::打开文档(Open Application And Document)");
int CHandBrake_Opids::OPID_TYPE_MARKER = 120000;
int CHandBrake_Opids::OPID_OPEN_APPLICATION = 120100;
int	CHandBrake_Opids::OPID_OPEN_DOCUMENT = 120200;
int	CHandBrake_Opids::OPID_VIDEO_ENCODE = 120300;



tstring CHandBrake_Opids::DESC_RUN = _T("HandBrake::Run");
int CHandBrake_Opids::OPID_RUN = 120300;