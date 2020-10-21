#include "stdafx.h"
#include "MarkObject.h"
#include "HandBrake.h"
#include "HandBrake_Opids.h"

tstring CHandBrakeMark::QSV_PRESET_NAME = _T("fast_1080p_qsv");
tstring CHandBrakeMark::NVENC_PRESET_NAME = _T("fast_1080p_h264_nvenc");
tstring CHandBrakeMark::VCE_PRESET_NAME = _T("fast_1080p_h264_vce");
tstring CHandBrakeMark::SW_PRESET_NAME = _T("fast_1080p_h264_sw");

CHandBrakeMark::CHandBrakeMark()
{
	LogInfo(_T("Invoke CHandBrakeMark().."));

	_tcscpy_s(m_szDescript, _T("Handbrake-视频转码"));
	m_eMarkType = E_MARK_HANDBRAKE;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Calculation;

	TCHAR szZipDir[_MAX_PATH] = { 0 };

	SetMD5PassWd(std::string("4c97f2c63f25d2db50fee9f4ffcb39f5")); //latest md5
	//SetMD5PassWd(std::string("bbe38ffaa3526e2085adbf67f01c84a1"));
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake.zip"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
 }

CHandBrakeMark::~CHandBrakeMark()
{
	LogInfo(_T("Invoke ~CHandBrakeMark().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);
}

int CHandBrakeMark::init()
{
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\HandBrakeCLI.exe"), m_szHandBrakePath);
	//MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\1.ts"), m_szSrcVideoFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\tos_c2_4k_24fps_40mbps_8bit_h264_aac_10sec.mp4"), m_szSrcVideoFP);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\1.mp4"), m_szDstVideoFP);

	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\fast_1080p_h264_qsv.json"), m_szQSVPresetPath);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\fast_1080p_h264_nvenc.json"), m_szNVENCPresetPath);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\fast_1080p_h264_vce.json"), m_szVCEPresetPath);
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\fast_1080p_h264_sw.json"), m_szSWVPresetPath);

	D3DADAPTER_IDENTIFIER9 identifier;
	LPDIRECT3D9 pD3d;

	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return 1;
	}
	pD3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &identifier);

	int const desc_size = sizeof(identifier.Description);
	TCHAR szDescription[desc_size];
	MultiByteToWideChar(CP_ACP, 0, identifier.Description, -1, szDescription, desc_size);

	if (_tcsstr(szDescription, _T("Intel")))
	{
		m_szCurrentPresetName = QSV_PRESET_NAME;
		m_szCurrentPresetPath = m_szQSVPresetPath;
		LogInfo(_T("Handbrake: Intel qsv"));
	} 
	else if (_tcsstr(szDescription, _T("NVIDIA")))
	{
		m_szCurrentPresetName = NVENC_PRESET_NAME;
		m_szCurrentPresetPath = m_szNVENCPresetPath;
		LogInfo(_T("Handbrake: NVIDIA nvenc"));
	} 
	else if (_tcsstr(szDescription, _T("AMD")))
	{
		m_szCurrentPresetName = VCE_PRESET_NAME;
		m_szCurrentPresetPath = m_szVCEPresetPath;
		LogInfo(_T("Handbrake: AMD vce"));
	}
	else
	{
		m_szCurrentPresetName = SW_PRESET_NAME;
		m_szCurrentPresetPath = m_szSWVPresetPath;
		LogInfo(_T("Handbrake: software"));
	}

	if (0 != _taccess(m_szHandBrakePath, 0)
		|| 0 != _taccess(m_szSrcVideoFP, 0))
	{
		LogError({ _T("转码文件不存在！") });
		return -1;
	}
	return BENCH_MARK_OK;
}

int CHandBrakeMark::run()
{
	TCHAR szCmd[_MAX_PATH * 5] = { 0 };
	int nRet = BENCH_MARK_OK;

/*	 _stprintf_s(szCmd, _T("\"%s\" -Z\"HQ 1080p30 Surround\" -i\"%s\" -o\"%s\""), m_szHandBrakePath, m_szSrcVideoFP, m_szDstVideoFP);
	LogInfo(_T("Handbrake:4K->2K视频转码进行中..."));
	TakeMeasurement(new CCounterContent(CHandBrake_Opids::DESC_TYPE_MARKER, CHandBrake_Opids::DESC_RUN),
		[&]()->int {
		nRet = runProcessAndWait(szCmd);
	return BENCH_MARK_OK; });
 	
	DeleteFile(m_szDstVideoFP);
	*/
	TCHAR szOutputLog[512];
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\out2.log"), szOutputLog);

	_stprintf_s(szCmd, _T("\"%s\" --preset-import-file \"%s\" -Z \"%s\" -i\"%s\" -o\"%s\""), 
		m_szHandBrakePath, m_szCurrentPresetPath.c_str(), m_szCurrentPresetName.c_str(), 
		m_szSrcVideoFP, m_szDstVideoFP);
	LogInfo(_T("Handbrake:4K->2K视频转码进行中..."));

	nRet = runProcessAndWaitRedirect(szCmd);
	FILE* fp = NULL;
	TCHAR szFileP[512];

	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\out.txt"), szFileP);

	_wfopen_s(&fp, szFileP, L"r");
	if (NULL == fp)
	{
		return -1;
	}
	char szContent[1024 * 100];
	fread(szContent, 1024 * 100, 1, fp);
	FCLOSE(fp);
	TCHAR wideStringContent[1024 * 100];
	size_t convertedbytes = 0;
	mbstowcs_s(&convertedbytes, wideStringContent, strlen(szContent), szContent, _TRUNCATE);

	if ((m_szCurrentPresetName == QSV_PRESET_NAME && _tcsstr(wideStringContent, _T("\"Encoder\": \"qsv_h264\""))) ||
		(m_szCurrentPresetName == NVENC_PRESET_NAME && _tcsstr(wideStringContent, _T("\"Encoder\": \"nvenc_h264\""))) ||
		(m_szCurrentPresetName == VCE_PRESET_NAME && _tcsstr(wideStringContent, _T("\"Encoder\": \"vce_h264\""))))
	{
		LogInfo(_T("Handbrake: Use hardware Acceleration"));
	}
	else
	{
		LogInfo(_T("Handbrake:fallback to Software encoding"));
		m_szCurrentPresetName = SW_PRESET_NAME;
		m_szCurrentPresetPath = m_szSWVPresetPath;
	}

	TakeMeasurement(new CCounterContent(CHandBrake_Opids::DESC_TYPE_MARKER, CHandBrake_Opids::DESC_RUN),
		[&]()->int {
		nRet = runProcessAndWait(szCmd);
		return BENCH_MARK_OK; });

	DeleteFile(m_szDstVideoFP);
	DeleteFile(szFileP);

	return nRet;
}

int CHandBrakeMark::destroy()
{
	LogInfo(_T("Invoke CHandBrakeMark.destroy().."));
	TCHAR szZipDir[_MAX_PATH] = { 0 };
	MyPlatform::GetFullPathUnderExe(_T("Resources\\HandBrake\\"), szZipDir);
	SetResourceFilePathUnicode(szZipDir);

	MyPlatform::KillProcessFromName(_T("Creative Cloud.exe"));
	return BENCH_MARK_OK;
}
