#pragma once

#include "InstallObject.h"

class InstallPr : public InstallObject
{
public:
	InstallPr();
	~InstallPr();

	virtual int Install();
};