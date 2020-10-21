#pragma once
class CHandBrakeMark : public CMarkObject
{
public:
	CHandBrakeMark();
	~CHandBrakeMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
private:
	TCHAR m_szHandBrakePath[_MAX_PATH] = { 0 };
	TCHAR m_szSrcVideoFP[_MAX_PATH] = { 0 };
	TCHAR m_szDstVideoFP[_MAX_PATH] = { 0 };
	TCHAR m_szQSVPresetPath[_MAX_PATH] = { 0 };
	TCHAR m_szNVENCPresetPath[_MAX_PATH] = { 0 };
	TCHAR m_szVCEPresetPath[_MAX_PATH] = { 0 };
	TCHAR m_szSWVPresetPath[_MAX_PATH] = { 0 };

	static tstring QSV_PRESET_NAME;
	static tstring NVENC_PRESET_NAME;
	static tstring VCE_PRESET_NAME;
	static tstring SW_PRESET_NAME;

	tstring m_szCurrentPresetName;
	tstring m_szCurrentPresetPath;

};


