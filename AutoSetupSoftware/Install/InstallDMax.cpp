#include "stdafx.h"
#include "InstallDMax.h"

InstallDMax::InstallDMax()
	: InstallAutodesk(_T("Software\\Autodesk_3ds_Max_2018_EFGJKPS_Win_64bit_dlm\\Setup.exe"), _T("Autodesk 3ds Max 2018"))
{
}

InstallDMax::~InstallDMax()
{

}

int InstallDMax::Install()
{
	__super::Install();
	
	return 0;
}

