#include "CollisionManager.h"

CollisionManager * CollisionManager::instance = NULL;

CollisionManager::CollisionManager()
{
	for (int i = 0; i < Count; i++)
	{
		list<Box*> list;
		listsOfBoxes.push_back(list);
	}

	for (int i = 0; i < Count; i++)
		for (int j = 0; j < Count; j++)
			layersRelation[i][j] = false;
}

void CollisionManager::AddBox(Box* box)
{
	listsOfBoxes[box->transform->GetLayer()].push_back(box);
}

void CollisionManager::SetRelation(int layer1, int layer2)
{
	if (layer1 <= layer2)
		layersRelation[layer1][layer2] = true;
	else
		layersRelation[layer2][layer1] = true;
}

void CollisionManager::DetectCollisions()
{
	for (int i = 0; i < Count; i++)
		for (int j = i; j < Count; j++)
			if (layersRelation[i][j] == 1)
				MakeTheRealDetection(i, j);
}

void CollisionManager::MakeTheRealDetection(int index1, int index2)
{
	// If any of this lists has no Entity just return.
	if (listsOfBoxes[index1].size() == 0
	 || listsOfBoxes[index2].size() == 0)
		return;
	
	for(list<Box*>::iterator it1 = listsOfBoxes[index1].begin(); it1 != listsOfBoxes[index1].end(); it1++)
	{
		vec3 col1 = (*it1)->size;

		for (list<Box*>::iterator it2 = listsOfBoxes[index2].begin(); it2 != listsOfBoxes[index2].end(); it2++)
		{
			if (!(*it1)->transform->isStatic || !(*it2)->transform->isStatic) // If both are static don't check collision.
			{
				vec3 diff = (*it2)->transform->vectorPosition - (*it1)->transform->vectorPosition;

				float dX = abs(diff.x);
				float dY = abs(diff.y);

				vec3 col2 = (*it2)->size;
				
				if (dX < col1.x / 2 + col2.x / 2
				&&  dY < col1.y / 2 + col2.y / 2)
				{
					float pX = col1.x / 2 + col2.x / 2 - diff.x;
					float pY = col1.y / 2 + col2.y / 2 - diff.y;

					if (pX > pY)
					{
						// Vertical Collision
						if (!(*it1)->transform->isStatic)
						{
							if (!(*it2)->transform->isStatic) // Any of them are static.
							{
								// Calculations with mass.
								float m1 = (*it1)->transform->mass;
								float m2 = (*it2)->transform->mass;

								float mPercentage = (m1 + m2) / m1;
								float move = pY / mPercentage;

								(*it2)->transform->Translate(vec3(0.0f, move, 0.0f));
								(*it1)->transform->Translate(vec3(0.0f, -pY + move, 0.0f));

								(*it1)->transform->CollisionWith((*it2)->transform);
								(*it2)->transform->CollisionWith((*it1)->transform);
							}
							else // Entity 1 is pushed back
							{
								if ((*it1)->transform->vectorPosition.y > (*it2)->transform->vectorPosition.y)
									(*it1)->transform->Translate(vec3(0.0f, pY, 0.0f));
								else
									(*it1)->transform->Translate(vec3(0.0f, -pY, 0.0f));

								(*it1)->transform->CollisionWith((*it2)->transform);
								(*it2)->transform->CollisionWith((*it1)->transform);
							}
						}
						else // Entity 2 is pushed back
						{
							if ((*it2)->transform->vectorPosition.y > (*it1)->transform->vectorPosition.y)
								(*it2)->transform->Translate(vec3(0.0f, pY, 0.0f));
							else
								(*it2)->transform->Translate(vec3(0.0f, -pY, 0.0f));

							(*it1)->transform->CollisionWith((*it2)->transform);
							(*it2)->transform->CollisionWith((*it1)->transform);
						}
					}
					else
					{
						// Horizontal Collision
						if (!(*it1)->transform->isStatic)
						{
							if (!(*it2)->transform->isStatic) // Any of them are static.
							{
								// Acá haría los cálculos teniendo en cuenta las masas.
								float m1 = (*it1)->transform->mass;
								float m2 = (*it2)->transform->mass;

								float mPercentage = (m1 + m2) / m1;
								float move = pX / mPercentage;

								(*it2)->transform->Translate(vec3(move, 0.0f, 0.0f));
								(*it1)->transform->Translate(vec3(-pX + move, 0.0f, 0.0f));

								(*it1)->transform->CollisionWith((*it2)->transform);
								(*it2)->transform->CollisionWith((*it1)->transform);
							}
							else // Entity 1 is pushed back
							{
								if ((*it1)->transform->vectorPosition.x > (*it2)->transform->vectorPosition.x)
									(*it1)->transform->Translate(vec3(pX, 0.0f, 0.0f));
								else
									(*it1)->transform->Translate(vec3(-pX, 0.0f, 0.0f));

								(*it1)->transform->CollisionWith((*it2)->transform);
								(*it2)->transform->CollisionWith((*it1)->transform);
							}
						}
						else // Entity 2 is pushed back
						{
							if ((*it2)->transform->vectorPosition.x > (*it1)->transform->vectorPosition.x)
								(*it2)->transform->Translate(vec3(pX, 0.0f, 0.0f));
							else
								(*it2)->transform->Translate(vec3(-pX, 0.0f, 0.0f));

							(*it1)->transform->CollisionWith((*it2)->transform);
							(*it2)->transform->CollisionWith((*it1)->transform);
						}
					}
				}
			}
		}
	}
}