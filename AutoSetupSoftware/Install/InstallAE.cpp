#include "stdafx.h"
#include "InstallAE.h"

InstallAE::InstallAE()
	:InstallObject::InstallObject(_T("Software\\AdobeAfterEffects16AllTrial\\Set-up.exe"))
{
}

InstallAE::~InstallAE()
{

}

int InstallAE::Install()
{
	__super::Install();
	__super::WaitInstallOver();
	return 0;
}

