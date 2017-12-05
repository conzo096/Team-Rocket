#pragma once
#include "Unit.h"
#include "Resource.h"
#include "GroundMovement.h"
#include "Game.h"
class Worker : public Unit
{
private:
	// How much resource is this worker unit carrying?
	int resourcesHeld = 0;
	// conditions the worker has to follow.
	bool returnToResource = true;
	bool walkToBase = false;
	bool waitingForCollection = false;

	// Location is needs to head towards to drop of resource.
	glm::dvec3 collectionPoint = glm::vec3(10,0,50);

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Worker() : Unit("Worker")
	{

	};
	~Worker() {};


	// This class does not attack any entities unless it is the a resource.
	void AttackEntity() override
	{
		if (timeSinceLastFire > fireRate)
			canShoot = true;
		if (targetEntity->GetComponent<Targetable>().IsDead())
		{
			targetEntity = NULL;
		}
		// Check if the target entity is a valid target.
		if (targetEntity != NULL)
		{
			if (targetEntity->GetCompatibleComponent<Resource>() != NULL)
			{
				// Walk towards resource.
				if (returnToResource)
				{
					if (GetParent()->GetCompatibleComponent<Movement>()->GetGoal() != targetEntity->GetPosition())
						GetParent()->GetCompatibleComponent<Movement>()->SetGoal(targetEntity->GetPosition());
					// Check if worker is within a close distance of the resource.
					if (glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) < 6 && canShoot)
					{
						canShoot = false;
						timeSinceLastFire = 0;
						// Harvest some of the resource.
						resourcesHeld += targetEntity->GetComponent<Resource>().RetrieveResource();
						// Check if worker is above capacity now.
						if (resourcesHeld >= 1000 || targetEntity->GetComponent<Targetable>().IsDead())
						{
							GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
							// Now the worker has to walk back to collection point.
							walkToBase = true;
							returnToResource = false;
							std::cout << "Returning to base." << std::endl;
						}
					}
				}
			}
		}
		if(walkToBase)
		{
			std::cout << "Walk to base." << std::endl;
			if(GetParent()->GetPosition() == GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(collectionPoint);
			//Check if it is within range of collection point
			if (glm::distance(GetParent()->GetPosition(), collectionPoint) < 1.5)
			{
				std::cout << "Waiting for collection" << std::endl;
				waitingForCollection = true;
				GetParent()->GetComponent<GroundMovement>().SetCurrentSpeed(0);
			}
		}
	}

	// Players collect resource from worker once it is in correct area.
	int Collect()
	{
		// Collect if at correct place, otherwise return 0.
		if (waitingForCollection == true)
		{
			std::cout << "Sending: " << resourcesHeld << std::endl;
			glm::vec3 t = Game::Get().ObtainNearestValidCoordinate(GetParent()->GetPosition(), targetEntity->GetPosition());
			//std::cout << t.x << ", " << t.z << std::endl;
			// Reset destination.
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(t);
			returnToResource = true;
			walkToBase = false;
			waitingForCollection = false;
			int balanceToSend = resourcesHeld;
			resourcesHeld = 0;
			return balanceToSend;
		}
		else
			return 0;
	}


};