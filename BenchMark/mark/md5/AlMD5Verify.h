#ifndef ALMD5VERIFY_H
#define ALMD5VERIFY_H

#include "almd5verify_global.h"
#include <string>

class ALMD5VERIFYSHARED_EXPORT AlMD5Verify
{
public:
    AlMD5Verify();
    AlMD5Verify(const char* str);
    virtual ~AlMD5Verify();

    virtual void SetFilePath(const char* str);
    virtual bool Verify(char * pMD5);

protected:
    std::string m_sFilePath;
};

#endif // ALMD5VERIFY_H
