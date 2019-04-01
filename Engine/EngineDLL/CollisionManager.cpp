#include "CollisionManager.h"

CollisionManager * CollisionManager::instance = NULL;

CollisionManager::CollisionManager()
{
	for (int i = 0; i < Count; i++)
	{
		list<Entity*> list;
		listsOfEntities.push_back(list);
	}

	for (int i = 0; i < Count; i++)
		for (int j = 0; j < Count; j++)
			tagsRelation[i][j] = false;
}

void CollisionManager::AddEntity(Entity* entity)
{
	listsOfEntities[entity->GetTag()].push_back(entity);
}

void CollisionManager::SetRelation(int layer1, int layer2)
{
	if (layer1 <= layer2)
		tagsRelation[layer1][layer2] = true;
	else
		tagsRelation[layer2][layer1] = true;
}

void CollisionManager::DetectCollisions()
{
	for (int i = 0; i < Count; i++)
		for (int j = i; j < Count; j++)
			if (tagsRelation[i][j] == 1)
				MakeTheRealDetection(i, j);
}

void CollisionManager::MakeTheRealDetection(int index1, int index2)
{
	// If any of this lists has no Entity just return.
	if (listsOfEntities[index1].size() == 0
	 || listsOfEntities[index2].size() == 0)
		return;
	
	for(list<Entity*>::iterator it1 = listsOfEntities[index1].begin(); it1 != listsOfEntities[index1].end(); it1++)
	{
		glm::vec2 col1 = (*it1)->GetColProps();

		for (list<Entity*>::iterator it2 = listsOfEntities[index2].begin(); it2 != listsOfEntities[index2].end(); it2++)
		{
			if (!(*it1)->IsStatic() || !(*it2)->IsStatic()) // If both are static don't check collision.
			{
				glm::vec3 diff = (*it2)->GetPosition() - (*it1)->GetPosition();

				float dX = abs(diff.x);
				float dY = abs(diff.y);

				glm::vec2 col2 = (*it2)->GetColProps();
				
				if (dX < col1.x / 2 + col2.x / 2
				&&  dY < col1.y / 2 + col2.y / 2)
				{
					float pX = col1.x / 2 + col2.x / 2 - diff.x;
					float pY = col1.y / 2 + col2.y / 2 - diff.y;

					if (pX > pY)
					{
						// Vertical Collision
						if (!(*it1)->IsStatic())
						{
							if (!(*it2)->IsStatic()) // Any of them are static.
							{
								// Calculations with mass.
								float m1 = (*it1)->GetMass();
								float m2 = (*it2)->GetMass();

								float mPercentage = (m1 + m2) / m1;
								float move = pY / mPercentage;

								(*it2)->Translate(glm::vec3(0.0f, move, 0.0f));
								(*it1)->Translate(glm::vec3(0.0f, -pY + move, 0.0f));

								(*it1)->CollisionWith(*it2);
								(*it2)->CollisionWith(*it1);
							}
							else // Entity 1 is pushed back
							{
								if ((*it1)->GetPosition().y > (*it2)->GetPosition().y)
									(*it1)->Translate(glm::vec3(0.0f, pY, 0.0f));
								else
									(*it1)->Translate(glm::vec3(0.0f, -pY, 0.0f));

								(*it1)->CollisionWith(*it2);
								(*it2)->CollisionWith(*it1);
							}
						}
						else // Entity 2 is pushed back
						{
							if ((*it2)->GetPosition().y > (*it1)->GetPosition().y)
								(*it2)->Translate(glm::vec3(0.0f, pY, 0.0f));
							else
								(*it2)->Translate(glm::vec3(0.0f, -pY, 0.0f));

							(*it1)->CollisionWith(*it2);
							(*it2)->CollisionWith(*it1);
						}
					}
					else
					{
						// Horizontal Collision
						if (!(*it1)->IsStatic())
						{
							if (!(*it2)->IsStatic()) // Any of them are static.
							{
								// Acá haría los cálculos teniendo en cuenta las masas.
								float m1 = (*it1)->GetMass();
								float m2 = (*it2)->GetMass();

								float mPercentage = (m1 + m2) / m1;
								float move = pX / mPercentage;

								(*it2)->Translate(glm::vec3(move, 0.0f, 0.0f));
								(*it1)->Translate(glm::vec3(-pX + move, 0.0f, 0.0f));

								(*it1)->CollisionWith(*it2);
								(*it2)->CollisionWith(*it1);
							}
							else // Entity 1 is pushed back
							{
								if ((*it1)->GetPosition().x > (*it2)->GetPosition().x)
									(*it1)->Translate(glm::vec3(pX, 0.0f, 0.0f));
								else
									(*it1)->Translate(glm::vec3(-pX, 0.0f, 0.0f));

								(*it1)->CollisionWith(*it2);
								(*it2)->CollisionWith(*it1);
							}
						}
						else // Entity 2 is pushed back
						{
							if ((*it2)->GetPosition().x > (*it1)->GetPosition().x)
								(*it2)->Translate(glm::vec3(pX, 0.0f, 0.0f));
							else
								(*it2)->Translate(glm::vec3(-pX, 0.0f, 0.0f));

							(*it1)->CollisionWith(*it2);
							(*it2)->CollisionWith(*it1);
						}
					}
				}
			}
		}
	}
}