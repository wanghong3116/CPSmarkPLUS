#include "stdafx.h"
#include "Chrome_Opids.h"


CChrome_Opids::CChrome_Opids()
{
	for (int i = 0, j=0; i < 60; )
	{
		i += 2;
		j++;
		std::wstring idx = MyPlatform::Int64ToStringW(j);
		m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION + i, DESC_OPEN_APPLICATION + idx));
		m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + i, DESC_OPEN_DOCUMENT + idx + _T("_863619.html")));

		i += 2;
		j++;
		idx = MyPlatform::Int64ToStringW(j);
		m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION + i, DESC_OPEN_APPLICATION + idx));
		m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + i, DESC_OPEN_DOCUMENT + idx + _T("_972747.html")));

		i += 2;
		j++;
		idx = MyPlatform::Int64ToStringW(j);
		m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION + i, DESC_OPEN_APPLICATION + idx));
		m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT + i, DESC_OPEN_DOCUMENT + idx + _T("_975632.html")));
	}
	

	
}

CChrome_Opids::~CChrome_Opids()
{
}
tstring CChrome_Opids::DESC_TYPE_MARKER = _T("Chrome");
tstring CChrome_Opids::DESC_OPEN_APPLICATION = _T("Chrome::打开程序(Open Application)");
tstring CChrome_Opids::DESC_OPEN_DOCUMENT = _T("Chrome::打开文档(Open Application And Document)");
int CChrome_Opids::OPID_TYPE_MARKER = 20000;
int CChrome_Opids::OPID_OPEN_APPLICATION = 20100;
int	CChrome_Opids::OPID_OPEN_DOCUMENT = 20200;