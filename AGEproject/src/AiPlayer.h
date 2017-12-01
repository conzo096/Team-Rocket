#pragma once
#include "Player.h"
#include "BoundingSphere.h"
#include "Game.h"
class AiPlayer : public Player
{

public:

	// Update the ai player.
	void Update(std::vector<Entity*>& enemyList)
	{
		HandleAiLogic(enemyList);
		// Collect any units that have been produced by your structures.
		std::vector<Entity*> temp;
		for (Entity*&e : entities)
		{
			if (e->GetCompatibleComponent<Structure>() != NULL)
				e->GetCompatibleComponent<Structure>()->Collect(temp);
		}
		for (Entity*&e : temp)
		{
			e->SetScale(glm::vec3(3, 3, 3));
			entities.push_back(e);
		}
	}


	// Ai decisions are handled here.
	void HandleAiLogic(std::vector<Entity*>& enemyList)
	{
		//if (entities.size() < 2 && entities.size() > 0)
		//{
		//	// Spawn an entity.
		//	if (entities[0]->GetCompatibleComponent<Structure>() != NULL)
		//		if (entities[0]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
		//			entities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance,0);

		//}
	/*	for (Entity*& e : entities)
		{
			if (e->GetCompatibleComponent<Unit>() != NULL)
			{
				if (enemyList.size() > 3)
				{
					e->GetCompatibleComponent<Unit>()->SetEntityToTarget(enemyList[2]);
					e->GetCompatibleComponent<Unit>()->SetAction(Unit::Attack);
				}
			}
		}*/
	}
};