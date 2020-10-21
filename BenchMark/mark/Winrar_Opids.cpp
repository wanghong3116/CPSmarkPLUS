#include "stdafx.h"
#include "Winrar_Opids.h"


CWinrar_Opids::CWinrar_Opids()
{
 	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_RAR, DESC_RAR));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_UNRAR, DESC_UNRAR));
}

CWinrar_Opids::~CWinrar_Opids()
{
}
tstring CWinrar_Opids::DESC_TYPE_MARKER = _T("WinRar");
tstring CWinrar_Opids::DESC_RAR = _T("压缩(Rar)");
tstring CWinrar_Opids::DESC_UNRAR = _T("解压缩(UnRar)");
tstring CWinrar_Opids::DESC_OPEN_APPLICATION = _T("打开程序(Open Winrar Application)");
tstring CWinrar_Opids::DESC_OPEN_DOCUMENT = _T("打开文档(Open Winrar Document)");

int CWinrar_Opids::OPID_TYPE_MARKER = 80000;
int CWinrar_Opids::OPID_RAR = 82000;
int CWinrar_Opids::OPID_UNRAR = 83000;

int CWinrar_Opids::OPID_OPEN_APPLICATION = 80100;
int	CWinrar_Opids::OPID_OPEN_DOCUMENT = 80200;


