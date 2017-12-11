#include "AiPlayer.h"
#include "WorkerUnit.h"
#include <random>

void AiPlayer::Update(std::vector<std::shared_ptr<Entity>>&)
{
	CheckProperty();
	MacroCycle();
	ArmyCycle();

	// Collect any units that have been produced by your structures.
	std::vector<std::shared_ptr<Entity>> temp;
	for (std::shared_ptr<Entity>&e : entities)
	{
		if (e->GetCompatibleComponent<Structure>() != NULL)
		{
			e->GetCompatibleComponent<Structure>()->Collect(temp);

		}
		if (e->GetCompatibleComponent<Worker>() != NULL)
			balance += e->GetCompatibleComponent<Worker>()->Collect();
	}
	for (int i = 0; i < temp.size(); i++)
	{
		entities.push_back(temp[i]);
		if (temp[i]->GetCompatibleComponent<Movement>() != NULL)
			temp[i]->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(70, 0, 70));//Rally point kinda
		unitsQueued--;
		moving = false;
	}
}

void AiPlayer::CheckProperty()
{
	resources = Game::Get().FindResources();

	workerCount = 0; factoryCount = 0; vehicleBayCount = 0; hangerCount = 0; droneCount = 0; wardenCount = 0; kestralCount = 0;
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
	std::default_random_engine generator(Game::Get().GetTime());
	std::uniform_int_distribution<int> distribution(buildMin, buildMax);

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == "Worker")
		{
			if (entities[i]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1 && entities[i]->GetCompatibleComponent<Worker>()->GetAction() == Unit::Stop)
			{
				int min = 0;
				int n = 0;
				for (std::shared_ptr<Entity>& e : resources)
				{
					if (distance(entities[i]->GetPosition(), e->GetPosition()) < distance(entities[i]->GetPosition(), resources[min]->GetPosition()))
					{
						min = n;
					}
					n++;
				}
				if (resources.size() > 0)
				{
					entities[i]->GetCompatibleComponent<Worker>()->SetEntityToTarget(resources[min]);
					entities[i]->GetCompatibleComponent<Worker>()->SetAction(Unit::Harvest);
				}
			}
		}
	}
	if (entities.size() > 0)
	{
		//Build up to 16 workers, 3 a minute
		if (workerCount < 16 && workerCount < (int)(Game::Get().GetTime() / 60 * 3.0) + 1)
		{
			if (entities[0]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
			{
				entities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
			}
		}
	}
	if (workerCount > 4)
	{
		//Build a factory every 2 minutes if there are less than 4
		if (factoryCount < (int)(Game::Get().GetTime() / 60 / 2) && factoryCount < 4)
		{
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->GetName() == "Worker")
				{
					if (entities[i]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
					{
						int buildX = distribution(generator);
						int buildZ = distribution(generator);
						entities[i]->GetCompatibleComponent<Structure>()->SetSpawnPoint(glm::dvec3(buildX, 0, buildZ));
						entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
					}
					break;
				}
			}
		}
	}
	if (workerCount > 6 && factoryCount > 1)
	{
		//Build a vehicleBay every 3 minutes
		if (vehicleBayCount < (int)(Game::Get().GetTime() / 60 / 3) && vehicleBayCount < 3)
		{
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->GetName() == "Worker")
				{
					if (entities[i]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
					{
						int buildX = distribution(generator);
						int buildZ = distribution(generator);
						entities[i]->GetCompatibleComponent<Structure>()->SetSpawnPoint(glm::vec3(buildX, 0, buildZ));
						entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 1);
					}
					break;
				}
			}
		}
	}
	if (workerCount > 8 && factoryCount > 2 && vehicleBayCount > 1)
	{
		//Build a vehicleBay every 4 minutes
		if (hangerCount < (int)(Game::Get().GetTime() / 60 / 4) && hangerCount < 2)
		{
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->GetName() == "Worker")
				{
					if (entities[i]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
					{
						int buildX = distribution(generator);
						int buildZ = distribution(generator);
						entities[i]->GetCompatibleComponent<Structure>()->SetSpawnPoint(glm::vec3(buildX, 0, buildZ));
						entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 2);
					}
					break;
				}
			}
		}
	}

	//Make all production structures make units not including the main base
	for (int i = 1; i < entities.size(); i++)
	{
		if (entities[i]->GetCompatibleComponent<Structure>() != NULL && entities[i]->GetName() != "Worker" && entities[i]->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
		{
			entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
		}
	}
}

void AiPlayer::ArmyCycle()
{
}

void AiPlayer::HandleAiLogic(std::vector<std::shared_ptr<Entity>>&)
{
	// If it only has a base, build a worker.

	if (entities.size() > 0)
	{
		if (entities.size() == 1 && entities[0]->GetName() == "Base" && !workerbuilding)
		{
			workerbuilding = true;
			// Build a worker.
			entities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
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
				entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
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
				entities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
				combatUnitCount++;
				unitsQueued++;
			}
		}
	}
}
