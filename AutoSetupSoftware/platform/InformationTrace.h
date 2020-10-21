
#ifndef __MYTRACE_H__3841AC47_3F17_4B65_AE3D_39E3B5EAE144
#define __MYTRACE_H__3841AC47_3F17_4B65_AE3D_39E3B5EAE144

namespace MyPlatform
{
	class CThreadMutex;

	class CInformationTrace
	{
	private:
		CInformationTrace();

	public:
		virtual ~CInformationTrace();

		static CInformationTrace* GetInformationTracePtr();

		static int TraceA(int nLevel, char *szFormat, ...);
		static int TraceW(int nLevel, wchar_t *szFormat, ...);

		int traceA(int nLevel, char *szFormat, ...);
		int traceW(int nLevel, wchar_t *szFormat, ...);


		int traceAArgPtr(int nLevel, char *szFormat, va_list argPtr);
		int traceWArgPtr(int nLevel, wchar_t *szFormat, va_list argPtr);

private:
		int resetLogFilePath();
		int combineMsg(int nLevel, const wchar_t *szTrace);

		int writeMsg(const wchar_t *szMsg);
	private:
		static CInformationTrace *m_pInforTrace;

		wchar_t		m_szLogFile[_MAX_PATH];
		wchar_t		m_szLogDir[_MAX_PATH];
		wchar_t		m_szLogExt[_MAX_EXT];

		int			m_nLimitNum;

		bool		m_bOpenConsole;


		CThreadMutex *m_pMutex;
	};
};



#endif

