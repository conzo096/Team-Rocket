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


	// True if it is to harvest resource, false if it is to head back to cp.
	bool harvestResource = true;


	// Location is needs to head towards to drop of resource.
	glm::dvec3 collectionPoint = glm::vec3(10,0,50);

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Worker() : Unit("Worker")
	{

	};
	~Worker() {};


	glm::vec3 GetCollectionPoint() { return collectionPoint; }
	void SetCollectionPoint(glm::vec3 cp) { collectionPoint = cp; }





	// This class does not attack any entities unless it is the a resource.
	void AttackEntity() override;
	void HarvestResource() override;
	// Players collect resource from worker once it is in correct area.
	int Collect();


};