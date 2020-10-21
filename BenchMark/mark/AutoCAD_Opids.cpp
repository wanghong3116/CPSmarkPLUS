#include "stdafx.h"
#include "AutoCAD_Opids.h"


CAutoCAD_Opids::CAutoCAD_Opids()
{
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT, DESC_OPEN_DOCUMENT));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_DRAWING + 10, DESC_DRAWING));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SCALING + 10, DESC_SCALING));
}

CAutoCAD_Opids::~CAutoCAD_Opids()
{
}
 
tstring CAutoCAD_Opids::DESC_TYPE_MARKER = _T("AutoCAD");
tstring CAutoCAD_Opids::DESC_OPEN_APPLICATION = _T("AutoCAD::打开程序(Open Application)");
tstring CAutoCAD_Opids::DESC_OPEN_DOCUMENT = _T("AutoCAD::打开文档(Open Document)");
tstring CAutoCAD_Opids::DESC_DRAWING = _T("AutoCAD::房间走线图绘制(Drawing)");
tstring CAutoCAD_Opids::DESC_SCALING = _T("AutoCAD::缩放(Scaling)");

int CAutoCAD_Opids::OPID_OPEN_APPLICATION = 10100;
int	CAutoCAD_Opids::OPID_OPEN_DOCUMENT = 10200;
int CAutoCAD_Opids::OPID_DRAWING = 10300;
int CAutoCAD_Opids::OPID_SCALING = 10400;
int CAutoCAD_Opids::OPID_TYPE_MARKER = 10000;

