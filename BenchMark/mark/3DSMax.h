#pragma once

class C3DSMaxMark : public CMarkObject
{
public:
	C3DSMaxMark();
	~C3DSMaxMark();

	virtual int init();
	virtual int run();
	virtual int destroy();
	
private:
	virtual int wirteScript() = 0;
public:
	TCHAR m_sz3DSMaxFP[_MAX_PATH]			= { 0 };
	TCHAR m_szScriptFP[_MAX_PATH]			= { 0 };
	TCHAR m_szSourceMaterialFP[_MAX_PATH]	= { 0 };
};


