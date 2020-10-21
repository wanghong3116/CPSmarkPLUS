#include "stdafx.h"
#include "PlatformHeader.h"

namespace MyPlatform
{
#ifdef __CURL_CURL_H

 #ifdef _DEBUG
 #pragma comment(lib, "./curl-ssl/lib/libcurl.lib")
 #else
 #pragma comment(lib, "curl/libcurl.lib")
 #endif

enum E_DataType
{
	E_DATATYPE_BEG,

	E_DT_DL_STRING = E_DATATYPE_BEG,
	E_DT_DL_FILE,
	E_DT_UL_FILE,

	E_DATATYPE_END
};

struct T_DataInfo
{
	void*		pData;
	E_DataType	eType;
	const bool*	pbExit;
};

const int MAX_RECONNECT = 100;

size_t WriteData(char* szBuffer, size_t nSize, size_t nMember, void* pDst)
{
	size_t nRealSize	= nSize * nMember; 
	T_DataInfo* ptDInfo	= (T_DataInfo*)pDst;

	if (*(ptDInfo->pbExit))
	{
		return 0;
	}

	switch (ptDInfo->eType)
	{
	case E_DT_DL_FILE:
		{
			fseek((FILE*)ptDInfo->pData, 0, SEEK_END);
			fwrite(szBuffer, nSize, nMember, (FILE*)ptDInfo->pData);
		}
		break;

	case E_DT_DL_STRING:
		{
			string strTemp(szBuffer, nRealSize);
			*((string*)ptDInfo->pData) += strTemp;
		}
		break;

	case E_DT_UL_FILE:
		{
			fwrite(szBuffer, nSize, nMember, (FILE*)ptDInfo->pData);
		}
		break;

	default:
		break;
	}

	if (*ptDInfo->pbExit)
	{
		return 0;
	}

	return nRealSize; 
}

#define CURLEASYSETPOT(pCurl, option, szOption, param, eErrCode) \
{\
	eErrCode = curl_easy_setopt(pCurl, option, param);\
	if (CURLE_OK != eErrCode)\
 	 {\
 		 MyTraceA(L0, "Error: curl_easy_setopt option=[%s] failed , err=[%d]\n", szOption, eErrCode);\
 	 }\
	 THROWINT_STD(eErrCode, CURLE_OK, -3);\
}

int CurlMultiMonitor( CURLM* curlm )
{

	int nRet = 0;  

	struct timeval timeout_tv;  
	// 设置select超时时间  //  
	timeout_tv.tv_sec = 1;  
	timeout_tv.tv_usec = 0;  

	fd_set  fd_read;  
	fd_set  fd_write;  
	fd_set  fd_except; 
	int     nMaxfd; 

	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);  
	FD_ZERO(&fd_except); 

	curl_multi_fdset(curlm, &fd_read, &fd_write, &fd_except, &nMaxfd);  
	
	 /** 
     * When max_fd returns with -1, 
     * you need to wait a while and then proceed and call curl_multi_perform anyway. 
     * How long to wait? I would suggest 100 milliseconds at least, 
     * but you may want to test it out in your own particular conditions to find a suitable value. 
     */  
	if (nMaxfd == -1)
	{
		Sleep(100);
		//curl_multi_fdset(curlm, &fd_read, &fd_write, &fd_except, &max_fd); 
		return 0;
	}

	 /** 
     * 执行监听，当文件描述符状态发生改变的时候返回 
     * 返回0，程序调用curl_multi_perform通知curl执行相应操作 
     * 返回-1，表示select错误 
     * 注意：即使select超时也需要返回0，具体可以去官网看文档说明 
     */  
    int nRetCode = select(nMaxfd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);  
    switch(nRetCode)  
    {  
    case -1:  
        /* select error */  
        nRet = -1;  
        break;  
    case 0:  
        /* select timeout */  
		nRet = -2;
		break;
    default:  
        /* one or more of curl's file descriptors say there's data to read or write*/  
        nRet = 0;  
        break;  
    }  

    return nRet;  
}

string ChangeUrlToNet(const char* szUrl)
{
	string strUrl = szUrl;
	for (int nIdx = 0; nIdx < (int)strUrl.size(); nIdx++)
	{
		if (_T(' ') == strUrl[nIdx])
		{
			strUrl.replace(nIdx, 1, "%20");
		}
	}

	return strUrl;
}

string ChangeUrlToNative(const char* szUrl)
{
	static const char* SPACE = "%20";
	static const int nSpceLen = strlen(SPACE);

	string strUrl = szUrl;

	int nPos = 0;
	while (string::npos != (nPos = strUrl.find(SPACE, nPos)))
	{
		strUrl.replace(nPos, nSpceLen, " ");
	}

	return strUrl;
}

int DownloadWithCookies(const char* cszURL, string* pStrData, const bool* pbExit, vector<string>* pvstrHttpHeader/* = NULL*/)
{
	CURL* pCurl		= NULL;
	CURLcode eErrCode	= CURLE_OK;
	T_DataInfo di		= {0};
	di.eType			= E_DT_DL_STRING;
	di.pData			= pStrData;
	di.pbExit			= pbExit;
	long lRetCode		= 0;

	try
	{
		pCurl = curl_easy_init();
		if (NULL == pCurl)
		{
			MyTraceA(L0, "Error: DownloadCookies curl_easy_init failed\n");
			throw -2;
		}

		CURLEASYSETPOT(pCurl, CURLOPT_URL, "CURLOPT_URL", cszURL, eErrCode);
		if (NULL != pvstrHttpHeader
			&& 0 != pvstrHttpHeader->size())
		{
			struct curl_slist *ptHeaders = NULL;
			for (int nIdx = 0; nIdx < (int)pvstrHttpHeader->size(); nIdx++)
			{
				ptHeaders = curl_slist_append(ptHeaders, (*pvstrHttpHeader)[nIdx].c_str());
			}
			CURLEASYSETPOT(pCurl, CURLOPT_HTTPHEADER, "CURLOPT_HTTPHEADER", ptHeaders, eErrCode);
		}
		CURLEASYSETPOT(pCurl, CURLOPT_COOKIEFILE, "CURLOPT_COOKIE", "", eErrCode);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEFUNCTION, "CURLOPT_WRITEFUNCTION", WriteData, eErrCode);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEDATA, "CURLOPT_WRITEDATA", &di, eErrCode);
		CURLEASYSETPOT(pCurl, CURLOPT_COOKIEFILE, "CURLOPT_COOKIEFILE", "", eErrCode);
		CURLEASYSETPOT(pCurl, CURLOPT_FOLLOWLOCATION, "CURLOPT_FOLLOWLOCATION", 0, eErrCode);
		eErrCode = curl_easy_perform(pCurl);		
		if (CURLE_OK != eErrCode)
		{
			MyTraceA(L0, "DownloadCookies curl_easy_perform failed! url=[%s] nErr=[%d]\n", ChangeUrlToNative(cszURL).c_str(), eErrCode);
			throw -7;
		}

		eErrCode = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lRetCode);
		if (CURLE_OK != eErrCode || 200 != lRetCode)
		{
			throw -8;
		}

		throw PLATFORM_FUNC_NOERROR;
	}
	catch (int nRet)
	{
		if (NULL != pCurl)
		{
			curl_easy_cleanup(pCurl);
		}
		return nRet; 
	}
}

int GetRemoteFileSize(const char* szURL, double* pdFLen)
{
	long lRetCode		= 0;
	double dFLen		= 0;
	CURL* pCurl		= NULL;
	CURLcode eErr		= CURLE_OK;

	try
	{
		pCurl = curl_easy_init();
		if (NULL == pCurl)
		{
			throw -1;
		}

		CURLEASYSETPOT(pCurl, CURLOPT_URL, "CURLOPT_URL", szURL, eErr);
		CURLEASYSETPOT(pCurl, CURLOPT_FOLLOWLOCATION, "CURLOPT_FOLLOWLOCATION", 1, eErr);
		CURLEASYSETPOT(pCurl, CURLOPT_NOBODY, "CURLOPT_NOBODY", 1, eErr);
		if (CURLE_OK != curl_easy_perform(pCurl))
		{
			throw -2;
		}
		eErr = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lRetCode);
		if (CURLE_OK != eErr || 200 != lRetCode)
		{
			throw -3;
		}
		eErr = curl_easy_getinfo(pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, pdFLen);
		if (CURLE_OK != eErr)
		{
			throw -4;
		}

		throw PLATFORM_FUNC_NOERROR;
	}catch(int nRet)
	{
		curl_easy_cleanup(pCurl);
		return nRet;
	}
}

 int GetHttpHeader(string* pstrResponse, const char* cszURL, const bool* pbExit, vector<string>* pvstrHttpHeader)
 {
 	CURL* pCurl						= NULL;
 	CURLcode eErr					= CURLE_OK;
 
 	try
 	{
 		CURLMcode eMErr				= CURLM_OK;
 		T_DataInfo tDInfo			= {0};
 		T_DataInfo tHttpHeadInfo	= {0};
 		int nRunHandle				= 1;
 		int nReconnect				= 0;


 
 		pCurl = curl_easy_init();
 		if (NULL == pCurl)
 		{
 			MyTraceA(L0, "Error: DownloadFile curl_easy_init failed url=[%s]\n",
 				cszURL);
 			throw -1;
 		}
 		CURLEASYSETPOT(pCurl, CURLOPT_URL, "CURLOPT_URL", cszURL, eErr);
 		if (NULL != pvstrHttpHeader
 			&& 0 != pvstrHttpHeader->size())
 		{
 			struct curl_slist *ptHeaders = NULL;
 			for (int nIdx = 0; nIdx < (int)pvstrHttpHeader->size(); nIdx++)
 			{
 				ptHeaders = curl_slist_append(ptHeaders, (*pvstrHttpHeader)[nIdx].c_str());
 			}
 			CURLEASYSETPOT(pCurl, CURLOPT_HTTPHEADER, "CURLOPT_HTTPHEADER", ptHeaders, eErr);
 		}
 		CURLEASYSETPOT(pCurl, CURLOPT_FOLLOWLOCATION, "CURLOPT_FOLLOWLOCATION", 1, eErr);   

 		eErr = curl_easy_perform(pCurl);
		if (CURLM_OK != eErr)
		{
			throw -4;
		}
 		long lRetCode		= 0;
 		eErr = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lRetCode);
 		if (CURLE_OK != eErr || 200 != lRetCode)
 		{
 			throw -5;
 		}
 
 		throw PLATFORM_FUNC_NOERROR;
 	}
 	catch (int nRet)
 	{
 		if (NULL != pCurl)
 		{
 			curl_easy_cleanup(pCurl);
 		}
 		return nRet; 
 	}
 }

 void SplitHttpHeadInfo( const char* head, const char* key, char* szBuff,int nBufflen )
 {

	 const char* szKeyPos = strstr(head,key);
	 if(NULL != szKeyPos)
	 {
		 szKeyPos = szKeyPos + strlen(key);
		 while((*szKeyPos == ':' || *szKeyPos == ' ') && *szKeyPos != '\r' && *szKeyPos != '\n' && *szKeyPos != '\0') szKeyPos++;
		 //p = p+strlen(key)+2;
		 const char* q = strchr(szKeyPos,' ');
		 const char* q1 = strchr(szKeyPos,'\r');
		 const char* q2 = strchr(szKeyPos,'\n');
		 if(!q) q = q1;
		 if(!q) q = q2;
		 if(((q1<q)&& q1)||(!q)) q = q1;
		 if(((q2<q)&& q2)||(!q)) q = q2;
		 if(!q)q = szKeyPos;
		 strncpy_s(szBuff, nBufflen, szKeyPos, (q-szKeyPos)>nBufflen?nBufflen-1:(q-szKeyPos));
		 szBuff[(q-szKeyPos)>nBufflen-1?nBufflen-1:(q-szKeyPos)] = '\0';
	 }
 }

int DownloadFile(const char* cszURL, const TCHAR* cszFPath, const bool* pbExit, tstring* pstrNameToServer, int* pnRetCode, vector<string>* pvstrHttpHeader)
{
	CURL* pCurl						= NULL;
	CURLM* pCurlM					= NULL;
	FILE* fp						= NULL;
	CURLcode eErr					= CURLE_OK;
	string strFName;

	try
	{
		CURLMcode eMErr				= CURLM_OK;
		T_DataInfo tDInfo			= {0};
		T_DataInfo tHttpHeadInfo	= {0};
		int nRunHandle				= 1;
		int nReconnect				= 0;
		

		_tfopen_s(&fp, cszFPath, _T("wb"));
		if (NULL == fp)
		{
			MyTraceA(L0, "Error: DownloadFile fopen_s failed nErr=[%d] pcszFName=[%s]\n", errno, cszFPath);
			throw -3;
		}
		tDInfo.eType		= E_DT_DL_FILE;
		tDInfo.pData		= fp;
		tDInfo.pbExit		= pbExit;

		pCurlM = curl_multi_init();
		if (NULL == pCurlM)
		{
			MyTraceA(L0, "Error: DownloadFile curl_multi_init failed url=[%s] file=[%s]", URLDecode(cszURL).c_str(), cszFPath);
			throw -2;
		}
		pCurl = curl_easy_init();
		if (NULL == pCurl)
		{
			MyTraceA(L0, "Error: DownloadFile curl_easy_init failed url=[%s] file=[%s]\n",
				URLDecode(cszURL).c_str(), cszFPath);
			throw -3;
		}
		CURLEASYSETPOT(pCurl, CURLOPT_URL, "CURLOPT_URL", cszURL, eErr);
		if (NULL != pvstrHttpHeader
			&& 0 != pvstrHttpHeader->size())
		{
			struct curl_slist *ptHeaders = NULL;
			for (int nIdx = 0; nIdx < (int)pvstrHttpHeader->size(); nIdx++)
			{
				ptHeaders = curl_slist_append(ptHeaders, (*pvstrHttpHeader)[nIdx].c_str());
			}
			CURLEASYSETPOT(pCurl, CURLOPT_HTTPHEADER, "CURLOPT_HTTPHEADER", ptHeaders, eErr);
		}
		CURLEASYSETPOT(pCurl, CURLOPT_FOLLOWLOCATION, "CURLOPT_FOLLOWLOCATION", "", eErr);
		CURLEASYSETPOT(pCurl, CURLOPT_VERBOSE, "CURLOPT_VERBOSE", 1, eErr);     
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEFUNCTION, "CURLOPT_WRITEFUNCTION", WriteData, eErr);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEDATA, "CURLOPT_WRITEDATA", &tDInfo, eErr);
		curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 1800);
		
		if (NULL != pstrNameToServer)
		{
			tHttpHeadInfo.eType		= E_DT_DL_STRING;
			tHttpHeadInfo.pData		= &strFName;
			tHttpHeadInfo.pbExit	= pbExit;
			CURLEASYSETPOT(pCurl, CURLOPT_HEADERFUNCTION, "CURLOPT_HEADERFUNCTION", WriteData, eErr);
			CURLEASYSETPOT(pCurl, CURLOPT_WRITEHEADER, "CURLOPT_WRITEHEADER", &tHttpHeadInfo, eErr);
		}
		if (CURLM_OK != curl_multi_add_handle(pCurlM, pCurl))
		{
			MyTraceA(L0, "Error: DownloadFile curl_multi_add_handle failed! eMErr=[%d] url=[%s] file=[%s]\n", eMErr, URLDecode(cszURL).c_str(), cszFPath);
			throw -4;
		}

		while (0 != nRunHandle)
		{
			if (*pbExit)
			{
				throw -5;
			}

			switch (CurlMultiMonitor(pCurlM))
			{
			case -1:
				break;
	
			case -2:
				//连接超时 
				if (nReconnect == MAX_RECONNECT)
				{
					MyTraceA(L2, "DownloadFile nReconnect == MAX_RECONNECT connect failed!!");
					throw -6;
				}
				else
				{
					MyTraceA(L2, "DownloadFile connect=[%d] test", nReconnect);
					nReconnect++;
				}
				Sleep(100);
				break;

			case 0:
				nReconnect = 0;	//重连上，恢复重连计数

			default:
			{
				CURLMcode mode = CURLM_OK;

				while (true)
				{
					mode = curl_multi_perform(pCurlM, &nRunHandle);
					if (CURLM_CALL_MULTI_PERFORM == mode)
					{
						continue;
					}
					else
					{
						break;
					}
				}
			}
				
				
				break;
			} 

		}
		long lRetCode		= 0;
		eErr = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lRetCode);
		if (NULL != pnRetCode)
		{
			*pnRetCode = lRetCode;
		}
		if (CURLE_OK != eErr || 200 != lRetCode)
		{
			MyTraceA(L0, "curl_easy_getinfo failed lRetCode=[%d] url=[%s] file=[%s]", 
				lRetCode, URLDecode(cszURL).c_str(), TcharToChar(cszFPath).c_str());
			throw -3;
		}
		if (NULL != pstrNameToServer)
		{
			static const char* HTTPHEADER_FILENAME = "filename=";
			int nPos = strFName.find(HTTPHEADER_FILENAME);
			if (string::npos != nPos)
			{
				strFName = strFName.substr(nPos + strlen(HTTPHEADER_FILENAME));
				nPos = strFName.find('.');
				while('\r' != strFName[nPos] && '\n' != strFName[nPos] && ' ' != strFName[nPos] && nPos < (int)strFName.size()) nPos++;
				strFName = strFName.substr(0, nPos);
			}
			else
			{
				strFName.clear();
			}
		}

		throw PLATFORM_FUNC_NOERROR;
	}
	catch (int nRet)
	{
		if (NULL != pCurl && NULL != pCurlM)
		{
			curl_multi_remove_handle(pCurlM, pCurl);
			curl_easy_cleanup(pCurl);
			curl_multi_cleanup(pCurlM);
		}
		else if (NULL != pCurlM)
		{
			curl_multi_cleanup(pCurlM);
		}
	
		FCLOSE(fp);
		if (NULL != pstrNameToServer && 0 != strFName.size())
		{
			strFName = Utf8ToChar(strFName.c_str());
			char szAFile[_MAX_PATH] = {0};
			strcpy_s(szAFile, sizeof(szAFile), TcharToChar(cszFPath).c_str());
			strFName.insert(0, szAFile, strrchr(szAFile, '\\') - szAFile + 1);
			if (0 == _access(strFName.c_str(), 0))
			{
				char szFName[_MAX_FNAME] = {0};
				_splitpath_s(szAFile, NULL, 0, NULL, 0, szFName, sizeof(szFName), 0, NULL);
				int nExtPos = strFName.rfind('.');
				if (string::npos == nExtPos)
				{
					strFName += szFName;
				}
				else
				{
					strFName.insert(nExtPos, szFName);
				}
				
			}
			
			DeleteFileA(strFName.c_str());
			if (0 != _trename(cszFPath, CharToTchar(strFName.c_str()).c_str()))
			{
				MyTrace(L0, _T("rename failed src=[%s] dst=[%s] errno=[%d]"), cszFPath, strFName.c_str(), errno);
			}
			*pstrNameToServer = CharToTchar(strFName.c_str());
		}
		return nRet; 
	}
}

int PostData(string* strReceive, const char* cszURL, const char* cszPostData, const bool* pbExit, std::string* pstrResponseHeader, long* plRet, vector<string>* pvstrHttpHeader)
{
	CURL* pCurl		= NULL;
	CURLcode eCURLErr	= CURLE_OK;
	
	try
	{
		T_DataInfo tHttpHeadInfo		= {0};
		T_DataInfo tDInfo				= {0};
		long lRetCode					= 0;
		struct curl_httppost* ptPost	= NULL;
		struct curl_httppost* ptLast	= NULL;

		tDInfo.eType		= E_DT_DL_STRING;
		tDInfo.pbExit		= pbExit;
		tDInfo.pData		= strReceive;

		pCurl = curl_easy_init();
		if (NULL == pCurl)
		{
			MyTraceA(L0, "PostData curl_easy_init failed!");
			throw -2;
		}
		if (NULL != pvstrHttpHeader
			&& 0 != pvstrHttpHeader->size())
		{
			struct curl_slist *ptHeaders = NULL;
			for (int nIdx = 0; nIdx < (int)pvstrHttpHeader->size(); nIdx++)
			{
				ptHeaders = curl_slist_append(ptHeaders, (*pvstrHttpHeader)[nIdx].c_str());
			}
			CURLEASYSETPOT(pCurl, CURLOPT_HTTPHEADER, "CURLOPT_HTTPHEADER", ptHeaders, eCURLErr);
		}
		CURLEASYSETPOT(pCurl, CURLOPT_URL, "CURLOPT_URL", cszURL, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_POST, "CURLOPT_URL", 1, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_POSTFIELDS, "CURLOPT_URL", cszPostData, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_MAX_SEND_SPEED_LARGE, "CURLOPT_MAX_SEND_SPEED_LARGE", 1000*1000, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEFUNCTION, "CURLOPT_WRITEFUNCTION", WriteData, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEDATA, "CURLOPT_WRITEDATA", &tDInfo, eCURLErr);
		if (NULL != pstrResponseHeader)
		{
			tHttpHeadInfo.eType		= E_DT_DL_STRING;
			tHttpHeadInfo.pData		= pstrResponseHeader;
			tHttpHeadInfo.pbExit	= pbExit;
			CURLEASYSETPOT(pCurl, CURLOPT_HEADERFUNCTION, "CURLOPT_HEADERFUNCTION", WriteData, eCURLErr);
			CURLEASYSETPOT(pCurl, CURLOPT_WRITEHEADER, "CURLOPT_WRITEHEADER", &tHttpHeadInfo, eCURLErr);
		}
		eCURLErr = curl_easy_perform(pCurl);
		if (CURLE_OK != eCURLErr)
		{
			MyTraceA(L0, "PostData curl_easy_perform failed! eCurErr=[%d]", eCURLErr);
			throw -4;
		}
		curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lRetCode);
		if (NULL != plRet)
		{
			*plRet = lRetCode;
		}
		if (200 != lRetCode && 302 != lRetCode)
		{
			MyTraceA(L2, "PostData curl_easy_getinfo failed! lRetCode=[%d]", lRetCode);
			throw -5;
		}

		throw PLATFORM_FUNC_NOERROR;
	}
	catch(int nRet)
	{
		if (NULL != pCurl)
		{
			curl_easy_cleanup(pCurl);
		}
		if (PLATFORM_FUNC_NOERROR != nRet)
		{
			MyTraceA(L0, "PostData failed! cszURL=[%s] cszPostData=[%s] pbExit=[%s]", URLDecode(cszURL).c_str(), URLDecode(cszPostData).c_str(), *pbExit ? "true" : "false");
		}
		return nRet;
	}
}

int UploadFile(tstring* pstrReceive, const char* cszURL, const TCHAR* cszFPath, const bool* pbExit)
{
	CURL* pCurl		= NULL;
	FILE* fp			= NULL;
	CURLcode eCURLErr	= CURLE_OK;

	try
	{
		T_DataInfo tDInfo				= {0};
		long	lFileSize					= 0;
		long lRetCode					= 0;
		struct curl_httppost* ptPost	= NULL;
		struct curl_httppost* ptLast	= NULL;
		string strRetInfo;

		errno_t eFErr = _tfopen_s(&fp, cszFPath, _T("rb"));
		if (NULL == fp)
		{
			MyTraceA(L0, "UploadFile _tfopen_s failed, szUrl=[%s] szFPath=[%s] eFErr=[%d]", URLDecode(cszURL).c_str(), cszFPath, eFErr);
			throw -1;
		}
		fseek(fp, 0, SEEK_END);
		lFileSize = ftell(fp);
		tDInfo.eType		= E_DT_DL_STRING;
		tDInfo.pbExit		= pbExit;
		tDInfo.pData		= &strRetInfo;

		pCurl = curl_easy_init();
		if (NULL == pCurl)
		{
			throw -2;
		}
		CURLEASYSETPOT(pCurl, CURLOPT_URL, "CURLOPT_URL", cszURL, eCURLErr);
		curl_formadd(&ptPost, &ptLast, CURLFORM_COPYNAME, "file", CURLFORM_FILE, TcharToChar(cszFPath).c_str(), 
			CURLFORM_FILENAME, TcharToUtf8(_tcsrchr(cszFPath, _T('\\')) + 1).c_str(), CURLFORM_END);
		curl_formadd(&ptPost, &ptLast, CURLFORM_COPYNAME, "submit", CURLFORM_COPYCONTENTS, "send", CURLFORM_END);
		CURLEASYSETPOT(pCurl, CURLOPT_HTTPPOST, "CURLOPT_HTTPPOST", ptPost, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_INFILESIZE_LARGE, "CURLOPT_INFILESIZE_LARGE", (curl_off_t)lFileSize, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_MAX_SEND_SPEED_LARGE, "CURLOPT_MAX_SEND_SPEED_LARGE", 1000*1000, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEFUNCTION, "CURLOPT_WRITEFUNCTION", WriteData, eCURLErr);
		CURLEASYSETPOT(pCurl, CURLOPT_WRITEDATA, "CURLOPT_WRITEDATA", &tDInfo, eCURLErr);
		eCURLErr = curl_easy_perform(pCurl);
		if (CURLE_OK != eCURLErr)
		{
			MyTraceA(L0, "UploadFile curl_easy_perform failed! url=[%s] szFPath=[%s] nErr=[%d]\n", URLDecode(cszURL).c_str(), cszFPath, eCURLErr);
			throw -4;
		}
		curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lRetCode);
		if (200 != lRetCode)
		{
			MyTraceA(L0, "UploadFile curl_easy_getinfo failed! url=[%s] szFPath=[%s] lRetCode=[%d]", URLDecode(cszURL).c_str(), cszFPath, lRetCode);
			throw -5;
		}
		*pstrReceive = Utf8ToTchar(strRetInfo.c_str());
		throw PLATFORM_FUNC_NOERROR;
	}
	catch(int nRet)
	{
		if (NULL != pCurl)
		{
			curl_easy_cleanup(pCurl);
		}
		FCLOSE(fp);
		return nRet;
	}
}

int ParseHttpHeader(const char* szHttpHeader, std::map<std::string, std::string>& rmapHttpHeadInfo)
{
	const char* SEPARATE = "\r\n";
	const int LEN_SEPARATE = strlen(SEPARATE);

	const char* szHeaderEndPos = szHttpHeader + strlen(szHttpHeader);
	const char* szLineBegPos = NULL;
	const char* szLineEndPos = NULL;
	const char* szColonPos = NULL;

	szLineBegPos = szLineEndPos = szHttpHeader;
	while (szLineEndPos < szHeaderEndPos)
	{
		if (NULL == (szLineEndPos = strstr(szLineEndPos, SEPARATE)))
		{
			szLineEndPos = szHeaderEndPos;
		}
		if (NULL != (szColonPos = strchr(szLineBegPos, ':')) && szColonPos < szLineEndPos)
		{
			string strKey(szLineBegPos, szColonPos - szLineBegPos);
			szColonPos++;
			while (*szColonPos == ' ')
			{
				szColonPos++;
			}
			if (szColonPos < szLineEndPos)
			{
				rmapHttpHeadInfo[strKey] = string(szColonPos, szLineEndPos - szColonPos);
			}
		}
		szLineEndPos += LEN_SEPARATE;
		szLineBegPos = szLineEndPos;
	}

	return PLATFORM_FUNC_NOERROR;
}

string GetHttpHeaderValueByName(const char* szHttpHeader, const char* szKey)
{
	std::map<std::string, std::string> mapHttpHeadInfo;

	ParseHttpHeader(szHttpHeader, mapHttpHeadInfo);
	if (0 == mapHttpHeadInfo.size()
		|| mapHttpHeadInfo.end() == mapHttpHeadInfo.find(szKey))
	{
		return "";
	}

	return mapHttpHeadInfo[szKey];
}

#endif		//__CURL_CURL_H

}