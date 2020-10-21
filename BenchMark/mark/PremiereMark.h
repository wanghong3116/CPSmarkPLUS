#pragma once

class CPremiereMark : public CMarkObject
{
public:
	const tstring WorkingVideoDir = _T("Resources\\Pr\\");
	const tstring BackeupVideoDir = _T("C:\\");
	const tstring PresetFileName = _T("render_clips.epr");
	tstring m_outputVideoNameFoundFromVecList;
	int MAX_RENDER_TIME_SECS = 60;
	int EXPECTED_RENDER_FILESIZE_MB = 72;
	int enum_proc_counter = 0;
	CPremiereMark();
	~CPremiereMark();

	virtual int init();
	virtual int run();
	virtual int destroy();

	int importPreset();
private:
	int initDependFile();
	int writeScript();

	int startPremiere();

	int findPremiereWnd();
	int SendRenderOrder();

	int deleteIntermediateFiles();
	int SendExportKey();

	int importPreset(tstring strPresetFileToImportFullPath);
	int setEncodeOutputPath(tstring strOutputFile);
	
private:
	TCHAR m_szPremiereFP[_MAX_PATH] = { 0 };
	TCHAR m_szProjectFP[_MAX_PATH] = { 0 };
	TCHAR m_szEmptyProjectFP[_MAX_PATH] = { 0 };
	TCHAR m_szRunDir[_MAX_PATH] = { 0 };
	vector<tstring> m_videoNameVec = {};
	vector<tstring> m_outputVideoNameVec = {};
	HWND hPromiereMainWnd = NULL;
};


