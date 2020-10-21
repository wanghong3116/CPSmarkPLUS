#pragma once

#include "InstallObject.h"

class InstallPS : public InstallObject
{
public:
	InstallPS();
	~InstallPS();

	virtual int Install();
};