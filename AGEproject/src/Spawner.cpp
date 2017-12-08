#include "Spawner.h"
#include "Renderable.h"
#include "AirMovement.h"
//#include "BoundingSphere.h"
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
#include "TurretRenderable.h"
// Creates a predefined entity.
std::shared_ptr<Entity> Spawner::CreateEntity(std::string name, glm::vec3 position, Team team)
{
	std::lock_guard<std::mutex> lock(mut);
	glm::vec3 spawnPosition = position;
	auto tempEntity = std::make_shared<Entity>();
	if (name == "Worker")
	{
		tempEntity->SetName("Worker");
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		if (team == Team::player)
			tempRenderable->GetMaterial().emissive = glm::vec4(74/255, 20/255, 140/255, 1);
		tempRenderable->SetProperties("./json/Worker.json");
		tempRenderable->SetPosition(vec3(0, -tempRenderable->GetModel().GetLowestYPosition(), 0));
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();

		auto tempMovement = std::make_unique<GroundMovement>();
		tempMovement->SetProperties("./json/WorkerMovement.json");
		tempMovement->SetGoal(glm::vec3(20, 2.5, 20));
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
		tempEntity->SetName(name);
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetModel("DronePlatform");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetProperties("./json/Drone.json");
		tempRenderable->SetPosition(vec3(0, -tempRenderable->GetModel().GetLowestYPosition(), 0));
		tempRenderable->UpdateTransforms();
		auto t = std::make_unique<TurretRenderable>();
		t->SetMaterial(new Material());
		t->SetProperties("./json/Drone.json");
		t->SetModel("DroneTurret");
		t->SetTexture("DroneTurretUV");
		t->SetTurnSpeed(3000);
		t->SetPosition(glm::vec3(0, -tempRenderable->GetModel().GetLowestYPosition()+ tempRenderable->GetModel().GetLargestYPosition() - t->GetModel().GetLowestYPosition(), 0));

		t->UpdateTransforms();
		tempEntity->AddComponent(move(t));


		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempMovement = std::make_unique<GroundMovement>();
		tempMovement->SetProperties("./json/WorkerMovement.json");
		//tempMovement->SetGoal(glm::vec3());
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
		tempEntity->SetName(name);
		std::cout << "Incomplete" << std::endl;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetProperties("./json/Warden.json");

		tempEntity->SetPosition(spawnPosition);
		tempRenderable->SetPosition(vec3(0, -tempRenderable->GetModel().GetLowestYPosition(), 0));
		tempRenderable->UpdateTransforms();	
		auto t = std::make_unique<TurretRenderable>();
		t->SetMaterial(new Material());
		t->SetProperties("./json/Warden.json");
		t->SetModel("WardenTurret");
		t->SetTexture("WardenTurretUV");
		t->SetTurnSpeed(3000);
		t->SetPosition(glm::vec3(tempRenderable->GetPosition().x, tempRenderable->GetPosition().y + 0.5, tempRenderable->GetPosition().y));

		t->UpdateTransforms();
		tempEntity->AddComponent(move(t));
		
		
		auto tempMovement = std::make_unique<GroundMovement>();
		tempMovement->SetProperties("./json/WorkerMovement.json");
		tempMovement->SetGoal(glm::vec3(20, 2.5, 20));
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
		tempEntity->SetName(name);
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetMaterial(new Material());
		tempRenderable->SetProperties("./json/Ship.json");
		tempRenderable->SetPosition(vec3(0, -tempRenderable->GetModel().GetLowestYPosition(), 0));
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempMovement = std::make_unique<AirMovement>();
		tempMovement->SetProperties("./json/ShipMovement.json");
		tempMovement->SetGoal(glm::vec3(20, 2.5, 20));
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
		tempEntity->SetName("Base");
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
		BoundingSphere sp;
		sp.SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		sp.SetCenter(spawnPosition);
		if (CheckGameGrid(sp))
		{
			UpdateGameGrid(sp);
		}
		else
			tempRenderable->GetMaterial().emissive = glm::vec4(1, 0, 0, 1);
		if (team == Team::player)
		{
			tempRenderable->GetMaterial().emissive = glm::vec4(0.031, 0.009, 0.059, 1);
		}
		if(team == Team::ai)
			tempRenderable->GetMaterial().emissive = glm::vec4(0.1, 0.025, 0.0125, 1);

		// Set spawn Point.
		glm::vec3 spawn = FindValidSpawnPoint(sp);
		// Check if it is invalid
		if (spawn == glm::vec3(-1))
		{
			// Spawn is somewhere weird?
			spawn = glm::vec3(50, 0, 10);
		}
		tempStructure->SetSpawnPoint(spawn);

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
		tempEntity->SetName("Hanger");
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
		BoundingSphere sp;
		sp.SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		sp.SetCenter(spawnPosition);
		if (CheckGameGrid(sp))
			UpdateGameGrid(sp);
		// Set spawn Point.
		glm::vec3 spawn = FindValidSpawnPoint(sp);
		// Check if it is invalid
		if (spawn == glm::vec3(-1))
		{
			// Spawn is somewhere weird?
			spawn = glm::vec3(50, 0, 10);
		}
		tempStructure->SetSpawnPoint(spawn);
		/*else
			tempRenderable->GetMaterial().emissive = glm::vec4(1, 0, 0, 1);*/


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
		tempEntity->SetName("Resource");
		auto tempRenderable = std::make_unique<Renderable>();
		tempEntity->SetPosition(position);
		tempRenderable->SetModel("Hanger");
		tempRenderable->SetTexture("debug");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetMaterial(new Material());
		tempRenderable->GetMaterial().diffuse = glm::vec4(1, 0, 0, 1);
		tempEntity->SetPosition(position);
		tempRenderable->UpdateTransforms();

		auto tempResource = std::make_unique<Resource>();
		tempEntity->AddComponent(move(tempResource));
		BoundingSphere sp;
		sp.SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		sp.SetCenter(spawnPosition);
		if (CheckGameGrid(sp))
		{
			UpdateGameGrid(sp);
		}
		/*else
			tempRenderable->GetMaterial().emissive = glm::vec4(1, 0, 0, 1);*/
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
		tempEntity->SetName("Factory");
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
		BoundingSphere sp;
		sp.SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		sp.SetCenter(spawnPosition);
		if (CheckGameGrid(sp))
		{
			UpdateGameGrid(sp);
		}
		/*else
			tempRenderable->GetMaterial().emissive = glm::vec4(1, 0, 0, 1);*/
			// Set spawn Point.
		glm::vec3 spawn = FindValidSpawnPoint(sp);
		// Check if it is invalid
		if (spawn == glm::vec3(-1))
		{
			// Spawn is somewhere weird?
			spawn = glm::vec3(50, 0, 10);
		}
		tempStructure->SetSpawnPoint(spawn);
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

	if (name == "VehicleBay")
	{
		tempEntity->SetName("VehicleBay");
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
		BoundingSphere sp;
		sp.SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		sp.SetCenter(spawnPosition);
		if (CheckGameGrid(sp))
		{
			UpdateGameGrid(sp);
		}
		/*else
			tempRenderable->GetMaterial().emissive = glm::vec4(1, 0, 0, 1);*/
			// Set spawn Point.
		glm::vec3 spawn = FindValidSpawnPoint(sp);
		// Check if it is invalid
		if (spawn == glm::vec3(-1))
		{
			// Spawn is somewhere weird?
			spawn = glm::vec3(50, 0, 10);
		}
		tempStructure->SetSpawnPoint(spawn);
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


glm::vec3 Spawner::FindValidSpawnPoint(BoundingSphere& sphere)
{
	// Check by row.  
	for (float i = -sphere.GetRadius(); i < sphere.GetRadius(); i++)
	{
		// check by depth.
		for (float j = -sphere.GetRadius(); j < sphere.GetRadius(); j++)
		{
			// Get Point to check.
			glm::ivec2 p = glm::ivec2(sphere.GetCenter().x, sphere.GetCenter().z) + glm::ivec2(i, j);
			// Check if it is within playable range.
			if ((p.x > 0 && p.y > 0) && (p.x < 99 && p.y < 99))
			{
				// Now check point is outside radius area.
				if ((p.x < -sphere.GetRadius()/2 || p.y > sphere.GetRadius()/2) || (p.x < -sphere.GetRadius()/2 || p.y > sphere.GetRadius()/2))
				{
					// Check if game grid is already occupied. If it is not return.
					if (Game::Get().GetNavGridValue(p) == 0)
					{
						return glm::vec3(p.x, 0, p.y);
					}

				}
			}
		}
	}
		// No point available, give invalid position.
		return glm::vec3(-1);
}

//Check if the Entity can be spawned in the area requested.
bool Spawner::CheckGameGrid(BoundingSphere& sphere)
{
	// Check by row.  
	for (float i = -sphere.GetRadius(); i <sphere.GetRadius(); i++)
	{
		// check by depth.
		for (float j = -sphere.GetRadius(); j < sphere.GetRadius(); j++)
		{
			// Get Point to check.
			glm::ivec2 p = glm::ivec2(sphere.GetCenter().x, sphere.GetCenter().z) + glm::ivec2(i, j);
			// Check if it is within playable range.
			if (p.x < 0 || p.y < 0 || p.x > 99 || p.y > 99)
			//if ((p.x > 0 && p.y > 0) && (p.x < 99 && p.y < 99))
			{
				return false;
			}
			// Check if game grid is already occupied.
			if (Game::Get().GetNavGridValue(p) == 1)
				return false;
		}
	}
	return true;
}

void Spawner::UpdateGameGrid(BoundingSphere& sphere, int value)
{
	// Check by row.  
	for (int i = -sphere.GetRadius() / 2; i <sphere.GetRadius() / 2; i++)
	{
		// check by depth.
		for (int j = -sphere.GetRadius() / 2; j < sphere.GetRadius() / 2; j++)
		{
			// Get Point to check.
			glm::ivec2 p = glm::ivec2(sphere.GetCenter().x, sphere.GetCenter().z) + glm::ivec2(i, j);
			// Update Game Grid.
			gameGridMut.lock();
			Game::Get().UpdateNavGrid(value, p);
			gameGridMut.unlock();
		}
	}
}
