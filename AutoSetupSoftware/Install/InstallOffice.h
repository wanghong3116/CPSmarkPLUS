#pragma once

#include "InstallObject.h"

class InstallOffice : public InstallObject
{
public:
	InstallOffice();
	~InstallOffice();

	virtual int Install();

private:
	int MountDiskImage();
	int DismountDiskImage();

	TCHAR m_szISOFP[_MAX_PATH] = { 0 };
};