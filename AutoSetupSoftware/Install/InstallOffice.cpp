#include "stdafx.h"
#include "InstallOffice.h"

InstallOffice::InstallOffice()
	: InstallObject::InstallObject(_T(""))
{
}

InstallOffice::~InstallOffice()
{

}

int InstallOffice::Install()
{
	HWND hMainWnd = NULL, hHost = NULL, hNet = NULL;

	if (0 != MountDiskImage())
	{
		return -1;
	}

	__super::Install();
	__super::WaitInstallOver();

	Sleep(3 * 1000);
	hMainWnd = FindWindow(_T("Click2RunSetupUIClass"), _T("‪Microsoft Office‬"));
	hHost = FindWindowEx(hMainWnd, NULL, _T("NetUINativeHWNDHost"), _T(""));
	hNet = FindWindowEx(hHost, NULL, _T("NetUIHWND"), _T(""));
	SendMessage(hNet, WM_LBUTTONDOWN, 0, MAKELPARAM(400, 440));
	SendMessage(hNet, WM_LBUTTONUP, 0, MAKELPARAM(400, 440));
	DismountDiskImage();
	return 0;
}

int InstallOffice::MountDiskImage()
{
	TCHAR szCMD[_MAX_PATH * 3] = { 0 };
	TCHAR szDriveArray[_MAX_PATH] = { 0 };
	std::vector<MyPlatform::tstring> vstrDrive;
	TCHAR szOfficeSetupFP[_MAX_PATH] = { 0 };

	MyPlatform::GetFullPathUnderExe(_T("Software\\cn_office_professional_plus_2016_x86_x64_dvd_6969182.iso"), m_szISOFP, _ARRAYSIZE(m_szISOFP));
	if (0 != _taccess(m_szISOFP, 0))
	{
		printf("无法找到Office镜像文件\r\n");
		return -1;
	}
	_stprintf_s(szCMD, _ARRAYSIZE(szCMD), _T("powershell Mount-DiskImage -ImagePath \"%s\""), m_szISOFP);
	runProcess(szCMD, NULL, m_hProcess, m_hThread);
	__super::WaitInstallOver();

	GetLogicalDriveStrings(_MAX_PATH, szDriveArray);
	for (TCHAR * szDrive = szDriveArray; *szDrive; szDrive += _tcslen(szDrive) + 1)
	{
		vstrDrive.push_back(szDrive);
	}
	for (size_t index = 0; index < vstrDrive.size(); index++)
	{
		if (DRIVE_CDROM == GetDriveType(vstrDrive[index].c_str()))
		{
			_stprintf_s(szOfficeSetupFP, _ARRAYSIZE(szOfficeSetupFP), _T("%s\\office\\setup32.exe"), vstrDrive[index].c_str());
			if (0 == _taccess(szOfficeSetupFP, 0))
			{
				_tcscpy_s(m_szSetupFilePath, szOfficeSetupFP);
				break;
			}
		}
	}
	if (0 != _taccess(m_szSetupFilePath, 0))
	{
		return -2;
	}

	return 0;
}

int InstallOffice::DismountDiskImage()
{
	TCHAR szCMD[_MAX_PATH * 3] = { 0 };

	_stprintf_s(szCMD, _ARRAYSIZE(szCMD), _T("powershell Dismount-DiskImage -ImagePath \"%s\""), m_szISOFP);
	runProcess(szCMD, NULL, m_hProcess, m_hThread);
	__super::WaitInstallOver();

	return 0;
}

