#pragma once

class COutlookMark : public CMarkObject
{
public:
	COutlookMark();
	~COutlookMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	int ExecuteOutlook();

	int PreviewAttachments();
	void ClosePreviewAttachments();

	void PrivateMailOpen();
	void PrivateMailRecv();
	void InsertMailTextByChar(Outlook::_MailItemPtr& pMailItem, std::wstring& strBody, std::wstring& strKey, std::wstring& strText, size_t& nFindIdx);
	void PrivateMailSwitch();
	void PrivateMailView();
	void PrivateShow();
	void PrivatePrepare(int mailCount, const TCHAR **mailWithAttachments);
	void PrivateFillFolder(Outlook::MAPIFolderPtr oFolder, int mailCount, const TCHAR **mailWithAttachments);
	void PrivateClearFolder(Outlook::MAPIFolderPtr oFolder);
	void PrivateClear();
	int MoveCursorToSendButton(Outlook::_MailItemPtr& pMailItem);



	Word::_ApplicationPtr m_pAppWord = nullptr;
	Word::_DocumentPtr m_pDoc = nullptr;

	Excel::_ApplicationPtr m_pAppExcel = nullptr;
	Excel::_WorkbookPtr m_pWorkBook = nullptr;

	PowerPoint::_ApplicationPtr m_pAppPowerPoint = nullptr;
	PowerPoint::_PresentationPtr m_oPresentation;
};


