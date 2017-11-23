#include "Spawner.h"
#include "Renderable.h"
#include "AirMovement.h"
#include "BoundingSphere.h"
#include "Targetable.h"
#include "ShipUnit.h"
#include "WorkerUnit.h"
#include "TroopUnit.h"
#include "Structure.h"
#include "TankUnit.h"
#include "VehicleDepotStructure.h"
#include "ShipyardStructure.h"
#include "Barracks.h"
#include "BaseStructure.h"
#include "Resource.h"
#include "GroundMovement.h"
// Creates a predefined entity.
Entity* Spawner::CreateEntity(std::string name, glm::vec3 position, Team team)
{
	std::lock_guard<std::mutex> lock(mut);
	glm::vec3 spawnPosition = position;
	Entity* tempEntity = new Entity;
	if (name == "Ship")
	{
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("Ship");
		tempRenderable->SetTexture("FlyerUV");
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetShader("Phong");
		tempRenderable->SetPosition(dvec3(0, 0.5, 0));
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<AirMovement>();
		tempAirMovement->SetDestination(glm::dvec3(90, 0, 90));
		tempAirMovement->SetSpeed(15.0);
		tempAirMovement->SetAcceleration(0.5);
		tempAirMovement->SetTurnSpeed(200.0);
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Ship>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempAirMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	if (name == "Troop")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("BillBoard");
		tempRenderable->SetTexture("FlyerUV");
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetShader("Phong");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<GroundMovement>();
		tempAirMovement->SetDestination(glm::dvec3(20, 15, 20));
		tempAirMovement->SetSpeed(15.0);
		tempAirMovement->SetAcceleration(0.5);
		tempAirMovement->SetTurnSpeed(200.0);
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Troop>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempAirMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	if (name == "Tank")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("BillBoard");
		tempRenderable->SetTexture("FlyerUV");
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetShader("Phong");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<GroundMovement>();
		tempAirMovement->SetDestination(glm::dvec3(20, 15, 20));
		tempAirMovement->SetSpeed(15.0);
		tempAirMovement->SetAcceleration(0.5);
		tempAirMovement->SetTurnSpeed(200.0);
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Tank>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempAirMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	if (name == "Worker")
	{
		tempEntity->SetName("Worker");
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("Worker");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetTexture("WorkerUV");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<GroundMovement>();
		tempAirMovement->SetGoal(glm::dvec3(20, 15, 20));
		tempAirMovement->SetSpeed(15.0);
		tempAirMovement->SetAcceleration(0.1);
		tempAirMovement->SetTurnSpeed(200.0);
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		auto tempStructure = std::make_unique<Structure>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 1500;
		si.unitType = "Shipyard";
		tempStructure->AddSpawnInfo(si);
		si.cost = 750;
		si.unitType = "Barracks";
		tempStructure->AddSpawnInfo(si);
		si.cost = 2000;
		si.unitType = "VehicleDepot";
		tempStructure->AddSpawnInfo(si);

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Worker>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempAirMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		tempEntity->AddComponent(move(tempStructure));





		return tempEntity;
	}

	// Structures...

	if (name == "Base")
	{
		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("Base");
		tempRenderable->SetTexture("ConstructorUV");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Shipyard>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 300;
		si.unitType = "Worker";
		tempStructure->AddSpawnInfo(si);
		tempStructure->SetTeam(team);
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		tempEntity->AddComponent(move(tempBoundSphere));
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempStructure));
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		tempEntity->AddComponent(move(target));

		return tempEntity;
	}

	if (name == "Shipyard")
	{
		std::cout << "Incomplete" << std::endl;

		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("Shipyard");
		tempRenderable->SetTexture("ConstructorUV");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Shipyard>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 300;
		si.unitType = "Ship";
		tempStructure->AddSpawnInfo(si);
		tempStructure->SetTeam(team);
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		tempEntity->AddComponent(move(tempBoundSphere));
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempStructure));
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	if (name == "Resource")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("Shipyard");
		tempRenderable->SetTexture("ConstructorUV");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempRenderable->GetMaterial().diffuse = glm::vec4(1, 0, 0, 1);
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();

		auto tempResource = std::make_unique<Resource>();
		tempEntity->AddComponent(move(tempResource));
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		tempEntity->AddComponent(move(tempBoundSphere));
		tempEntity->AddComponent(move(tempRenderable));
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}


	if (name == "Barracks")
	{
		std::cout << "Incomplete" << std::endl;

		tempEntity->SetPosition(position);

		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("Torus");
		tempRenderable->SetTexture("ConstructorUV");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Barracks>();
		SpawnInfo si;
		si.buildTime = 2.0f;
		si.cost = 100;
		si.unitType = "Troop";
		tempStructure->AddSpawnInfo(si);
		tempStructure->SetTeam(team);
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		tempEntity->AddComponent(move(tempBoundSphere));
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempStructure));
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		tempEntity->AddComponent(move(target));
		return tempEntity;

		return tempEntity;
	}

	if (name == "VehicleDepot")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("Torus");
		tempRenderable->SetTexture("ConstructorUV");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Shipyard>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 300;
		si.unitType = "Tank";
		tempStructure->AddSpawnInfo(si);
		tempStructure->SetTeam(team);
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		tempEntity->AddComponent(move(tempBoundSphere));
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempStructure));
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	return NULL;
}
