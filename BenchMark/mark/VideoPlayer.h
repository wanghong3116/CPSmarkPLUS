#pragma once

class CVideoPlayerMark : public CMarkObject
{
public:
	CVideoPlayerMark();
	~CVideoPlayerMark();

	virtual int init();
	virtual int run();
	virtual int destroy();

private:
	TCHAR m_szVideoPlayerFP[_MAX_PATH] = { 0 };
	TCHAR m_szVideFP[_MAX_PATH] = { 0 };

};


