#pragma once

#include "InstallObject.h"

class InstallAE : public InstallObject
{
public:
	InstallAE();
	~InstallAE();

	virtual int Install();
};