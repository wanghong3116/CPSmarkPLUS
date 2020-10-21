#include "stdafx.h"
#include "InstallPS.h"

InstallPS::InstallPS()
	: InstallObject::InstallObject(_T("Software\\AdobePhotoshop20-mul_x64\\Set-up.exe"))
{
}

InstallPS::~InstallPS()
{

}

int InstallPS::Install()
{
	__super::Install();
	__super::WaitInstallOver();
	return 0;
}

