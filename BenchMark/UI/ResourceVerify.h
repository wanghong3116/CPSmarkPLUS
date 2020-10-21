#pragma once

#include "MarkObject.h"
class CResourceVerify
{
    CResourceVerify();
    CResourceVerify(const CResourceVerify&) = delete;
    CResourceVerify& operator = (const CResourceVerify&) = delete;
private:
    MyPlatform::tstring             m_sResourceFilePath;
	tstring							t_m_sVerifyCode;
    std::string                     m_sVerifyCode;
    CString                         m_strLastErrMsg;

public:
    ~CResourceVerify();

    static CResourceVerify *GetInstance();
    bool                    VarifyResourceFile(std::vector<CMarkObject*>& vpMarkObject);
    CString                 GetLastError();
private:
	bool                    VerifyMd5_T();
    bool                    VerifyMd5();
    int                     DeleteResourceDir();
};

