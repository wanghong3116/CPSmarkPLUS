#include "stdafx.h"
#include "Counter.h"
#include "Opids.h"
 
int CCounter::InitializeItemStart(CCounterContent* details)
{
	auto opid_new = -1;
	auto iter = std::find_if(std::begin(m_opid_map), std::end(m_opid_map), [&](const std::pair<int, std::tstring> &pair)
	{
		return (pair.second.compare(details->m_description) == 0);
	});
	if (iter != std::end(m_opid_map))
	{
		opid_new = (*iter).first;
		m_content_map.insert(std::pair<int, CCounterContent*>(opid_new, details));
	}
	
	std::wostringstream stringStream;
	stringStream << "InitializeItemStart" << opid_new << "##"  <<
	 details->m_description << "##"
		<< details->m_measurement << "##" << details->m_scriptname << "##"
		<< details->m_starttime << details->m_timestamp << endl;
	std::tstring copyOfStr = stringStream.str();
	MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)copyOfStr.c_str());
	return opid_new;
}
void CCounter::UpdateItemEnd(int opid, ULONGLONG sleepTime)
{
	std::wostringstream stringStream;
	auto iter = std::find_if(std::begin(m_content_map), std::end(m_content_map), [&](const std::pair<int, CCounterContent*> &pair)
	{
		return (pair.first == opid);
	});
	if (iter != std::end(m_content_map))
	{
		auto content = (*iter).second;
		ULONGLONG endTime = MyPlatform::GetTimeStampMillisecond();
		content->m_measurement = endTime - content->m_starttime - sleepTime;
		
		stringStream << "UpdateItemEnd" << opid << "##" << sleepTime << "##" << content->m_measurement << endl;
		std::tstring copyOfStr = stringStream.str();
		MyPlatform::CInformationTrace::TraceW(3, (wchar_t*)copyOfStr.c_str());
	}
	else
	{
		stringStream << "UpdateItemEnd" << opid << "##" << "Error, couldn't find previously defined opid"  << endl;
		std::tstring copyOfStr = stringStream.str();
		MyPlatform::CInformationTrace::TraceW(0, (wchar_t*)copyOfStr.c_str());
	}
}

