#include "Spawner.h"
#include "Renderable.h"
#include "AirMovement.h"
#include "BoundingSphere.h"
#include "Targetable.h"
#include "ShipUnit.h"
#include "WorkerUnit.h"
#include "Structure.h"
// Creates a predefined entity.
Entity* Spawner::CreateEntity(std::string name, glm::vec3 position, int team)
{
	glm::vec3 spawnPosition = position;
	Entity* tempEntity = new Entity;
	if (name == "Ship")
	{
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("../res/models/Flyer.obj");
		tempRenderable->SetTexture("FlyerUV");
		tempRenderable->SetShader("Phong");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<AirMovement>();
		tempAirMovement->SetDestination(glm::dvec3(20, 15, 20));
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
	if (name == "Worker")
	{
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("../res/models/Torus2.obj");
		tempRenderable->SetShader("Phong");
		tempRenderable->SetTexture("FlyerUV");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<AirMovement>();
		tempAirMovement->SetDestination(glm::dvec3(20, 15, 20));
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


	// Structures...

	if (name == "Base")
	{



	}

	if (name == "Shipyard")
	{
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("../res/models/Constructor.obj");
		tempRenderable->SetTexture("ConstructorUV");
		tempEntity->SetPosition(glm::vec3(3.5f, 2.5f, 3.5f));
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Structure>();
		tempStructure->SetTeam(team);
		auto tempBoundSphere = std::make_unique<BoundingSphere>();
		auto target = std::make_unique<Targetable>();
		target->SetHealth(100);
		tempEntity->AddComponent(move(target));
		tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
		tempEntity->AddComponent(move(tempBoundSphere));
		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempStructure));

		return tempEntity;


	}

	return NULL;
}
