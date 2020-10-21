#include "stdafx.h"
#include "MarkObject.h"
#include "OutlookMark.h"
#include "AutoItX3_DLL.h"
#include "Mark.h"
#include "MarkFrame.h"
#include "MarkAuplus.h"
#include "MarkAuplusUDF.h"
#include "OfficeRegisterWindow.h"
#include "Outlook_Opids.h"


#pragma comment(lib, "AutoItX3_DLL.lib")
#pragma comment(lib, "MarkFrame.lib")

using namespace Mark;

Outlook::_ApplicationPtr g_ApplicationPtr;

COutlookMark::COutlookMark()
{
	LogInfo(_T("Invoke COutlookMark().."));

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	_tcscpy_s(m_szDescript, _ARRAYSIZE(m_szDescript), _T("Outlook-邮件发送"));
	m_eMarkType = E_MARK_OUTLOOK;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;

	SetMD5PassWd(std::string("e32ddbf53aac7513a2b84acd46e565e5")); //latest md5
	//SetMD5PassWd(std::string("ea32cfd3523f4c04f7d398cca8b0f761"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

COutlookMark::~COutlookMark()
{
	LogInfo(_T("Invoke ~COutlookMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int COutlookMark::init()
{
	destroy();
	SleepAndRecord(2 * dwTimerNode2);

	CoInitialize(NULL);
	 
	TakeMeasurement(  new CCounterContent(COutlook_Opids::DESC_TYPE_MARKER, COutlook_Opids::DESC_OPEN_APPLICATION),
		[&]()->int {
	
		if (FAILED(g_ApplicationPtr.CreateInstance(__uuidof(Outlook::Application))))
		{
			LogError({ _T("Outlook没有安装") });
			return -1;
		}
	return BENCH_MARK_OK; });
	try
	{
		OfficeRegisterWindow wndOfficeReg(this);
		wndOfficeReg.RegisterWindowHandler([&]()->int {
			Outlook::MAPIFolderPtr oInbox;
			Outlook::_NameSpacePtr oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
			oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
			oInbox->Display();
			return BENCH_MARK_OK;
		});
	}
	catch (_com_error &err)
	{
		LogError({ _T("Outlook无法启动"),  err.ErrorMessage() });
		return -2;
	}
	
	return BENCH_MARK_OK;
}

int COutlookMark::run()
{ 
	if (BENCH_MARK_OK != ExecuteOutlook())
	{
		return -1;
	}
 
	return BENCH_MARK_OK;
}

int COutlookMark::destroy()
{
	LogInfo(_T("Invoke COutlookMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\MsOffice\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	//MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	CoUninitialize();
	//MyPlatform::KillProcessFromName(_T("OUTLOOK.EXE"));
	SleepAndRecord(2 * dwTimerNode3);
	//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
	SleepAndRecord(2 * dwTimerNode3);
	//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	SleepAndRecord(2 * dwTimerNode3);
	//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	SleepAndRecord(2 * dwTimerNode3);
	//MyPlatform::KillProcessFromName(_T("chrome.exe"));
	SleepAndRecord(2 * dwTimerNode3);
	return BENCH_MARK_OK;
}




std::tstring lpResource;
extern double dwTimerNode;
extern double dwTimerNode1;
extern double  dwTimerNode2;
extern double  dwTimerNode3;
MyPlatform::tstring FileRead(const TCHAR* lpFilePath) {
	FILE *f;
	_tfopen_s(&f, lpFilePath, TEXT("rb"));
	fseek(f, 0, SEEK_END);
	int flen = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<unsigned char> fcont(flen + 2);
	flen = fread(fcont.data(), 1, flen, f);
	fclose(f);
	TCHAR *lpTString = (TCHAR *)fcont.data();
	fcont[flen] = 0;
	fcont[flen + 1] = 0;
	MyPlatform::tstring stdTString = MyPlatform::tstring((MyPlatform::tstring::const_pointer)fcont.data());
	return stdTString;
};


void COutlookMark::PrivateClearFolder(Outlook::MAPIFolderPtr oFolder) {
	Outlook::_ItemsPtr oItems;
	Outlook::_MailItemPtr oMailItem;
	oItems = oFolder->Items;
	oMailItem = oItems->GetFirst();
	while (oMailItem != 0) {
		oMailItem->Delete();
		SleepAndRecord(2 * 500);
		oMailItem = oItems->GetNext();
	};
};
void COutlookMark::PrivateClear() {
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox, oOutbox, oDrafts, oDeleted;
	oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
	oInbox = oMAPI->GetDefaultFolder(Outlook::olFolderInbox);
	oOutbox = oMAPI->GetDefaultFolder(Outlook::olFolderOutbox);
	oDrafts = oMAPI->GetDefaultFolder(Outlook::olFolderDrafts);
	oDeleted = oMAPI->GetDefaultFolder(Outlook::olFolderDeletedItems);
	PrivateClearFolder(oInbox);
	PrivateClearFolder(oOutbox);
	PrivateClearFolder(oDrafts);
	PrivateClearFolder(oDeleted);
};
Outlook::_MailItemPtr PrivateCreateMail(
	LPCTString sTo, LPCTString sCc, LPCTString sSubject,
	LPCTString sHTMLBody, LPCTString *mailWithAttachments,
	Outlook::OlBodyFormat iBodyFormat = Outlook::OlBodyFormat::olFormatUnspecified,
	Outlook::OlImportance iImportance = Outlook::OlImportance::olImportanceNormal)
{
	Outlook::_MailItemPtr oMail = g_ApplicationPtr->CreateItem(Outlook::olMailItem);
	oMail->To = sTo;
	oMail->CC = sCc;
	oMail->Subject = sSubject;
	oMail->HTMLBody = sHTMLBody;
	//oMail->PutBodyFormat = iBodyFormat;
	//oMail->Importance = iImportance;
	if (mailWithAttachments) {
		Outlook::AttachmentsPtr oAttachments = oMail->Attachments;
		while (LPCTString tstrAttachment = *mailWithAttachments++) {
			oAttachments->Add(_bstr_t(tstrAttachment));
		};
	};
	return oMail;
};
std::wstring replace_s(std::wstring &source, std::wstring finds, std::wstring replaces, int offset = 0)
{
	UINT index = source.find(finds);
	while (index >= 0)
	{
		if (index > source.length()) break;
		index = index - offset;
		source.replace(index, replaces.length(), replaces);
		index = index + replaces.length() + 1;
		index = source.find(finds, index);
	}
	return source;
}

std::wstring insert_s(std::wstring &source, std::wstring finds, std::wstring replaces, int offset = 0)
{
	UINT index = source.find(finds);
	while (index >= 0)
	{
		if (index > source.length()) break;
		index = index + finds.length() - offset;
		source.insert(index, replaces);
		index = index + finds.length() + replaces.length() + 1;
		index = source.find(finds, index);
	}
	return source;
}

void COutlookMark::PrivateFillFolder(Outlook::MAPIFolderPtr oFolder, int mailCount, const TCHAR **mailWithAttachments) {
	Outlook::_MailItemPtr oMail;
	MTStr recvMailAddr0 = TEXT("6668889999@tbjf.com.cn;"),
		recvMailAddr1 = TEXT("6668889999@tbjf.com.cn;"), strSubject, strBody;
	for (Int i = 0;i < mailCount;i++) {
		std::wstring tbody,// = lpResource + TEXT("MsOffice\\Outlook\\Send Mail One.htm"),
			tmrespath = lpResource + TEXT("Resources\\MsOffice\\Outlook\\");
		switch (i)
		{
		case 0:
			tbody = lpResource + TEXT("Resources\\MsOffice\\Outlook\\Send Mail One.htm");
			recvMailAddr0 = TEXT("Xiaoyao.Li@tbjf.com");recvMailAddr1 = TEXT("Xiaoyao.Li@tbjf.com");
			strSubject = TEXT("产品调查报告");
			break;
		case 1:
			tbody = lpResource + TEXT("Resources\\MsOffice\\Outlook\\Send Mail Two.htm");
			strSubject = TEXT("天宝金蚁账户提醒：您已成功注册一个免费账户");
			break;
		case 2:
			tbody = lpResource + TEXT("Resources\\MsOffice\\Outlook\\Send Mail Three.htm");
			strSubject = TEXT("天宝金蚁账户提醒：您的网易邮箱若再不登录将被冻结，请您尽快登录！");
			break;
		}
		tbody = FileRead(tbody.c_str());
		replace_s(tmrespath, TEXT("\\"), TEXT("/"), 0);
		tmrespath.replace(1, 1, TEXT("|"));
		tmrespath.insert(0, TEXT("file:///"));
		insert_s(tbody, TEXT(" src=\"Send"), tmrespath, 4);
		insert_s(tbody, TEXT(" src=\'Send"), tmrespath, 4);

		//strBody.Format(TEXT("A Sample Mail"));
		oMail = PrivateCreateMail(
			recvMailAddr0.C_Str(), recvMailAddr1.C_Str(),
			strSubject.C_Str(), tbody.c_str(), mailWithAttachments);
		oMail->Move(oFolder);
		mailWithAttachments[0] = 0;
	};
};
void COutlookMark::PrivatePrepare(int mailCount, const TCHAR **mailWithAttachments) {
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox;
	oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
	oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
	PrivateFillFolder(oInbox, mailCount, mailWithAttachments);
};


void COutlookMark::PrivateShow() {
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox, oDrafts, oOutbox;
	oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
	oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
	oInbox->Display();
	AuUDF::MsOfficeActivationWizardSkip(TEXT("OUTLOOK\\.EXE"), (Mark::Int)dwTimerNode3, (Mark::Int)dwTimerNode3);
};

void COutlookMark::PrivateMailView() {
	LogInfo(TEXT("预览附件内容"));
	Outlook::_ExplorerPtr oExplorerPtr;
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox, oDrafts, oOutbox;
	oExplorerPtr = g_ApplicationPtr->ActiveExplorer();
	oExplorerPtr->Activate();

	{
		TCHAR szExePath[_MAX_PATH] = { 0 };
		MyPlatform::GetFullPathUnderExe(_T(""), szExePath);
		lpResource = szExePath;
	}
	std::tstring attachpath1, attachpath2, attachpath3;
	attachpath1 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.doc");
	attachpath2 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.xlsx");
	attachpath3 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.ppt");

	oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
	oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
	oDrafts = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderDrafts);
	oOutbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderOutbox);

	oExplorerPtr->CurrentFolder = oInbox;

	/*for (Int i = 1;i <= 2;i++)
	oExplorerPtr->ClearSelection();
	oExplorerPtr->IsItemSelectableInView(oInbox->Items->Item(i));
	oExplorerPtr->AddToSelection(oExplorerPtr->CurrentFolder->Items->Item(i));
	SleepAndRecord(2 * 5000);
	if (i == 2) {
	AuProcess pImage;
	pImage.ExecuteOpen(image0.c_str(), 0, true);
	SleepAndRecord(2 * 5000);
	while(HWND hWnd = AU3_WinGetHandle(L"[class:Photo_Lightweight_Viewer]", L"")){
	AU3_WinCloseByHandle(hWnd);
	};
	//pImage.Terminate(0);
	} else {
	AuProcess pText;
	pText.ExecuteOpen(text1.c_str(), 0, true);
	SleepAndRecord(2 * 3000);
	pText.Terminate(0);
	};
	};*/
	{
		{
			oExplorerPtr->ClearSelection();
			oExplorerPtr->IsItemSelectableInView(oInbox->Items->Item(1));
			oExplorerPtr->AddToSelection(oExplorerPtr->CurrentFolder->Items->Item(1));
			SleepAndRecord(2 * (DWORD)(dwTimerNode3));
			AuProcess pText;
			pText.ExecuteOpen(attachpath1.c_str(), 0, true);
			
			SleepAndRecord(2 * (DWORD)(dwTimerNode3));
			pText.Terminate(0);
		};
		{
			oExplorerPtr->ClearSelection();
			oExplorerPtr->IsItemSelectableInView(oInbox->Items->Item(1));
			oExplorerPtr->AddToSelection(oExplorerPtr->CurrentFolder->Items->Item(1));
			SleepAndRecord(2 * (DWORD)(dwTimerNode3));
			AuProcess pText;
			pText.ExecuteOpen(attachpath2.c_str(), 0, true);
			SleepAndRecord(2 * (DWORD)(dwTimerNode3));
			pText.Terminate(0);
		};
		{
			oExplorerPtr->ClearSelection();
			oExplorerPtr->IsItemSelectableInView(oInbox->Items->Item(1));
			oExplorerPtr->AddToSelection(oExplorerPtr->CurrentFolder->Items->Item(1));
			SleepAndRecord(2 * (DWORD)(dwTimerNode3));
			AuProcess pText;
			pText.ExecuteOpen(attachpath3.c_str(), 0, true);
			SleepAndRecord(2 * (DWORD)(dwTimerNode3));
			pText.Terminate(0);
		};
	}
	{
		oExplorerPtr->ClearSelection();
		oExplorerPtr->IsItemSelectableInView(oInbox->Items->Item(2));
		oExplorerPtr->AddToSelection(oExplorerPtr->CurrentFolder->Items->Item(2));
		SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	}
	{
		oExplorerPtr->ClearSelection();
		oExplorerPtr->IsItemSelectableInView(oInbox->Items->Item(3));
		oExplorerPtr->AddToSelection(oExplorerPtr->CurrentFolder->Items->Item(3));
		SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	}

	oExplorerPtr->ClearSelection();
};
void COutlookMark::PrivateMailSwitch() {

	Outlook::_ExplorerPtr oExplorerPtr;
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox, oDrafts, oOutbox;
	oExplorerPtr = g_ApplicationPtr->ActiveExplorer();
	oExplorerPtr->Activate();
	oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
	oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
	oDrafts = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderDrafts);
	oOutbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderOutbox);
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	oExplorerPtr->CurrentFolder = oDrafts;
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	oExplorerPtr->CurrentFolder = oOutbox;
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	oExplorerPtr->CurrentFolder = oInbox;
};

void COutlookMark::InsertMailTextByChar(Outlook::_MailItemPtr& pMailItem, std::wstring& strBody, std::wstring& strKey, std::wstring& strText, UINT& nFindIdx)
{
	nFindIdx = strBody.find(strKey, nFindIdx);
	nFindIdx += strKey.length();

	int nCharCount = strText.length();
	wchar_t szText[2] = { 0 };
	for (int nCharIdx = 0; nCharIdx < nCharCount; nCharIdx++)
	{
		szText[0] = strText[nCharIdx];
		strBody.insert(nFindIdx, szText);
		pMailItem->HTMLBody = strBody.c_str();
		SleepAndRecord(2 * dwTimerNode);
		nFindIdx++;
	}
}

void COutlookMark::PrivateMailRecv() {
	Outlook::_ExplorerPtr oExplorerPtr;
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox, oDrafts, oOutbox;

	LogInfo(TEXT("发送邮件"));
	TakeMeasurement(new CCounterContent(COutlook_Opids::DESC_TYPE_MARKER, COutlook_Opids::DESC_SEND_MAIL),
		[&]()->int {
	//lpResource = GetSharedParameter(TEXT("dir"), TEXT("resc"));
	{
		TCHAR szExePath[_MAX_PATH] = { 0 };
		MyPlatform::GetFullPathUnderExe(_T(""), szExePath);
		lpResource = szExePath;
	}
	std::tstring attachpath1, attachpath2, attachpath3;
	attachpath1 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.doc");
	attachpath2 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.xlsx");
	attachpath3 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.ppt");

	oExplorerPtr = g_ApplicationPtr->ActiveExplorer();
	oExplorerPtr->Activate();


	Outlook::_MailItemPtr oMail = g_ApplicationPtr->CreateItem(Outlook::olMailItem);
	oMail->Display();
	
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	oMail->To = TEXT("Xiaoyao.Li@tbjf.com");
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	//oMail->CC = TEXT("admin1@a.com;admin2@a.com;");
	//SleepAndRecord(2 * 1500);
	//oMail->CC = TEXT("admin1@a.com;admin2@a.com;admin3@a.com;");
	//SleepAndRecord(2 * 4000);
	oMail->Subject = TEXT("产品调查报告");
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));

	std::wstring tbody1 = lpResource + TEXT("Resources\\MsOffice\\Outlook\\Product requirement.htm"),
		tmrespath1 = lpResource + TEXT("Resources\\MsOffice\\Outlook\\");
	tbody1 = FileRead(tbody1.c_str());
	replace_s(tmrespath1, TEXT("\\"), TEXT("/"), 0);
	tmrespath1.replace(1, 1, TEXT("|"));
	tmrespath1.insert(0, TEXT("file:///"));
	//insert_s(tbody1,TEXT(" src=\'Product"),tmrespath1,7);

	oMail->HTMLBody = tbody1.c_str();

	std::wstring _mkey = TEXT(""),
		_mvalue = TEXT("");
	UINT _findex = 0;

	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content1'>");
		_mvalue = TEXT("李总");
		_findex = tbody1.find(_mkey);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content2'>");
		_mvalue = TEXT("您好，以下是公司天宝金蚁业务的调查分析报告:");
		InsertMailTextByChar(oMail, tbody1, _mkey, _mvalue, _findex);
// 		_findex = tbody1.find(_mkey, _findex);
// 		tbody1.insert(_findex + _mkey.length(), _mvalue);
// 		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content3'>");
		_mvalue = TEXT("1)&nbsp; 采集了核心用户的基本信息：从有效问卷中采集的重点用户和潜在用户的基本信息有助于我们更好地把握金蚁产品在目标市场的基本定位与主要用户的分布情况，有助于今后市场工作的开展和推进。");
		InsertMailTextByChar(oMail, tbody1, _mkey, _mvalue, _findex);
// 		_findex = tbody1.find(_mkey, _findex);
// 		tbody1.insert(_findex + _mkey.length(), _mvalue);
// 		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content4'>");
		_mvalue = TEXT("2)&nbsp; 获得了有价值的统计数据：在对有效问卷进行统计、分析的基础上，我们获得了有关用户认知度和满意度的基本数据，数据分布符合统计规律，数据样本真实、有效。");
		InsertMailTextByChar(oMail, tbody1, _mkey, _mvalue, _findex);
// 		_findex = tbody1.find(_mkey, _findex);
// 		tbody1.insert(_findex + _mkey.length(), _mvalue);
// 		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content5'>");
		_mvalue = TEXT("3)&nbsp; 为公司决策层提供了有价值的调查结论：在基本数据的基础上，我们得出了若干调查结论，这些调查结论可以作为公司决策层的参考，为今后公司的产品研发、市场宣传和商业运作提供较为准确、合理的依据。");
		InsertMailTextByChar(oMail, tbody1, _mkey, _mvalue, _findex);
// 		_findex = tbody1.find(_mkey, _findex);
// 		tbody1.insert(_findex + _mkey.length(), _mvalue);
// 		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("code_show_content6");
		_mvalue = TEXT("样板数据如下:");
		_findex = tbody1.find(_mkey, _findex);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_table'>");
		_mvalue += TEXT("<table border=1 cellpadding=0 cellspacing=0 style='height: 188px; width: 900px; border: thin solid #000000;'><tr>");
		_mvalue += TEXT("<td  colspan='3'>年龄分布表</td><td  rowspan='3'>&nbsp;</td>");
		_mvalue += TEXT("<td  colspan='3'>地区分布表</td></tr><tr><td >");
		_mvalue += TEXT("18-30</td><td >");
		_mvalue += TEXT("30-50</td><td >");
		_mvalue += TEXT("50以上</td>");
		_mvalue += TEXT("<td >");
		_mvalue += TEXT("乡镇</td>");
		_mvalue += TEXT("<td >");
		_mvalue += TEXT("中型城市</td>");
		_mvalue += TEXT("<td >");
		_mvalue += TEXT("大型城市</td></tr><tr>");
		_mvalue += TEXT("<td >14700</td><td >60000</td><td >");
		_mvalue += TEXT("25300</td><td >19600</td>");
		_mvalue += TEXT("<td >26700</td><td >54300</td></tr></table>");
		_findex = tbody1.find(_mkey, _findex);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content7'>");
		_mvalue = TEXT("详细内容已发送至附件");
		_findex = tbody1.find(_mkey, _findex);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content8'>");
		_mvalue = TEXT("项目负责人");
		_findex = tbody1.find(_mkey, _findex);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_content9'>");
		_mvalue = TEXT("刘廿");
		_findex = tbody1.find(_mkey, _findex);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	{
		_mkey.clear();
		_mvalue.clear();
		_mkey = TEXT("id='code_show_image'>");
		_mvalue = TEXT("<img src='") + tmrespath1 + TEXT("Product%20requirement.files/logo.png' />");
		_findex = tbody1.find(_mkey);
		tbody1.insert(_findex + _mkey.length(), _mvalue);
		oMail->HTMLBody = tbody1.c_str();
	}

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	oMail->Attachments->Add(_bstr_t(attachpath1.c_str()));
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	oMail->Attachments->Add(_bstr_t(attachpath2.c_str()));
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	oMail->Attachments->Add(_bstr_t(attachpath3.c_str()));

	SleepAndRecord(2 * (DWORD)(dwTimerNode3));

	MoveCursorToSendButton(oMail);

	oMail->Delete();
	//oMail->Send();
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));


	oMail = g_ApplicationPtr->CreateItem(Outlook::olMailItem);
	oMail->Display();
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	oMail->To = TEXT("6668889999@tbjf.com.cn;");
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));
	oMail->Subject = TEXT("天宝金蚁账户提醒：您已成功注册一个免费账户");
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));

	std::wstring tbody2 = lpResource + TEXT("Resources\\MsOffice\\Outlook\\Send Mail Two.htm"),
		tmrespath2 = lpResource + TEXT("Resources\\MsOffice\\Outlook\\");
	tbody2 = FileRead(tbody2.c_str());
	replace_s(tmrespath2, TEXT("\\"), TEXT("/"), 0);
	tmrespath2.replace(1, 1, TEXT("|"));
	tmrespath2.insert(0, TEXT("file:///"));
	insert_s(tbody2, TEXT(" src=\"Send"), tmrespath2, 4);

	oMail->HTMLBody = tbody2.c_str();
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));

	MoveCursorToSendButton(oMail);

	oMail->Delete();
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));


	oMail = g_ApplicationPtr->CreateItem(Outlook::olMailItem);
	oMail->Display();
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	oMail->To = TEXT("6668889999@tbjf.com.cn;");
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	oMail->Subject = TEXT("天宝金蚁账户提醒：您的网易邮箱若再不登录将被冻结，请您尽快登录！");
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));

	std::wstring tbody3 = lpResource + TEXT("Resources\\MsOffice\\Outlook\\Send Mail Three.htm"),
		tmrespath3 = lpResource + TEXT("Resources\\MsOffice\\Outlook\\");
	tbody3 = FileRead(tbody3.c_str());
	replace_s(tmrespath3, TEXT("\\"), TEXT("/"), 0);
	tmrespath3.replace(1, 1, TEXT("|"));
	tmrespath3.insert(0, TEXT("file:///"));
	insert_s(tbody3, TEXT(" src=\"Send"), tmrespath3, 4);
	oMail->HTMLBody = tbody3.c_str();
	SleepAndRecord(2 * (DWORD)(dwTimerNode3));

	MoveCursorToSendButton(oMail);

	oMail->Delete();
	
	SleepAndRecord(2 * (DWORD)(dwTimerNode2));
	return BENCH_MARK_OK;
	});
};
void COutlookMark::PrivateMailOpen() {
	LogInfo(TEXT("打开邮件"));
	Outlook::_ExplorerPtr oExplorerPtr;
	Outlook::_NameSpacePtr oMAPI;
	Outlook::MAPIFolderPtr oInbox, oDrafts, oOutbox;

	TakeMeasurement(new CCounterContent(COutlook_Opids::DESC_TYPE_MARKER, COutlook_Opids::DESC_OPEN_MAIL),
		[&]()->int {
	oExplorerPtr = g_ApplicationPtr->ActiveExplorer();
	oExplorerPtr->Activate();
	oMAPI = g_ApplicationPtr->GetNamespace("MAPI");
	oInbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderInbox);
	oDrafts = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderDrafts);
	oOutbox = oMAPI->GetDefaultFolder(Outlook::OlDefaultFolders::olFolderOutbox);

	Outlook::_ItemsPtr oItems;
	Outlook::_MailItemPtr oMailItem;
	oItems = oInbox->Items;
	oMailItem = oItems->GetFirst();
	Int i = 0;
	while (oMailItem != 0 && i < 9) {
		oMailItem->Display(false);
		oMailItem = oItems->GetNext();
		i++;
	};
	while (g_ApplicationPtr->Inspectors->Count < i) {
		SleepAndRecord(2 * (DWORD)(dwTimerNode));
	};
	i = 0;
	while (oMailItem != 0 && i < 9) {
		oMailItem->Close(Outlook::OlInspectorClose::olDiscard);
		oMailItem = oItems->GetNext();
		i++;
		SleepAndRecord(2 * (DWORD)(dwTimerNode1));
	};
	return BENCH_MARK_OK;
	});
};
int COutlookMark::ExecuteOutlook() {
	//ILog *lpLog = GetController()->GetTipsLog();
	//lpResource = GetSharedParameter(TEXT("dir"), TEXT("resc"));
	{
		TCHAR szExePath[_MAX_PATH] = { 0 };
		MyPlatform::GetFullPathUnderExe(_T(""), szExePath);
		lpResource = szExePath;
	}

	std::tstring attachpath1, attachpath2, attachpath3;
	attachpath1 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.doc");
	attachpath2 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.xlsx");
	attachpath3 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.ppt");
	const TCHAR* lpAttachments[] = {
		attachpath1.c_str(),
		attachpath2.c_str(),
		attachpath3.c_str(),
		0,
	};

	try {
		TakeMeasurement( new CCounterContent(COutlook_Opids::DESC_TYPE_MARKER, COutlook_Opids::DESC_OPEN_DOCUMENT),
			[&]()->int {
			OfficeRegisterWindow wndOfficeReg(this);
			wndOfficeReg.RegisterWindowHandler([&]()->int {
				LogInfo(_T("Outlook::启动程序"));
				PrivateClear();
				LogInfo(_T("Outlook::准备..."));
				PrivatePrepare(3, lpAttachments);
				PrivateShow();
				// MailView
				LogInfo(TEXT("打开网页"));
				return BENCH_MARK_OK;
			});
		return BENCH_MARK_OK; });
		//lpLog->T(TEXT("Outlook"), TEXT("开始计时"));
		Double fBegin = AuTimer().TimerInit();

		LogInfo(_T("Outlook::预览附件内容"));
		if (BENCH_MARK_OK != PreviewAttachments())
		{
			LogError({ _T("运行发邮件预览发生错误\r\n") });
			//return -1;
		}
		//PrivateMailView();
		SleepAndRecord(2 * dwTimerNode3);
// 		LogInfo(_T("Outlook::激活邮件"));
// 		PrivateMailSwitch();
// 		SleepAndRecord(2 * dwTimerNode3 * 2);
		LogInfo(_T("Outlook::发送邮件"));
		PrivateMailRecv();
		//SleepAndRecord(2 * dwTimerNode3 * 2);
		//PrivateMailOpen();
		//SleepAndRecord(2 * dwTimerNode3 * 2);
		//PrivateMailSwitch();
		//Double fEnd = AuTimer().TimerDiff(fBegin);
		//lpLog->T(TEXT("Outlook"), TEXT("终止计时"));
		//m_ResultsShared[TEXT("Outlook")].SetData(fEnd);

// 		SleepAndRecord(2 * dwTimerNode3);
// 		PrivateClear();
// 		while (true) {
// 			Outlook::_ExplorerPtr oExplorerPtr = g_ApplicationPtr->ActiveWindow();
// 			if (oExplorerPtr) {
// 				oExplorerPtr->Close();
// 			}
// 			else {
// 				break;
// 			};
// 		};

		ClosePreviewAttachments();

		g_ApplicationPtr->Quit();
		g_ApplicationPtr->Release();
		g_ApplicationPtr = NULL;
	}
	catch (_com_error &err) {
		LogError({ _T("运行发邮件时出现异常"),  err.ErrorMessage() });
		return -2;
	};
	

	return BENCH_MARK_OK;
};

int COutlookMark::MoveCursorToSendButton(Outlook::_MailItemPtr& pMailItem)
{
	CString strWindwowName;
	HWND hWnd = NULL;

	strWindwowName.Format(_T("%s - 邮件 (HTML) "), (const  TCHAR*)pMailItem->Subject);
	hWnd = ::FindWindow(_T("rctrl_renwnd32"), strWindwowName.GetBuffer(0));
	if (NULL != hWnd)
	{
		CRect rectWindow;
		CPoint pointCursor;
		
		GetWindowRect(hWnd, rectWindow);
		pointCursor = rectWindow.TopLeft();
		pointCursor.Offset(CSize(40, 230));
		SetCursorPos(pointCursor.x, pointCursor.y);
	}

	SleepAndRecord(2 * dwTimerNode2);
	SetCursorPos(0, 0);
	return BENCH_MARK_OK;
}

int COutlookMark::PreviewAttachments()
{
	std::tstring attachpath1 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.doc");
	std::tstring attachpath2 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.xlsx");
	std::tstring attachpath3 = lpResource + TEXT("Resources\\MsOffice\\产品调查报告.ppt");

	try
	{
		Word::_ApplicationPtr pApp;
		TakeMeasurement( new CCounterContent(COutlook_Opids::DESC_TYPE_MARKER, COutlook_Opids::DESC_PREVIEW_ATTACHMENTS),
			[&]()->int {
			OfficeRegisterWindow wndOfficeReg(this);
			wndOfficeReg.RegisterWindowHandler([&]()->int {
				//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
				SleepAndRecord(2 * dwTimerNode1);
				//Word::_ApplicationPtr pApp = NULL;

				HRESULT hr = m_pAppWord.CreateInstance(__uuidof(Word::Application));
				pApp = m_pAppWord;
				if (FAILED(hr))
				{
					LogError({ _T("WINWORD.exe start error inside Outlook..") });
					return -2;
				}
				return BENCH_MARK_OK;
			});
			pApp->Visible = VARIANT_TRUE;
			pApp->WindowState = Word::WdWindowState::wdWindowStateMaximize;
			Word::_DocumentPtr pDoc = pApp->Documents->Open(&(_variant_t(attachpath1.c_str())));
			m_pDoc = pDoc;
			SleepAndRecord(5 * dwTimerNode3);
			  
			for (int nIdx = 0; nIdx < 5; nIdx++)
			{
				pApp->ActiveWindow->SmallScroll(&_variant_t(12));
				SleepAndRecord(2 * dwTimerNode1);
			}
	
 
			SleepAndRecord(2 * dwTimerNode1);
			{
				//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
				OfficeRegisterWindow wndOfficeReg(this); 
				Excel::_ApplicationPtr pApp = NULL;
				wndOfficeReg.RegisterWindowHandler([&]()->int {
					
					HRESULT hr = m_pAppExcel.CreateInstance(__uuidof(Excel::Application));
					pApp = m_pAppExcel;
					if (FAILED(hr))
					{
						LogError({ _T("Excel.exe start error inside Outlook..") });
						return -2;
					}
				
					pApp->PutVisible(0, VARIANT_TRUE);
					Excel::_WorkbookPtr pWorkBook = pApp->Workbooks->Open(_bstr_t(attachpath2.c_str()));
					m_pWorkBook = pWorkBook;
					pApp->WindowState[0] = Excel::XlWindowState::xlMaximized;
					return BENCH_MARK_OK;
				}); 
				SleepAndRecord(5 * dwTimerNode3);
  
				for (int nIdx = 0; nIdx < 5; nIdx++)
				{
					pApp->ActiveWindow->SmallScroll(&_variant_t(2));
					SleepAndRecord(2 * dwTimerNode1);
				}
			}
 
		{
			//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
			SleepAndRecord(2 * dwTimerNode1);
			OfficeRegisterWindow wndOfficeReg(this); 
			PowerPoint::_ApplicationPtr pApp = NULL;
			PowerPoint::_PresentationPtr oPresentation;
			wndOfficeReg.RegisterWindowHandler([&]()->int {
				HRESULT hr = m_pAppPowerPoint.CreateInstance(__uuidof(PowerPoint::Application));
				pApp = m_pAppPowerPoint;
				if (FAILED(hr))
				{
					LogError({ _T("POWERPNT.exe start error inside Outlook..") });
					return -2;
				}
			pApp->Visible = Office::msoTrue;
			oPresentation = pApp->Presentations->Open(_bstr_t(attachpath3.c_str()), Office::MsoTriState::msoFalse, Office::MsoTriState::msoFalse, Office::MsoTriState::msoTrue);
			return BENCH_MARK_OK; 
			});
			m_oPresentation = oPresentation;
			pApp->WindowState = PowerPoint::PpWindowState::ppWindowMaximized;


			SleepAndRecord(5 * dwTimerNode3);
		 
			PowerPoint::SlidesPtr oSlides = oPresentation->Slides;
			int nSlides = oSlides->Count, nLimit = 3;
			for (int i = 1; i <= nSlides && i <= nLimit; i++) {
				
				VARIANT var;
				VariantInit(&var);
				oSlides->raw__Index(i, &var);
				PowerPoint::_SlidePtr oSlide = var.pdispVal;
				oSlide->Select();
				VariantClear(&var);
				SleepAndRecord(2 * dwTimerNode2);
			}
			oSlides->Release();
			oSlides = NULL;

 			SleepAndRecord(2 * dwTimerNode2);
		}
		return BENCH_MARK_OK;
	});

	}
	catch (_com_error& err)
	{
		LogError({ _T("Outlook:邮件发送预览附件失败"), err.ErrorMessage() });
		return -1;
	}

	return BENCH_MARK_OK;
}

void COutlookMark::ClosePreviewAttachments()
{	
	//Word Close.
	m_pDoc->Close(&_variant_t(Word::wdDoNotSaveChanges));
	m_pDoc->Release();
	m_pDoc = NULL;
	SleepAndRecord(2 * dwTimerNode1);
	m_pAppWord->Quit();
	SleepAndRecord(2 * dwTimerNode1);
	m_pAppWord->Release();
	m_pAppWord = NULL;
	//MyPlatform::KillProcessFromName(_T("WINWORD.EXE"));
	SleepAndRecord(2 * dwTimerNode1);

	//Excel Close.
	m_pWorkBook->Close(&_variant_t(Excel::xlDoNotSaveChanges));
	m_pWorkBook->Release();
	m_pWorkBook = NULL;
	SleepAndRecord(2 * dwTimerNode1);
	m_pAppExcel->Quit();
	SleepAndRecord(2 * dwTimerNode1);
	m_pAppExcel->Release();
	m_pAppExcel = NULL;
	//MyPlatform::KillProcessFromName(_T("EXCEL.EXE"));
	SleepAndRecord(2 * dwTimerNode1);

	//PPT close.
	m_oPresentation->Close();
	m_oPresentation->Release();
	m_oPresentation = NULL;
	SleepAndRecord(2 * dwTimerNode1);
	m_pAppPowerPoint->Quit();
	SleepAndRecord(2 * dwTimerNode1);
	m_pAppPowerPoint->Release();
	m_pAppPowerPoint = NULL;
	//MyPlatform::KillProcessFromName(_T("POWERPNT.EXE"));
	SleepAndRecord(2 * dwTimerNode1);
}

