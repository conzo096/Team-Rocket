#include "AiPlayer.h"

void AiPlayer::Update(std::vector<Entity*>& enemyList)
{
	CheckProperty();
	MacroCycle();
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
	for (int i = 0; i < temp.size(); i++)
	{
		//if(temp[i]->GetCompatibleComponent<Movement>()!=NULL)
		//	temp[i]->GetCompatibleComponent<Movement>()->SetGoal(temp[i]->GetPosition());
		entities.push_back(temp[i]);
		unitsQueued--;
		moving = false;
	}
}

void AiPlayer::CheckProperty()
{
	workerCount, factoryCount, vehicleBayCount, hangerCount, droneCount, wardenCount, kestralCount = 0;
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == "Worker")
		{
			workerCount++;
		}
		else
			if (entities[i]->GetName() == "Drone")
			{
				droneCount++;
			}
			else
				if (entities[i]->GetName() == "Warden")
				{
					wardenCount++;
				}
				else
					if (entities[i]->GetName() == "Kestral")
					{
						kestralCount++;
					}
					else
						if (entities[i]->GetName() == "Factory")
						{
							factoryCount++;
						}
						else
							if (entities[i]->GetName() == "VehicleBay")
							{
								vehicleBayCount++;
							}
							else
								if (entities[i]->GetName() == "Hanger")
								{
									hangerCount++;
								}
	}
}

void AiPlayer::MacroCycle()
{
	if (entities.size() > 0)
	{
		//Build up to 16 workers, 3 a minute
		if (workerCount < 16 && workerCount < (int)(Game::Get().GetTime() * 3))
		{
			if (entities[0]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
			{
				entities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, entities[0]->GetPosition());
			}
		}
	}
	if (workerCount > 4)
	{
		//Build a factory every 3 minutes
		if (factoryCount < (int)(Game::Get().GetTime() / 3))
		{
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->GetName() == "Worker")
				{
					int buildX = rand() % 20 + 80;
					int buildZ = rand() % 20 + 80;
					entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, glm::vec3(buildX, 0, buildZ));
					break;
				}
			}
		}
	}
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetCompatibleComponent<Structure>() != NULL && entities[i]->GetName() != "Worker")
		{
			entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, entities[i]->GetPosition());
		}
	}
}

void AiPlayer::ArmyCycle()
{
}

void AiPlayer::HandleAiLogic(std::vector<Entity*>& enemyList)
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
	for (int i = 0; i < entities.size(); i++)
	{

		if (entities[i]->GetCompatibleComponent<Structure>() == NULL)
		{
			entities[i]->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(65 + (i * 2), 2.5, 50));
			moving = true;
		}
		// Factory made
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
	int combatUnitCount = 0;
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
}
