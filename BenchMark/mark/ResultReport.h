#pragma once
#include "WorkMark.h"
#include <unordered_map>
class ResultReport
{
public:
	ResultReport(const std::vector<MarkWorkRunningInfor>* pvMarkWorkRunInfor,const int pnAchievement[E_MARK_CLASSIFY_END], 
		const TCHAR* szCPUName, const TCHAR* szCPUHz, const TCHAR* szCPUNumber, const TCHAR* szMemory, const TCHAR* szGPU, const TCHAR* szOSNameAndVersion, const TCHAR* szPDFFileName, CWorkMark* pworkMark
	,map<EMarkType, std::tstring>* mapItemToName);
	~ResultReport();

	int init();
	int Generate();
	int destroy();
	
private:
	int SummaryExcelFile(const std::tstring szDir);
	int SecondPage();
	int ThirdPage();
	
	int FillResult(int nPageNumber);
 private:
	PowerPoint::_ApplicationPtr m_app = NULL;
	PowerPoint::_PresentationPtr m_presentation = NULL;

	TCHAR m_szTemplateFP[_MAX_PATH] = { 0 };

	const TCHAR* m_szCPUName = NULL;
	const TCHAR* m_szCPUHz = NULL;
	const TCHAR* m_szCPUNumber = NULL;
	const TCHAR* m_szMemory = NULL;
	const TCHAR* m_szGPU = NULL;
	const TCHAR* m_szProjectNameAndVersion = NULL;
	const TCHAR* m_szOSNameAndVersion = NULL;
 
	const std::vector<MarkWorkRunningInfor>* m_pvMarkWorkRunInfor = NULL;
	const int* m_pnAchievement = NULL;
	map<EMarkType, tstring>*	m_mapItemToName;
	const TCHAR* 		m_szPDFFileName;
	CWorkMark*    m_pworkMark;
};


