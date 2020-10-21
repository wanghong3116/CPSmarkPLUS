#pragma once
#include <map>
#include <vector>
#include <tuple>
#include <CounterContent.h>
#include <string>
#include <Opids.h>
using namespace std;
class COpids;
class CCounter 
{
public:
	 
	map<int, CCounterContent*> m_content_map = {};
	map<int, std::tstring> m_opid_map = {};
	int						InitializeItemStart(CCounterContent* details);
	void					UpdateItemEnd(int opid, ULONGLONG sleepTime);
	COpids*					m_opids;
};