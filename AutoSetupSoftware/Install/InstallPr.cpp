#include "stdafx.h"
#include "InstallPr.h"

InstallPr::InstallPr()
	: InstallObject(_T("Software\\AdobePremierePro13AllTrial\\Set-up.exe"))
{
}

InstallPr::~InstallPr()
{

}

int InstallPr::Install()
{
	__super::Install();
	__super::WaitInstallOver();
	return 0;
}

