#pragma once

#include "Exports.h"

class ENGINEDLL_API Component
{
protected:
	int id;

public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	int GetId();

	Component();
	~Component();
};