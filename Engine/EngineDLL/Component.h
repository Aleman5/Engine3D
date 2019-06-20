#pragma once

#include <string>
#include "Exports.h"

using namespace std;

class Transform;

class ENGINEDLL_API Component
{
protected:
	string name;
	bool reqTransform;

public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void SetTransform(Transform* transform) = 0;

	string GetName();
	bool ReqTransform();

	Component();
	~Component();
};