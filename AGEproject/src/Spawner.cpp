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
	if (name == "Worker")
	{
		tempEntity->SetName("Worker");
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetProperties("./json/Warden.json");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempMovement = std::make_unique<GroundMovement>();
		tempMovement->SetProperties("./json/WorkerMovement.json");
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		auto tempStructure = std::make_unique<Structure>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 500;
		si.unitType = "Factory";
		tempStructure->AddSpawnInfo(si);
		si.cost = 750;
		si.unitType = "VehicleBay";
		tempStructure->AddSpawnInfo(si);
		si.cost = 1000;
		si.unitType = "Hanger";
		tempStructure->AddSpawnInfo(si);

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Worker>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		tempEntity->AddComponent(move(tempStructure));
		return tempEntity;
	}
	if (name == "Drone")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetProperties("./json/Drone.json");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempMovement = std::make_unique<GroundMovement>();
		tempMovement->SetProperties("./json/WorkerMovement.json");
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Troop>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	if (name == "Warden")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetProperties("./json/Warden.json");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempMovement = std::make_unique<GroundMovement>();
		tempMovement->SetProperties("./json/WorkerMovement.json");
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Tank>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
		return tempEntity;
	}

	if (name == "Kestrel")
	{
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetProperties("./json/Ship.json");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempMovement = std::make_unique<AirMovement>();
		tempMovement->SetProperties("./json/ShipMovement.json");
		auto tempBoundingSphere = std::make_unique<BoundingSphere>();
		tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());

		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		auto tempUnit = std::make_unique<Ship>();
		tempUnit->SetTeam(team);
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempUnit));
		tempEntity->AddComponent(move(tempMovement));
		tempEntity->AddComponent(move(tempBoundingSphere));
		tempEntity->AddComponent(move(target));
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

	if (name == "Hanger")
	{
		std::cout << "Incomplete" << std::endl;

		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("Hanger");
		tempRenderable->SetTexture("debug");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		//tempRenderable->SetProperties("./json/Shipyard.json");
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Shipyard>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 300;
		si.unitType = "Kestrel";
		tempStructure->AddSpawnInfo(si);
		tempStructure->SetTeam(team);
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());

		// Check by row.  
		for (int i = -tempBoundSphere->GetRadius(); i < tempBoundSphere->GetRadius(); i++)
		{
			// check by depth.
			for (int j = -tempBoundSphere->GetRadius(); j < tempBoundSphere->GetRadius(); j++)
			{
				// Get Point to check.
				glm::ivec2 p = glm::ivec2(position.x, position.z) + glm::ivec2(i, j);
				Game::Get().UpdateNavGrid(1, p);
			}
		}

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
		tempRenderable->SetModel("WardenPlatform");
		tempRenderable->SetTexture("debug");
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


	if (name == "Factory")
	{
		std::cout << "Incomplete" << std::endl;

		tempEntity->SetPosition(position);

		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("Factory");
		tempRenderable->SetTexture("debug");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Barracks>();
		SpawnInfo si;
		si.buildTime = 2.0f;
		si.cost = 100;
		si.unitType = "Drone";
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

	if (name == "VehicleBay")
	{
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("VehicleBay");
		tempRenderable->SetTexture("debug");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Shipyard>();
		SpawnInfo si;
		si.buildTime = 3.0f;
		si.cost = 300;
		si.unitType = "Warden";
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
