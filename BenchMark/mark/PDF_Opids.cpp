#include "stdafx.h"
#include "PDF_Opids.h"


CPDF_Opids::CPDF_Opids()
{
 	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION_ACROBAT, DESC_OPEN_APPLICATION_ACROBAT));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT_ACROBAT, DESC_OPEN_DOCUMENT_ACROBAT));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT, DESC_OPEN_DOCUMENT));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SAVE_AS_PDF_DOCUMENT, DESC_SAVE_AS_PDF_DOCUMENT));
}

CPDF_Opids::~CPDF_Opids()
{
}
tstring CPDF_Opids::DESC_TYPE_MARKER = _T("Acrobat");
tstring CPDF_Opids::DESC_OPEN_APPLICATION_ACROBAT = _T("Acrobat::Open Acrobat Application");
tstring CPDF_Opids::DESC_OPEN_DOCUMENT_ACROBAT = _T("Acrobat::Open Acrobat Document");
tstring CPDF_Opids::DESC_OPEN_APPLICATION = _T("Acrobat::打开程序(Open Application)");
tstring CPDF_Opids::DESC_OPEN_DOCUMENT = _T("Acrobat::打开文档(Open Document)");
tstring CPDF_Opids::DESC_SAVE_AS_PDF_DOCUMENT = _T("Acrobat::保存为PDF(Save As PDF Document)");

 int CPDF_Opids::OPID_TYPE_MARKER = 60000;
 int CPDF_Opids::OPID_OPEN_APPLICATION_ACROBAT = 60300;
 int CPDF_Opids::OPID_OPEN_DOCUMENT_ACROBAT = 60210;
 int CPDF_Opids::OPID_SAVE_AS_PDF_DOCUMENT = 60220;
 int CPDF_Opids::OPID_OPEN_APPLICATION = 60100;
 int CPDF_Opids::OPID_OPEN_DOCUMENT = 60200;

