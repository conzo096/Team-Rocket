#pragma once
#include "Singleton.h"
#include <mutex>
#include "Entity.h"
#include "Player.h"

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

class LevelLoader : public  Singleton<LevelLoader>
{
	void SetMovement(json j, Movement &m);
	Team GetTeam(const string team);
public:
	std::mutex mut;
	// Creates a predefined entity.
	
	void EncodeEntity(Entity* entity, json &objects);

	void LoadLevel(const std::string jsonFile, vector<Entity*> &playerEntities, vector<Entity*> &NPCEntities, vector<Entity*> &neutralEntities);
	void SaveLevel(const std::string jsonFile, vector<Entity*> &playerEntities, vector<Entity*> &NPCEntities, vector<Entity*> &neutralEntities);
};
