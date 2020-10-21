#pragma once

#include "WorkMark.h"
#include "AutoItX3_DLL.h"

typedef std::function<void()> Action;
typedef std::function<int()> Function_T;
class CMarkObject
{	 
public:
	CMarkObject();
	~CMarkObject();
	
	virtual bool VerifyFile_T();
	virtual bool VerifyFile();
	virtual bool UnzipResourceFile();
	virtual int init() = 0;
	virtual int run() = 0;
	virtual int destroy() = 0;
	int measurement_counter = 0;

	static bool CMarkObject::UnzipResourceFile(const MyPlatform::tstring & resourceFilePath, CString & lastError);
 
	__int64			getRunningTime();
	const TCHAR*	getDescript();
	EMarkType		getMarkType();
	EMarkClassify	getMarkClassify();
	 
 	void SleepAndRecord(ULONGLONG dwTime);
	void LogError(std::vector<std::tstring> errList);
	void LogError(std::tstring err, std::tstring desc);
	void LogInfo(std::tstring err);
	void LogInfo(std::vector<std::tstring> infoList);
	void LogInfo(std::tstring info, std::tstring desc);
	ULONGLONG  dwTimerNode = 50, dwTimerNode1 = 100, dwTimerNode2 = 250, dwTimerNode3 = 500;
	CWorkMark*	m_workMark;
	COpids*		m_opids;

	map<int, CCounterContent*> m_content_map = {};
	ULONGLONG  curTime = 0;
	void TakeMeasurement(CCounterContent* details, Function_T function);

protected:
	void SetMD5PassWd_T(tstring& sMD5);
	void SetMD5PassWd(const std::string& sMD5);
	
	void SetResourceFilePathUnicode(const MyPlatform::tstring& sResourceFilePathUnicode);
	int runProcessAndWait(TCHAR *szCmd, const TCHAR* szRunDir = NULL);
	int runProcessAndWaitRedirect(TCHAR *szCmd, const TCHAR* szRunDir = NULL);
	void changePathDoubleSplit(TCHAR *szPath, int nBufLen);
 
	int DeleteResourceDir();

	EMarkType					m_eMarkType = E_MARK_TYPE_BEG;
	EMarkClassify				m_eMarkClassify = E_MARK_CLASSIFY_BEG;
	__int64						m_nRunTime = 0;
	__int64						m_nSleepTime = 0;
	bool						m_bRunning = false;
	TCHAR						m_szDescript[_MAX_PATH] = { 0 };

	tstring						t_m_sMD5PassWd;
	std::string                 m_sMD5PassWd;
	MyPlatform::tstring         m_sResourceFilePath;

	int						InitializeItemStart(CCounterContent* details);
	void					UpdateItemEnd(int opid);
};


