#include "stdafx.h"
#include "ResourceVerify.h"
#include "AlMD5Verify.h"

#include "MarkObject.h"

MyPlatform::CIni g_ini_resource;

CResourceVerify::CResourceVerify()
{
	MyPlatform::CInformationTrace::TraceW(3, /*_bstr_t*/(wchar_t*)(_T("Invoke CResourceVerify..")));

    TCHAR szZipDir[_MAX_PATH] = { 0 };
    MyPlatform::GetFullPathUnderExe(_T("Resources.zip"), szZipDir);
    m_sResourceFilePath = szZipDir;
	//m_sVerifyCode = std::string("9d5e1c679cf1f3f10e7b85bce2e03a14"); //original md5
	//m_sVerifyCode = std::string("fa980ee41f20002d6612ed2f4f7d71da"); //latest md5
	m_sVerifyCode = std::string("e597a7d11af3730a97532a2bc1aa1a78"); //latest md5

}


CResourceVerify::~CResourceVerify()
{
}


CResourceVerify * CResourceVerify::GetInstance()
{
    static CResourceVerify rv;
    return &rv;
}

CString CResourceVerify::GetLastError()
{
    return m_strLastErrMsg;
}


bool CResourceVerify::VarifyResourceFile(std::vector<CMarkObject*>& vpMarkObject)
{
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)(_T("Invoke ResourceVerify.VarifyResourceFile..")));
    int nMarkCount = 0;

    //if (!VerifyMd5_T()) return false;
	if (!VerifyMd5()) return false; //orig code line

    if (!CMarkObject::UnzipResourceFile(m_sResourceFilePath, m_strLastErrMsg)) return false;

    nMarkCount = vpMarkObject.size();
    for (int nMarkIdx = 0; nMarkIdx < nMarkCount; nMarkIdx++)
    {
		if (!vpMarkObject[nMarkIdx]->VerifyFile())
        {
			return false;
        }
    }

    return true;
}

bool CResourceVerify::VerifyMd5_T()
{
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)(_T("Invoke ResourceVerify.VerifyMd5_T..")));
	char szMD5[1024] = { 0 };
	AlMD5Verify oMD5Verify(MyPlatform::TcharToChar(m_sResourceFilePath.c_str()).c_str());

	DeleteResourceDir();
	//DeleteResourceDir();
	//Sleep(1000 * 5);
	if (!oMD5Verify.Verify(szMD5))
	{
		m_strLastErrMsg.Format(_T("资源文件生成验证码失败\r\n"));
		return false;
	}
	if (MyPlatform::TcharToChar(t_m_sVerifyCode.c_str()) != std::string(szMD5))
	{
		m_strLastErrMsg.Format(_T("资源文件验证失败,可能被修改\r\n"));
		return false;
	}

	return true;
}

bool CResourceVerify::VerifyMd5()
{
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)(_T("Invoke ResourceVerify.VerifyMd5..")));
    char szMD5[1024] = { 0 };
    AlMD5Verify oMD5Verify(MyPlatform::TcharToChar(m_sResourceFilePath.c_str()).c_str());

	DeleteResourceDir();
    //DeleteResourceDir();
    //Sleep(1000 * 5);
    if (!oMD5Verify.Verify(szMD5))
    {
        m_strLastErrMsg.Format(_T("资源文件生成验证码失败\r\n"));
        return false;
    }
    if (m_sVerifyCode != std::string(szMD5))
    {
        m_strLastErrMsg.Format(_T("资源文件验证失败,可能被修改\r\n"));
        return false;
    }

    return true;
}

int CResourceVerify::DeleteResourceDir()
{
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)(_T("Invoke ResourceVerify.DeleteResourceDir..")));
    TCHAR szSourceDir[_MAX_PATH] = { 0 };
    TCHAR szSourceFileName[_MAX_PATH] = { 0 };


    _tsplitpath_s(m_sResourceFilePath.c_str(), NULL, 0, NULL, 0, szSourceFileName, _ARRAYSIZE(szSourceDir), NULL, 0);
    MyPlatform::GetFullPathUnderExe(szSourceFileName, szSourceDir, _ARRAYSIZE(szSourceDir));
    //MyPlatform::DeleteDir(szSourceDir);
	MyPlatform::DeleteDir(szSourceDir);

    return 0;
}
