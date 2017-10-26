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
				e->GetComponent<Structure>().Collect(temp);
		}
		for (Entity*&e : temp)
			entities.push_back(e);
	}


	// Ai decisions are handled here.
	void HandleAiLogic(std::vector<Entity*>& enemyList)
	{
		// If structure is still around.
		if (entities.size() >0)
		{
			// Spawn an entity.
			if (entities[0]->GetCompatibleComponent<Structure>() != NULL)
				if (entities[0]->GetCompatibleComponent<Structure>()->GetQueueSize() < 3)
					entities[0]->GetCompatibleComponent<Structure>()->AddProduct("Ship", 3);
		}
		for (Entity*& e : entities)
		{
			if (e->GetCompatibleComponent<Unit>() != NULL)
			{
				if (enemyList.size() > 3)
				{
					e->GetCompatibleComponent<Unit>()->SetEntityToTarget(enemyList[2]);
					e->GetCompatibleComponent<Unit>()->SetAction(Unit::Attack);
				}
			}
		}
	}
};