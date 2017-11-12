#pragma once
#include "Unit.h"
#include "Resource.h"
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
	glm::dvec3 collectionPoint = glm::vec3(50,50,50);

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
		if (targetEntity != NULL && targetEntity->GetCompatibleComponent<Resource>() != NULL)
		{
			if  (returnToResource)
			{
				GetParent()->GetCompatibleComponent<Movement>()->SetDestination(targetEntity->GetPosition());
				// If within range, Collect some of the resource.
				if (glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) < 16 && canShoot)
				{
					canShoot = false;
					timeSinceLastFire = 0;
					// Take some of the resource from the targetEntity.
					resourcesHeld += targetEntity->GetCompatibleComponent<Resource>()->RetrieveResource();
					if (resourcesHeld >= 500)
					{
						walkToBase = true;
						returnToResource = false;
					}
				}
			}
			// If the resource contained in this unit is within a certain amount, empty at base, then return.
			if (walkToBase)
			{
				// Get it to walk to base.
 				GetParent()->GetCompatibleComponent<Movement>()->SetDestination(collectionPoint);
				// If it close enough to destination, give resource to team.
				if (glm::distance(GetParent()->GetPosition(), collectionPoint) < 2)
				{
					std::cout << "Deposit money" << std::endl;
					waitingForCollection = true;
					walkToBase = false;
					returnToResource = true;
				}
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
			returnToResource = true;
			walkToBase = false;
			waitingForCollection = false;
			int balanceToSend = resourcesHeld;
			resourcesHeld = 0;
			//GetParent()->GetCompatibleComponent<Movement>()->SetDestination(targetEntity->GetPosition());
			return balanceToSend;
		}
		else
			return 0;
	}


};