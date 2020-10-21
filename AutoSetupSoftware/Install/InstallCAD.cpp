#include "stdafx.h"
#include "InstallCAD.h"

InstallCAD::InstallCAD()
	: InstallAutodesk(_T("Software\\AutoCAD_2018_Simplified_Chinese_Win_64bit_dlm\\Setup.exe"), _T("Autodesk® AutoCAD® 2018"))
{
	
}

InstallCAD::~InstallCAD()
{

}

int InstallCAD::Install()
{
	__super::Install();
	
	return 0;
}

