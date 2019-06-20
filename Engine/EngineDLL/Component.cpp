#include "Component.h"

Component::Component()
	: name(""), reqTransform(false)
{
}
Component::~Component()
{
}

string Component::GetName()
{
	return name;
}

bool Component::ReqTransform()
{
	return reqTransform;
}