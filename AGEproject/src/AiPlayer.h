#pragma once
#include "Player.h"
#include "BoundingSphere.h"
#include "Game.h"
class AiPlayer : public Player
{
	bool workerbuilding = false;
	bool moving = false;
	bool factoryMade = false;
	int unitsQueued = 0;
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
			{
				e->GetCompatibleComponent<Structure>()->Collect(temp);

			}
		}
		for (int i=0; i<temp.size();i++)
		{
			//if(temp[i]->GetCompatibleComponent<Movement>()!=NULL)
			//	temp[i]->GetCompatibleComponent<Movement>()->SetGoal(temp[i]->GetPosition());
			entities.push_back(temp[i]);
			unitsQueued--;
			moving = false;
		}
	}


	// Ai decisions are handled here.
	void HandleAiLogic(std::vector<Entity*>& enemyList)
	{
		// If it only has a base, build a worker.

		if (entities.size() > 0)
		{
			if (entities.size() == 1 && entities[0]->GetName() == "Base" && !workerbuilding)
			{
				workerbuilding = true;
				// Build a worker.
				entities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, glm::vec3(75, 0, 75));
				unitsQueued++;
			}
		}

		// Set all units to same destination.
		for (int i=0; i < entities.size();i++)
		{

			if (entities[i]->GetCompatibleComponent<Structure>() == NULL && !moving)
			{
				entities[i]->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(65+(i*2), 0, 65 + (i * 2)));
				moving = true;
			}
		}

		// Tell worker to spawn a factory object.
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->GetName() == "Factory")
			{
				break;
				factoryMade = true;
			}
		}
		if (factoryMade == false)
		{
			// Get first worker.
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->GetName() == "Worker")
				{
					// Add a factory at a set position.
					entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, glm::vec3(75, 0, 15));
					unitsQueued++;
					factoryMade = true;
				}
			}
		}


		// Spawn up to 3 units if there is not enough.
		int combatUnitCount =0;
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->GetCompatibleComponent<Structure>() == NULL)
			{
				combatUnitCount++;
			}
		}
		// If there are not enough combat units, spawn some more.
		if (combatUnitCount < 3 && unitsQueued < 5)
		{
			// Find a factory.
			for (int i = 0; i < entities.size(); i++)
			{
			//	std::cout << entities[i]->GetName() << std::endl;
				if (entities[i]->GetName() == "Factory")
				{
					entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, glm::vec3(70, 0, 15));
					combatUnitCount++;
					unitsQueued++;
				}
			}
		}
		int inRange;
		// If any enemies are in range of any structures attack them.
		for (int i = 0; i < enemyList.size(); i++)
		{
			//check through npc list.
			for (int j = 0; j < entities.size(); j++)
			{
				// if entity is a structure and not a worker..
				if (entities[j]->GetName() != "Worker" && entities[j]->GetCompatibleComponent<Structure>() != NULL)
				{
					if (glm::distance(entities[j]->GetPosition(), enemyList[i]->GetPosition()) < 6)
					{
						inRange = true;
						break;
					}
				}
			}
			// If enemy is in range.
			if (inRange)
			{
				// Send combat units to attack then leave
				for (int j = 0; j < entities.size(); j++)
				{
					// if entity is a structure and not a worker..
					if (entities[j]->GetCompatibleComponent<Structure>() == NULL)
					{
						entities[j]->GetCompatibleComponent<Unit>()->SetEntityToTarget(enemyList[i]);
						entities[j]->GetCompatibleComponent<Unit>()->SetAction(Unit::Attack);
					}
				}
			}
		}

	}
};