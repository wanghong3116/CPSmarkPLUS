// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "WorkMark.h"
#include "MarkObject.h"
#include "PremiereMark.h"
#include "Photoshop.h"
#include "ChromeMark.h"
#include "OfficeWordMark.h"
#include "OfficeExcelMark.h"
#include "OfficePowerPointMark.h"
//#include "ZipMark.h"
#include "VideoPlayer.h"
#include "AEMark.h"
#include "HandBrake.h"
//#include "OpenCL.h"
#include "3DSMax.h"
#include "Resource.h"
#include "WinrarZipMark.h"
#include "PDFMark.h"
#include "3DSMaxWhale.h"
//#include "3DSMaxCar.h"
//#include "3DSMaxHome.h"
#include "ResultDetailUI.h"
#include "AutoCADMark.h"
#include "OutlookMark.h"
//#include "AntiVirus.h"
#include "InformationTrace.h"
#include "OfficeRegisterWindow.h"
#include "MainTabUI.h"
#include <future>
#include "ResourceVerify.h"
#define IDT_TIME_CPU_UTILIZATION 1001
#include "Chrome_Opids.h"
#include "AutoCAD_Opids.h"
#include "AE_Opids.h"
#include "3DSMaxWhale_Opids.h"
#include "Excel_Opids.h"
#include "HandBrake_Opids.h"
#include "Outlook_Opids.h"
#include "PDF_Opids.h"
#include "PhotoShop_Opids.h"
#include "PPT_Opids.h"
#include "Premiere_Opids.h"
#include "Winrar_Opids.h"
#include "Word_Opids.h"
//------------------------------------------------------------------------------
std::wostream& wendl(std::wostream& out)
{
	// needed for binary mode files to retain Windows-style newline "\x0D\x0A"
	return out.put(L'\r').put(L'\n').flush();
}//wendl
 
 //------------------------------------------------------------------------------
 
int SetAutoRun()
{
	const TCHAR* c_szRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	TCHAR szFilePath[MAX_PATH] = { 0 };
	TCHAR szStartFP[_MAX_PATH * 2] = { 0 };
	HKEY hKey = NULL;

	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	_stprintf_s(szStartFP, _T("\"%s\" %s"), szFilePath, REBOOT_RUN_COMPREHENSIVE_CALC_ARGUEMENT);
	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, c_szRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		return -1;
		
	}
	RegSetValueEx(hKey, REBOOT_START_REG_NAME, 0, REG_SZ, (LPBYTE)szStartFP, (_tcslen(szStartFP) + 1) * sizeof(TCHAR));
	RegCloseKey(hKey);
	return 0;

}

int ReSetAutoRun()
{
	HKEY hKey = NULL;
	const TCHAR* c_szRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, c_szRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		return -1;
	}

	RegDeleteValue(hKey, REBOOT_START_REG_NAME);
	RegCloseKey(hKey);
	return 0;
}

map<int, CCounterContent*> CWorkMark::m_total_content_map = {};
int CWorkMark::cur_row = 1, CWorkMark::cur_col = 1;
CWorkMark::CWorkMark()
{
}

CWorkMark::~CWorkMark()
{
	WaitForWorkThread();
}

int CWorkMark::SetCSVFileName(const TCHAR* szCSVFileName)
{

	_tcscpy_s(m_szCSVFileName, szCSVFileName);
	MyPlatform::CInformationTrace::m_BaseLogFileName = m_szCSVFileName;
	return 0;
}

int CWorkMark::SetProjectName(const TCHAR* szPrjName)
{
	_tcscpy_s(m_szProjectName, _ARRAYSIZE(m_szProjectName), szPrjName);
	return BENCH_MARK_OK;
}

int CWorkMark::SetHWnd(HWND hWnd)
{
	m_hWnd = hWnd;
	return BENCH_MARK_OK;
}

int CWorkMark::SetMarkClassifySelect(bool pbMCSelect[E_MARK_CLASSIFY_END])
{
	memcpy(m_pbMarkClassifySelect, pbMCSelect, E_MARK_CLASSIFY_END * sizeof(bool));

	return BENCH_MARK_OK;
}

int CWorkMark::SetMarkClassifyItemSelect(EMarkClassify eMarkClassify, bool bSelect)
{
	if (eMarkClassify < E_MARK_CLASSIFY_BEG || eMarkClassify >= E_MARK_CLASSIFY_END)
	{
		return -1;
	}

	m_pbMarkClassifySelect[eMarkClassify] = bSelect;
	return BENCH_MARK_OK;
}

void CWorkMark::SetRunNumber(int nLoopRunTime, int currentRunNumber)
{
	m_nLoopRunTime = nLoopRunTime;
	m_currentRunNumber = currentRunNumber;
}

const std::vector<MarkWorkRunningInfor>* CWorkMark::GetMarkRunInfor()
{
	if (!m_bExitThreadMark)
	{
		return NULL;
	}
	
	return &m_vMarkWorkRunInfor;
}

int CWorkMark::SetRunMarkStatus(ERunMarkStatus eRMStatus)
{
	m_eRunMarkStatus = eRMStatus;
	return 0;
}

int CWorkMark::StartWork(bool bCurrentRunIsComprehensiveApplication)
{
	m_bExitThreadMark = FALSE;
	m_bCurrentRunIsComprehensiveApplication = bCurrentRunIsComprehensiveApplication;
	m_hThreadMark = CreateThread(NULL, 0, ThreadProcMarkWork, this, 0, NULL);
	if (NULL == m_hThreadMark)
	{
		MessageBox(m_hWnd, _T("工作线程开辟失败，请重新运行"), _T("提示"), MB_OK);
		return -1;
	}
	return BENCH_MARK_OK;
}

int CWorkMark::WaitForWorkThread()
{
	m_bExitThreadMark = TRUE;

	if (NULL != m_hThreadMark)
	{
		WaitForSingleObject(m_hThreadMark, INFINITE);
		CloseHandle(m_hThreadMark);
		m_hThreadMark = NULL;
	}
	return BENCH_MARK_OK;
}

bool CWorkMark::IsWorking()
{
	return m_hThreadMark != NULL;
}

HANDLE CWorkMark::GetThreadHandle()
{
	return m_hThreadMark;
}

DWORD WINAPI CWorkMark::ThreadProcMarkWork(_In_ LPVOID lpParameter)
{
	CWorkMark* pThis = (CWorkMark*)lpParameter;
	return pThis->MarkWork();
}

DWORD CWorkMark::MarkWork()
{
	m_total_content_map.clear();

	std::vector<CMarkObject*> vpMarkObject;

	MarkWorkInit(vpMarkObject);

/**/if (!CResourceVerify::GetInstance()->VarifyResourceFile(vpMarkObject))
	{
		PostMessage(m_hWnd, WS_MARK_WORK_OVER, (WPARAM)1, (LPARAM)new MyPlatform::tstring(_T("校验文件错误，资源被修改")));
		 
		return 0;
	}
/**/	
	MarkWork(vpMarkObject);
	 
	return BENCH_MARK_OK;
}
DWORD CWorkMark::MarkWork(std::vector<CMarkObject*>& vpMarkObject)
{
	int nMarkCount = 0;
	
	nMarkCount = vpMarkObject.size();
	SendMessage(m_hWnd, WS_MARK_WORK_PROGRESS, 0, nMarkCount);
	for (int nMarkIdx = 0; nMarkIdx < nMarkCount && !m_bExitThreadMark; nMarkIdx++)
	{
		MarkWorkRunningInfor mwri = { GetCurrentnRunningTime(vpMarkObject[nMarkIdx]->getMarkType()) };
		mwri.nRunningTime.push_back(0);
		SendMessage(m_hWnd, WS_MARK_WORK_PROGRESS, 1, nMarkIdx);

		while (m_eRunMarkStatus != E_RMS_RUN)
		{
			Sleep(200);
		}

		if (NULL == vpMarkObject[nMarkIdx])
		{
			continue;
		}
		if (!m_pbMarkClassifySelect[vpMarkObject[nMarkIdx]->getMarkClassify()])
		{
			continue;
		}
		
		SendMarkTypeNameToWindow(vpMarkObject[nMarkIdx]->getDescript());
		LogInfo({ _T("执行内容："),  vpMarkObject[nMarkIdx]->getDescript() });
		
		if (!vpMarkObject[nMarkIdx]->UnzipResourceFile())
		{
			LogInfo({ _T("验证并解压资源文件失败... "),
				_T("错误原因:"),
				_T("-------------------------------------------- ") });
			continue;
		}

		/*	if (!vpMarkObject[nMarkIdx]->VerifyAndUnzipResourceFile())
		{
			LogInfo({ _T("验证并解压资源文件失败... ")\
				_T("错误原因:")\
				_T("-------------------------------------------- "),
				vpMarkObject[nMarkIdx]->getLastError() });
			continue;
		}
		*/
		LogInfo({ _T("测试初始化中... "), vpMarkObject[nMarkIdx]->getDescript() });
		if (BENCH_MARK_OK != vpMarkObject[nMarkIdx]->init())
 		{
			m_error = 1;
			LogInfo({ _T("初始化程序失败... ") });
 			SDELETE(vpMarkObject[nMarkIdx]);
 			break;
 		}
		LogInfo({ _T("测试初始化成功...."), vpMarkObject[nMarkIdx]->getDescript() });
		Sleep(100);
		LogInfo({ _T("测试程序执行中... ") });
		try
		{
			if (BENCH_MARK_OK != vpMarkObject[nMarkIdx]->run())
			{
				m_error = 1;
				LogInfo({ _T("运行中出错。请查看log。终止运行。") });
				SDELETE(vpMarkObject[nMarkIdx]);
				break;
			}
			LogInfo({ _T("测试运行成功...."), vpMarkObject[nMarkIdx]->getDescript() });
			for each (auto iter in vpMarkObject[nMarkIdx]->m_content_map)
			{
				mwri.nRunningTime[m_currentRunNumber-1] += (int)iter.second->m_measurement;
				m_total_content_map.emplace(iter);
			}
			vpMarkObject[nMarkIdx]->m_content_map.erase(vpMarkObject[nMarkIdx]->m_content_map.begin(), vpMarkObject[nMarkIdx]->m_content_map.end());
			
			mwri.eMarkType = vpMarkObject[nMarkIdx]->getMarkType();
			mwri.eMarkClassify = vpMarkObject[nMarkIdx]->getMarkClassify();
			m_vMarkWorkRunInfor.push_back(mwri);
			LogInfo({ _T("eMarkType:"),  MyPlatform::Int64ToStringW(mwri.eMarkType) });
			LogInfo({ _T("eMarkClassify:"),  MyPlatform::Int64ToStringW(mwri.eMarkClassify) });
			LogInfo({ _T("执行时间:"),  MyPlatform::Int64ToStringW(mwri.nRunningTime[m_currentRunNumber-1]) });
			 
			vpMarkObject[nMarkIdx]->destroy();
		}
		catch (exception ex)
		{
			LogInfo({ _T("运行中出现异常。终止运行。"), MyPlatform::CharToTchar(ex.what()) });
			Sleep(100); 
			break;
		}
		SDELETE(vpMarkObject[nMarkIdx]);
	} 		
	ProcessCounterInfo(); //POSSIBLY ADD COMMENT OUT LOGIC IF DEBUG MODE=TRUE
	
	PostMessage(m_hWnd, WS_MARK_WORK_OVER, 0, 0);
	return BENCH_MARK_OK;
}

int CWorkMark::MarkWorkInit(std::vector<CMarkObject*>& vpMarkObject)
{
	m_vMarkWorkRunInfor.clear();

	if (MyPlatform::CInformationTrace::IsInTestMode == 1)
	{
		m_veAppMarkRun.push_back(E_MARK_NET);
	 }
	else
	{
		m_veAppMarkRun.push_back(E_MARK_NET);
	 	m_veAppMarkRun.push_back(E_MARK_OUTLOOK);
		m_veAppMarkRun.push_back(E_MARK_OFFICE_POWERPOINT);
		m_veAppMarkRun.push_back(E_MARK_OFFICE_WORD);
		m_veAppMarkRun.push_back(E_MARK_OFFICE_EXCEL);
		m_veAppMarkRun.push_back(E_MARK_PDF);
		m_veAppMarkRun.push_back(E_MARK_PHOTOSHOP);
		m_veAppMarkRun.push_back(E_MARK_WINRAR);
		m_veAppMarkRun.push_back(E_MARK_CAD);
	}
 	if (m_bCurrentRunIsComprehensiveApplication)
	{
		if (MyPlatform::CInformationTrace::IsInTestMode == 1)
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_NET, (CChromeMark*)NULL, new CChrome_Opids());
		else
		{
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_NET, (CChromeMark*)NULL, new CChrome_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OUTLOOK, (COutlookMark*)NULL, new COutlook_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OFFICE_EXCEL, (COfficeExcelMark*)NULL, new CExcel_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OFFICE_POWERPOINT, (COfficePowerPointMark*)NULL, new CPPT_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OFFICE_WORD, (COfficeWordMark*)NULL, new CWord_Opids());
			//NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_OPNCL, (COpenCLMark*)NULL);
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_PDF, (CPDFMark*)NULL, new CPDF_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_PHOTOSHOP, (CPhotoshopMark*)NULL, new CPhotoShop_Opids());
			//NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_VIDEOPLAYER, (CVideoPlayerMark*)NULL);
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_WINRAR, (CWinrarZipMark*)NULL, new CWinrar_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_CAD, (CAutoCADMark*)NULL, new CAutoCAD_Opids());
			//NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_ANTIVIRUS, (CAntiVirusMark*)NULL);
		}
	}
	else
	{
		ReSetAutoRun();
		if (MyPlatform::CInformationTrace::IsInTestMode == 1)
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_HANDBRAKE, (CHandBrakeMark*)NULL, new CHandBrake_Opids());
		else
		{
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_3DSMAX_WHALE, (C3DSMaxWhaleMark*)NULL, new C3DSMaxWhale_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_AE, (CAEMark*)NULL, new CAE_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_PREMIERE, (CPremiereMark*)NULL, new CPremiere_Opids());
			NewMarkObjectAndAdd(vpMarkObject, CFG_MARK_HANDBRAKE, (CHandBrakeMark*)NULL, new CHandBrake_Opids());
		}
		Sleep(100 * 2);
	}
	return BENCH_MARK_OK;
}
 
int CWorkMark::LogInfoToExcel(std::wofstream&	outfile, vector<tstring> szContent)
{
	tstring szAll;
	for each (auto s in szContent)
 	 	szAll += s + _T("\t");
  	cur_row++;
	cur_col = 1;
	outfile << szAll << endl;
	return BENCH_MARK_OK;
}

int CWorkMark::ProcessCounterInfo()
{
	try
	{
		TCHAR szRunningInforFP[_MAX_PATH] = { 0 };
		MyPlatform::GetFullPathUnderExe(MyPlatform::ResultsFolder.c_str(), szRunningInforFP);
		MyPlatform::CreateFullDirectory(szRunningInforFP);
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		tstring filename = tstring(szRunningInforFP).append(_T("\\")).append(m_szCSVFileName).append(_T("\\")).append(m_szCSVFileName).append(_T(".csv"));
		LogInfo({ filename });

		wcvt_utf16le_bom cvt(1);
		std::wofstream	outfile;
		std::locale wloc(outfile.getloc(), &cvt);
		outfile.imbue(wloc);
		outfile.open(filename, std::wofstream::out | std::ofstream::app | std::wofstream::binary);
		if (m_currentRunNumber == 1 && CMainTabUI::m_StartupRunFromUI)
		{
			LogInfoToExcel(outfile, { _T("Run"), _T("Error"), _T("unique opid"), _T("scriptname"), _T("timestamp"), _T("measurement"), _T("description") });
		}
		map<ULONGLONG, vector<tstring>> map_output;
		for (auto iter : m_total_content_map)
		{
			vector<tstring> vec;			
			vec.push_back(MyPlatform::Int64ToStringW(m_currentRunNumber));
			vec.push_back(MyPlatform::Int64ToStringW(m_error));
			vec.push_back(MyPlatform::Int64ToStringW(iter.first));
			vec.push_back(iter.second->m_scriptname);
			vec.push_back(iter.second->m_timestamp);
			vec.push_back(MyPlatform::Int64ToStringW(iter.second->m_measurement));
			vec.push_back(iter.second->m_description);
			map_output.insert(std::pair<ULONGLONG, vector<tstring> >(iter.second->m_starttime, vec));
			vec.clear();
		}
		for (auto iter : map_output)
		{
			LogInfo(iter.second);
			LogInfoToExcel(outfile, iter.second);
		}
		if (outfile.is_open())
			outfile.close();
		// POSSIBLY ADD DELETE filename LOGIC IF DEBUG MODE=TRUE

		for (auto iter : m_total_content_map)
			delete iter.second;
		m_total_content_map.clear();
	}
	catch (exception ex)
	{
		std::tstring err = _T("outer Add Content header has crashed.");
		std::wostringstream stringStream;
		stringStream << err.c_str() << ex.what() << endl;
		std::tstring copyOfStr = stringStream.str();
		MyPlatform::CInformationTrace::TraceW(0, (wchar_t*)copyOfStr.c_str());
		stringStream = std::wostringstream();
	}
	return BENCH_MARK_OK;
}

int CWorkMark::SendRunningMessageToWindow(const TCHAR* szText)
{
	SendMessage(m_hWnd, WS_CHANGE_RUNNING_INFOR, 0, (LPARAM)szText);
	return BENCH_MARK_OK;
}

int CWorkMark::SendMarkTypeNameToWindow(const TCHAR* szText)
{
	SendMessage(m_hWnd, WS_MARK_TYPE_NAME, 0, (LPARAM)szText);
	return BENCH_MARK_OK;
}

BOOL CWorkMark::WriteInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data)
{
	DWORD Ret; //use to check status
	HKEY hKey; //key


			   //Open the key
	Ret = RegOpenKeyEx(
		hKeyParent,
		subkey,
		0,
		KEY_WRITE,
		&hKey
	);

	if (Ret == ERROR_SUCCESS)
	{

		//Set the value in key
		if (ERROR_SUCCESS !=
			RegSetValueEx(
				hKey,
				valueName,
				0,
				REG_DWORD,
				reinterpret_cast<BYTE *>(&data),
				sizeof(data)))
		{
			LogInfo({ _T("Error In WriteInRegistry with Key = "),  valueName });
			RegCloseKey(hKey);
			return FALSE;
		}

		//close the key
		RegCloseKey(hKey);

		return TRUE;
	}

	return FALSE;
}

int CWorkMark::Reboot()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };

	SetAutoRun();
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		::MessageBox(NULL, _T("重启计算机失败，请手动重启\r\n"), _T("提示"), MB_OK);
		return -1;
	}
	
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1; // one privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		::MessageBox(NULL, _T("重启计算机失败，请手动重启\r\n"), _T("提示"), MB_OK);
		return -2;
	}

	WriteInRegistry(
		HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		_T("EnableLUA"), 0);
	WriteInRegistry(
		HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
		_T("ConsentPromptBehaviorAdmin"), 0);
	 
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0); 
	exit(0);

	return BENCH_MARK_OK;
}

int CWorkMark::RecordAllMarkResult()
{
	for (size_t index = 0; index < m_veAppMarkRun.size(); index++)
	{
		g_ini.SetIntW(CFG_APP_MARK_IS_RUN, std::to_wstring(m_veAppMarkRun[index]).c_str(), FALSE);
	}

	for (size_t index = 0; index < m_vMarkWorkRunInfor.size(); index++)
	{
		MarkWorkRunningInfor&  nwri = m_vMarkWorkRunInfor[index];
		g_ini.SetIntW(CFG_APP_MARK_IS_RUN, std::to_wstring(nwri.eMarkType).c_str(), TRUE);
		tstring runStr;
		for each(auto& s in nwri.nRunningTime)
		{
			runStr += MyPlatform::Int64ToStringW(s);
			runStr += _T(",");
		}
		LogInfo({_T("Record all logged runtime key = "), std::to_wstring(nwri.eMarkType).c_str(), _T("values = "), runStr});
		g_ini.SetStringW(CFG_APP_MARK_RUN_TIME, std::to_wstring(nwri.eMarkType).c_str(), runStr.c_str());
	}
	return BENCH_MARK_OK;
}

int CWorkMark::ReadAppMarkResultFromDisk()
{
	vector<EMarkType> veAppMarkRun{};
	veAppMarkRun.push_back(E_MARK_NET);
	veAppMarkRun.push_back(E_MARK_OUTLOOK);
	veAppMarkRun.push_back(E_MARK_OFFICE_POWERPOINT);
	veAppMarkRun.push_back(E_MARK_OFFICE_WORD);
	veAppMarkRun.push_back(E_MARK_OFFICE_EXCEL);
	veAppMarkRun.push_back(E_MARK_PDF);
	veAppMarkRun.push_back(E_MARK_PHOTOSHOP);
	veAppMarkRun.push_back(E_MARK_WINRAR);
	veAppMarkRun.push_back(E_MARK_CAD);

	for (size_t index = 0; index < veAppMarkRun.size(); index++)
	{
		MarkWorkRunningInfor  nwri = { {}, E_MARK_TYPE_BEG, E_MARK_CLASSIFY_BEG };
		nwri.eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;
		nwri.eMarkType = veAppMarkRun[index];
		nwri.nRunningTime = GetCurrentnRunningTime(nwri.eMarkType);
		if (nwri.nRunningTime.size() > 0)
			m_vMarkWorkRunInfor.push_back(nwri);	 
	}

	return 0;
}

vector<__int64> CWorkMark::GetCurrentnRunningTime(int index)
{
	TCHAR str[MAX_PATH] = {};
	g_ini.GetStringW(CFG_APP_MARK_RUN_TIME, std::to_wstring(index).c_str(), str, MAX_PATH);
	LogInfo({ _T("Inside GetCurrentnRunningTime str = "), str });
	std::vector<__int64> runs;
	std::vector<tstring> parts = split(str, L',');
	for each (auto& s in parts)
	{
		LogInfo({_T("Inside GetCurrentnRunningTime runs loop "), s });
		runs.push_back(_ttoi64(s.c_str()));
	}
	return runs;
}
void CWorkMark::LogError(std::vector<std::tstring> errList)
{
	tstring err;
	for each (auto& s in errList)
		err.append(_T(" ")).append(s);
	MyPlatform::CInformationTrace::TraceW(0, (wchar_t*)err.c_str());
}
void CWorkMark::LogError(std::tstring err, std::tstring desc)
{
	MyPlatform::CInformationTrace::TraceW(0, _bstr_t(err.c_str()), MyPlatform::TcharToChar(desc.c_str()).c_str());
}

void CWorkMark::LogInfo(std::vector<std::tstring> infoList)
{
	tstring info;
	for each (auto& s in infoList)
		info.append(_T(" ")).append(s);
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)info.c_str());
}
void CWorkMark::LogInfo(std::tstring info, std::tstring desc)
{
	MyPlatform::CInformationTrace::TraceW(3,  _bstr_t(info.c_str()), MyPlatform::TcharToChar(desc.c_str()).c_str());
}

std::vector<std::tstring>  CWorkMark::split(const tstring& s, TCHAR delimiter)
{
	std::vector<std::tstring> tokens;
	std::tstring token;
	USES_CONVERSION;
	std::wistringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void  CWorkMark::SetMaxListDigits(tstring last, int& max)
{
	if (isdigit(last[0]))
	{
		int cur;
		std::wstringstream(last) >> cur;
		cur > max ? max = cur : 0;
	}
}

CString  CWorkMark::DefaultFileNameByProjectId()
{
	CString strFileName;
	vector<tstring> vecDirList{};
	wchar_t	szParentDir[_MAX_PATH];
	MyPlatform::GetFullPathUnderExeW(MyPlatform::ResultsFolder.c_str(), szParentDir, _MAX_PATH);
	MyPlatform::GetDirUnderDirW(szParentDir, &vecDirList, true);
	int max = 0;
	for each(auto& s in vecDirList)
	{
		vector<tstring> pathlist = split(s, L'\\');
		tstring projectName = pathlist.back();
		vector<tstring> projectnamelist = split(pathlist.back(), L'_');
		if (projectName.length() != PROJECT_FOLDER_PREFIX.length() + 3)
			continue;
		tstring projectNamePrefix = projectName.substr(0, projectName.length() - 3);
		if (0 != _tcsicmp(projectNamePrefix.c_str(), PROJECT_FOLDER_PREFIX.c_str()))
			continue;
		tstring curCountStr = projectName.substr(PROJECT_FOLDER_PREFIX.length(), 3);
		SetMaxListDigits(curCountStr, max);
	}
	strFileName.Format(_T("%s%03d"), PROJECT_FOLDER_PREFIX.c_str(), max + 1);
	return strFileName;
}
