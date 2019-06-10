#pragma once

#include "Exports.h"

class ENGINEDLL_API Component
{
public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	Component();
	~Component();
};