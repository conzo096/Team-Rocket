#include "WorkerUnit.h"


int Worker::Collect()
{
	// If near collection point return value.
	if (glm::distance(GetParent()->GetPosition(), collectionPoint) < 1.5)
	{
		//std::cout << "Waiting for collection" << std::endl;
		waitingForCollection = true;
		if (action == Harvest)
		{
			// Reset destination.
			if (targetEntity != NULL)
			{
				glm::dvec3 t = Game::Get().ObtainNearestValidCoordinate(GetParent()->GetPosition(), targetEntity->GetPosition());
				if (t != GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
				{
				//	GetParent()->GetComponent<GroundMovement>().SetCurrentSpeed(0);
					GetParent()->GetCompatibleComponent<Movement>()->SetGoal(t);
				}
			}
		}
		int balanceToSend = resourcesHeld;
		resourcesHeld = 0;
		return balanceToSend;
	}
	return 0;
}

void Worker::AttackEntity()
{
}

void Worker::HarvestResource()
{
	if (timeSinceLastFire > fireRate)
		canShoot = true;
	
	if (targetEntity != NULL)
	{
		// Check if target is active.
		if (targetEntity->GetComponent<Targetable>().IsDead())
		{
			// If target is now depleted, head back to collection point and change state.
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(collectionPoint);
			targetEntity = NULL;
			// Dead resource change state.
			action = Move;
			harvestResource = false;
			return;
		}
		// Now check its current carry weight.
		if (resourcesHeld >= 1000 || targetEntity->GetComponent<Targetable>().IsDead())
		{
			std::cout << "Wanna go home" << std::endl;
			// Need to head back to base. 
			harvestResource = false;
			// If cp is not its current destination, set it.
			if (collectionPoint != GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(collectionPoint);
			return;
		}
		// else you can harvest.
		else if (glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) < 6 && canShoot )
		{
			canShoot = false;
			timeSinceLastFire = 0;
			// Harvest some of the resource.
			resourcesHeld += targetEntity->GetComponent<Resource>().RetrieveResource();
		}
	}
}
