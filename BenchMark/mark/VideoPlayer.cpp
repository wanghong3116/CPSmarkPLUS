#include "stdafx.h"
#include "MarkObject.h"
#include "VideoPlayer.h"

#define FFMPEG_PLAYER

CVideoPlayerMark::CVideoPlayerMark()
{
	_tcscpy_s(m_szDescript, _T("视频播放:4K高清"));
	m_eMarkType = E_MARK_OUTLOOK;
	m_eMarkClassify = E_MARK_CLASSIFY_Comprehensive_Application;
}

CVideoPlayerMark::~CVideoPlayerMark()
{

}

int CVideoPlayerMark::init()
{
#ifdef FFMPEG_PLAYER
	MyPlatform::GetFullPathUnderExe(_T("VideoPlayer\\ffplay.exe"), m_szVideoPlayerFP);
#else
	MyPlatform::GetFullPathUnderExe(_T("VideoPlayer\\videoplayer.exe"), m_szVideoPlayerFP);
#endif
	
	MyPlatform::GetFullPathUnderExe(_T("HandBrake\\1.ts"), m_szVideFP);

	if (0 != _taccess(m_szVideoPlayerFP, 0)
		|| 0 != _taccess(m_szVideFP, 0))
	{
		return -1;
	}

	return BENCH_MARK_OK;
}

int CVideoPlayerMark::run()
{
	TCHAR szCmd[_MAX_PATH * 3] = { 0 };

#ifdef FFMPEG_PLAYER
	_stprintf_s(szCmd, _ARRAYSIZE(szCmd), _T("%s -autoexit %s"), m_szVideoPlayerFP, m_szVideFP);
#else
	_stprintf_s(szCmd, _ARRAYSIZE(szCmd), _T("%s %s 1"), m_szVideoPlayerFP, m_szVideFP);
#endif
	
	ULONGLONG ullTime = MyPlatform::GetTimeStampMillisecond();


	SAFE_MARK_STEP_INFORMATION_CALLBACK(_T("VideoPlayer:4K视频播放中。。。"));
	runProcessAndWait(szCmd);

	m_nRunTime = MyPlatform::GetTimeStampMillisecond() - ullTime;

	return BENCH_MARK_OK;
}

int CVideoPlayerMark::destroy()
{

	return BENCH_MARK_OK;
}
