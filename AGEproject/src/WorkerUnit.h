#pragma once
#include "Unit.h"
#include "Resource.h"
#include "GroundMovement.h"
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
	glm::dvec3 collectionPoint = glm::vec3(0,0,50);

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

		// Check if the target entity is a valid target.
		if (targetEntity != NULL)
		{
			if (targetEntity->GetComponent<Targetable>().IsDead())
			{
				targetEntity = NULL;
			}

			if (targetEntity->GetCompatibleComponent<Resource>() != NULL)
			{
				// Walk towards resource.
				if (returnToResource)
				{
					if(GetParent()->GetCompatibleComponent<Movement>()->GetGoal() != targetEntity->GetPosition())
						GetParent()->GetCompatibleComponent<Movement>()->SetGoal(targetEntity->GetPosition());
					// Check if worker is within a close distance of the resource.
					if(glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) < 6 && canShoot)
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
				else if(walkToBase)
				{
					if(GetParent()->GetPosition() == GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
						GetParent()->GetCompatibleComponent<Movement>()->SetGoal(collectionPoint);
					//Check if it is within range of collection point
					if (glm::distance(GetParent()->GetPosition(), collectionPoint) < 1.3)
					{
						std::cout << "Waiting for collection" << std::endl;
						waitingForCollection = true;
						GetParent()->GetComponent<GroundMovement>().SetCurrentSpeed(0);
					}
				}
			}
		}



		//if (targetEntity != NULL && targetEntity->GetCompatibleComponent<Resource>() != NULL)
		//{
		//	if (returnToResource)
		//	{
		//		GetParent()->GetComponent<GroundMovement>().SetGoal(targetEntity->GetPosition());
		//		// If within range, Collect some of the resource.
		//		if (glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) < 16 && canShoot)
		//		{
		//			canShoot = false;
		//			timeSinceLastFire = 0;
		//			// Take some of the resource from the targetEntity.
		//			resourcesHeld += targetEntity->GetCompatibleComponent<Resource>()->RetrieveResource();
		//			if (resourcesHeld >= 1000 || targetEntity->GetComponent<Targetable>().IsDead() && walkToBase == false)
		//			{
		//				//GetParent()->GetComponent<GroundMovement>().SetGoal(glm::vec3(GetPosition().x,0,GetPosition().z));
		//				walkToBase = true;
		//				returnToResource = false;
		//				std::cout << "Returning to base." << std::endl;
		//				//GetParent()->GetComponent<GroundMovement>().SetGoal(collectionPoint);
		//			}
		//			if (targetEntity->GetComponent<Targetable>().IsDead())
		//			{
		//				targetEntity = NULL;
		//			}
		//		}
		//	}
		//	// If the resource contained in this unit is within a certain amount, empty at base, then return.
		//	if (walkToBase)
		//	{
		//		// Get it to walk to base.
		//		GetParent()->GetComponent<GroundMovement>().SetGoal(collectionPoint);
		//		// If it close enough to destination, give resource to team.
		//		if (glm::distance(GetParent()->GetPosition(), collectionPoint) < 5)
		//		{
		//			GetParent()->GetComponent<GroundMovement>().SetCurrentSpeed(0);
		//			std::cout << "Depositing resource." << std::endl;
		//			waitingForCollection = true;
		//			walkToBase = false;
		//			returnToResource = true;
		//			std::cout << "Returning to resource." << std::endl;

		//		}
		//	}
		//}
	}

	// Players collect resource from worker once it is in correct area.
	int Collect()
	{
		// Collect if at correct place, otherwise return 0.
		if (waitingForCollection == true)
		{
			// Reset destination.
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
			std::cout << "Sending: " << resourcesHeld << std::endl;
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