// MFCDemoDlg.h : Í·ÎÄ¼þ
//

#pragma once
#include <fstream>
#include <CounterContent.h>
#include "Opids.h"
#include <codecvt>
#include <d3d9types.h>
#include <d3d9.h>

class CMarkObject;

const std::codecvt_mode le_bom =
static_cast<std::codecvt_mode>(std::little_endian |
	std::generate_header |
	std::consume_header);

typedef std::codecvt_utf16<wchar_t, 0x10ffff, le_bom> wcvt_utf16le_bom;

class CWorkMark 
{
 	template<class type1, class type2>
	int NewMarkObjectAndAdd(std::vector<type1*>& rvMark, const TCHAR* szKey, type2* newType, COpids* opIds);
public:
	CWorkMark();;
	~CWorkMark();

	int					SetCSVFileName(const TCHAR* szCSVFileName);
	int					SetProjectName(const TCHAR* szPrjName);
	int					SetHWnd(HWND hWnd);
	int					SetMarkClassifySelect(bool pbMCSelect[E_MARK_CLASSIFY_END]);
	int					SetMarkClassifyItemSelect(EMarkClassify eMarkClassify, bool bSelect);
	const vector<MarkWorkRunningInfor>* GetMarkRunInfor();
	int					SetRunMarkStatus(ERunMarkStatus eRMStatus);

	int					StartWork(bool bRunComprehensiveCalculation);
	int					WaitForWorkThread();
	bool				IsWorking();
	HANDLE				GetThreadHandle();
	int					ProcessCounterInfo();
	 
	static Excel::RangePtr m_pRange;
	void LogError(std::vector<std::tstring>  err);
	void LogError(std::tstring err, std::tstring ddesc);
 	void LogInfo(std::vector<std::tstring> infoList);
	void LogInfo(std::tstring info, std::tstring ddesc);
	void SetMaxListDigits(tstring last, int& max);
	CString DefaultFileNameByProjectId();
	vector<tstring> split(const tstring& s, TCHAR delimiter);
	int	LogInfoToExcel(std::wofstream& outfile, vector<tstring>);
	static int cur_row, cur_col;
	int m_nLoopRunTime;
	int m_currentRunNumber;
	void SetRunNumber(int nLoopRunTime, int currentRunNumber);
	int					Reboot();
	int					RecordAllMarkResult();
	int					ReadAppMarkResultFromDisk();

private:
	static DWORD WINAPI ThreadProcMarkWork(_In_ LPVOID lpParameter);
	DWORD				MarkWork();
	DWORD				MarkWork(std::vector<CMarkObject*>& vpMarkObject);
	int					MarkWorkInit(std::vector<CMarkObject*>& vpMarkObject);
	
	int					SendRunningMessageToWindow(const TCHAR* szText);
	int					SendMarkTypeNameToWindow(const TCHAR* szText);
	bool				m_bCurrentRunIsComprehensiveApplication;
		TCHAR							m_szCSVFileName[_MAX_PATH] = { 0 };
	TCHAR							m_szProjectName[_MAX_PATH] = { 0 };

	std::vector<MarkWorkRunningInfor> m_vMarkWorkRunInfor;
	bool							m_pbMarkClassifySelect[E_MARK_CLASSIFY_END] = { 0 };
	ERunMarkStatus					m_eRunMarkStatus = E_RMS_RUN;

	HWND							m_hWnd = NULL;
	BOOL							m_bExitThreadMark = TRUE;
	HANDLE							m_hThreadMark = NULL;

	CONST tstring RESULTS_FOLDER = MyPlatform::ResultsFolder;
	CONST tstring PROJECT_FOLDER_PREFIX = _T("Project");


	std::vector<EMarkType>			m_veAppMarkRun;
	std::map<EMarkType, COpids*>			m_mapAppMarkOpids;
	static map<int, CCounterContent*> m_total_content_map;
	int m_error = 0;
	BOOL WriteInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data);
	vector<__int64> GetCurrentnRunningTime(int index);
 };

template<class type1, class type2>
int CWorkMark::NewMarkObjectAndAdd(std::vector<type1*>& rvMark, const TCHAR* szKey, type2* newType, COpids* opIds)
{
	if (!g_ini.GetInt(CFG_MARK, szKey))
	{
		return -1;
	}

	type2* pNew = new type2();
	if (NULL == pNew)
	{
		return -2;
	}
	(static_cast<CMarkObject*>(pNew))->m_workMark = this;
	(static_cast<CMarkObject*>(pNew))->m_opids = opIds;
	rvMark.push_back(pNew);
	return BENCH_MARK_OK;
}
