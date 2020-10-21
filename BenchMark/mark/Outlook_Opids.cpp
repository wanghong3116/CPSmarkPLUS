#include "stdafx.h"
#include "Outlook_Opids.h"


COutlook_Opids::COutlook_Opids()
{
 	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_APPLICATION, DESC_OPEN_APPLICATION));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_DOCUMENT, DESC_OPEN_DOCUMENT));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_OPEN_MAIL, DESC_OPEN_MAIL));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_SEND_MAIL, DESC_SEND_MAIL));
	m_opid_map.insert(std::pair<int, std::tstring>(OPID_PREVIEW_ATTACHMENTS, DESC_PREVIEW_ATTACHMENTS));
}

COutlook_Opids::~COutlook_Opids()
{
}
tstring COutlook_Opids::DESC_TYPE_MARKER = _T("Outlook");
tstring COutlook_Opids::DESC_OPEN_APPLICATION = _T("打开程序(Open Outlook Application)");
tstring COutlook_Opids::DESC_OPEN_DOCUMENT = _T("打开文档(Open Outlook Folder)");
tstring COutlook_Opids::DESC_OPEN_MAIL = _T("Outlook::打开邮件(Open Mail)");
tstring COutlook_Opids::DESC_SEND_MAIL = _T("Outlook::发送邮件(Send Mail)");
tstring COutlook_Opids::DESC_PREVIEW_ATTACHMENTS = _T("Outlook::预览附件内容(Preview Attachments)");

int COutlook_Opids::OPID_TYPE_MARKER = 90000;
int COutlook_Opids::OPID_OPEN_APPLICATION = 90100;
int	COutlook_Opids::OPID_OPEN_DOCUMENT = 90200;
int COutlook_Opids::OPID_OPEN_MAIL = 90300;
int COutlook_Opids::OPID_SEND_MAIL = 90400;
int COutlook_Opids::OPID_PREVIEW_ATTACHMENTS = 90500;
