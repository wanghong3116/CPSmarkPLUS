#include "stdafx.h"
#include "CounterContent.h"

CCounterContent::CCounterContent()
{
}

CCounterContent::CCounterContent( tstring sc, tstring d)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	std::wostringstream stringStream;
	stringStream << systime.wMonth << ":" << systime.wDay << ":" << systime.wYear << " " << 
		systime.wHour << ":" << systime.wMinute << ":" << systime.wSecond << "." << systime.wMilliseconds;
	m_timestamp = stringStream.str();
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)m_timestamp.c_str());
	m_measurement = 0;
	m_description = d;
	m_scriptname = sc;
	m_starttime = MyPlatform::GetTimeStampMillisecond();
	stringStream = std::wostringstream();

}

CCounterContent::~CCounterContent()
{
}
