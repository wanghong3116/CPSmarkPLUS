#pragma once
#include "InstallObject.h"
class InstallWinrar : public InstallObject
{
public:
	InstallWinrar();
	~InstallWinrar();

	virtual int Install();
};
