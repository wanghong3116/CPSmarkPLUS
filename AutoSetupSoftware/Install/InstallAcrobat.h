#pragma once

#include "InstallObject.h"

class InstallAcrobat : public InstallObject
{
public:
	InstallAcrobat();
	~InstallAcrobat();

	virtual int Install();

private:
	int StartSetup();
	int WaitSetupOver();

	HWND FindSetUpWindow();
private:
};