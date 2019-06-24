#pragma once

#include <list>
#include <vector>
#include "Exports.h"
#include "Box.h"
#include "Layers.h"

using namespace std;

class ENGINEDLL_API CollisionManager
{
	static CollisionManager *instance;

	vector<list<Box*>> listsOfBoxes;

	bool layersRelation[Count][Count];

	void MakeTheRealDetection(int index1, int index2);
public:
	void AddBox(Box* box);
	void SetRelation(int layer1, int layer2);
	void DetectCollisions();

	static CollisionManager* getInstance()
	{
		if (instance == NULL) instance = new CollisionManager();
		return instance;
	}
	CollisionManager(CollisionManager const&) = delete;
	void operator = (CollisionManager const&) = delete;
private:
	CollisionManager();
};