// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"

std::string GetComputerCPUName()
{
	const char* szSubKey = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";//注册表子键路径  

	HKEY hKey = NULL;
	char szCPUName[_MAX_PATH] = { 0 };
	DWORD lNameLen = _ARRAYSIZE(szCPUName);

	if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hKey))
	{
		return "";
	}
	if (ERROR_SUCCESS != RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)szCPUName, &lNameLen))
	{
		return "";
	}

	return szCPUName;
}

std::string GetComputerCPUIdentifier()
{
	const char* szSubKey = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";//注册表子键路径  
 
	HKEY hKey = NULL;
	char szCPUIdentifier[_MAX_PATH] = { 0 };
	DWORD lNameLen = _ARRAYSIZE(szCPUIdentifier);

	if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hKey))
	{
		return "";
	}
	if (ERROR_SUCCESS != RegQueryValueExA(hKey, "Identifier", NULL, NULL, (LPBYTE)szCPUIdentifier, &lNameLen))
	{
		return "";
	}

	return szCPUIdentifier;
}

std::vector<DWORD> GetComputerCPUDominantFrequency(DWORD dwPhysicalProcessorNumber)
{
	const char* szSubKey = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor";//注册表子键路径  
	std::vector<DWORD> vec = {};
	HKEY hKey = NULL;
	DWORD dwVaule = 0;
	DWORD lNameLen = sizeof(dwVaule);
	for (int i = 0; i < dwPhysicalProcessorNumber; i++)
	{
		std::tstring szSubKeyNew{ MyPlatform::CharToTchar(szSubKey) };
		szSubKeyNew.append(_T("\\")).append(MyPlatform::Int64ToStringW(i));
		if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, szSubKeyNew.c_str(), 0, KEY_READ, &hKey))
		{
			return{};
		}
		if (ERROR_SUCCESS != RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&dwVaule, &lNameLen))
		{
			return{};
		}
		vec.push_back(dwVaule);
	}

	return std::move(vec);
}

/// 获取内存大小以GB为单位
double GetComputerMemorySize()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex); // I misunderstand that

	GlobalMemoryStatusEx(&statex);
	return (double)statex.ullTotalPhys / (1024 * 1024 * 1024);
}

typedef BOOL(WINAPI *LPFN_GLPI)(
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
	PDWORD);


// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	DWORD i;

	for (i = 0; i <= LSHIFT; ++i)
	{
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest /= 2;
	}

	return bitSetCount;
}

int GetComputerCPUNumber(DWORD& processorPackageCount, DWORD& processorCoreCount, DWORD& logicalProcessorCount)
{
	LPFN_GLPI glpi;
	BOOL done = FALSE;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	DWORD returnLength = 0;
	//DWORD logicalProcessorCount = 0;
	DWORD numaNodeCount = 0;
	//DWORD processorCoreCount = 0;
	DWORD processorL1CacheCount = 0;
	DWORD processorL2CacheCount = 0;
	DWORD processorL3CacheCount = 0;
	//DWORD processorPackageCount = 0;
	DWORD byteOffset = 0;
	PCACHE_DESCRIPTOR Cache;

	glpi = (LPFN_GLPI)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")),
		"GetLogicalProcessorInformation");
	if (NULL == glpi)
	{
		_tprintf(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
		return (1);
	}

	while (!done)
	{
		DWORD rc = glpi(buffer, &returnLength);

		if (FALSE == rc)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer)
					free(buffer);

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
					returnLength);

				if (NULL == buffer)
				{
					_tprintf(TEXT("\nError: Allocation failure\n"));
					return (2);
				}
			}
			else
			{
				_tprintf(TEXT("\nError %d\n"), GetLastError());
				return (3);
			}
		}
		else
		{
			done = TRUE;
		}
	}

	ptr = buffer;

	while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
	{
		switch (ptr->Relationship)
		{
		case RelationNumaNode:
			// Non-NUMA systems report a single record of this type.
			numaNodeCount++;
			break;

		case RelationProcessorCore:
			processorCoreCount++;

			// A hyperthreaded core supplies more than one logical processor.
			logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
			break;

		case RelationCache:
			// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
			Cache = &ptr->Cache;
			if (Cache->Level == 1)
			{
				processorL1CacheCount++;
			}
			else if (Cache->Level == 2)
			{
				processorL2CacheCount++;
			}
			else if (Cache->Level == 3)
			{
				processorL3CacheCount++;
			}
			break;

		case RelationProcessorPackage:
			// Logical processors share a physical package.
			processorPackageCount++;
			break;

		default:
			_tprintf(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
			break;
		}
		byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		ptr++;
	}

	// 	_tprintf(TEXT("\nGetLogicalProcessorInformation results:\n"));
	// 	_tprintf(TEXT("Number of NUMA nodes: %d\n"),
	// 		numaNodeCount);
	// 	_tprintf(TEXT("Number of physical processor packages: %d\n"),
	// 		processorPackageCount);
	// 	_tprintf(TEXT("Number of processor cores: %d\n"),
	// 		processorCoreCount);
	// 	_tprintf(TEXT("Number of logical processors: %d\n"),
	// 		logicalProcessorCount);
	// 	_tprintf(TEXT("Number of processor L1/L2/L3 caches: %d/%d/%d\n"),
	// 		processorL1CacheCount,
	// 		processorL2CacheCount,
	// 		processorL3CacheCount);

	free(buffer);

	return 0;
}

#include "D3d9.h"
#pragma comment(lib, "D3d9.lib")

DWORD GetComputerAdapterCount()
{
	LPDIRECT3D9 pD3D = NULL;
	DWORD dwNumAdapters = 0;

	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))//创建Direct 3D对象
	{
		return 0;
	}
	dwNumAdapters = pD3D->GetAdapterCount();//获得显卡数量
	pD3D->Release();

	return dwNumAdapters;
}

int GetComputerAdapterDescription(std::vector<std::string>& rvstrAdapterDescription)
{
	LPDIRECT3D9 pD3D = NULL;
	DWORD m_dwNumAdapters = 0;

	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))//创建Direct 3D对象
	{
		return -1;
	}
	m_dwNumAdapters = pD3D->GetAdapterCount();//获得显卡数量
	for (UINT iAdapter = 0; iAdapter < m_dwNumAdapters; iAdapter++)
	{
		D3DADAPTER_IDENTIFIER9 di;
		pD3D->GetAdapterIdentifier(iAdapter, 0, &di);//获得显卡信息
		rvstrAdapterDescription.push_back(di.Description);
	}

	return 0;
}

std::tstring GetComputerOSNameAndVersion()
{
	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion
	const char* szSubKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";//注册表子键路径  
	std::tstring OSNameAndVersion;
	TCHAR OSName[MAX_PATH] = {};
	TCHAR OSVersion[MAX_PATH] = {};
	HKEY hKey = NULL;
	DWORD lNameLen = _ARRAYSIZE(OSName);
	std::wostringstream ostream;
	if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hKey))
	{

		if (ERROR_SUCCESS == RegQueryValueExW(hKey, _T("ProductName"), NULL, NULL, (LPBYTE)OSName, &lNameLen))
		{
			ostream << OSName;
			lNameLen = _ARRAYSIZE(OSVersion);
			if (ERROR_SUCCESS == RegQueryValueExW(hKey, _T("EditionSubstring"), NULL, NULL, (LPBYTE)OSVersion, &lNameLen))
			{
				ostream << OSVersion << endl;
			}
		}
		OSNameAndVersion = ostream.str();		
	}
	return OSNameAndVersion.substr(0, OSNameAndVersion.find_first_of(L'('));;
}


