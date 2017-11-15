#pragma once
#include "Singleton.h"
#include <mutex>
#include "Entity.h"
#include "Player.h"
class Spawner : public  Singleton<Spawner>
{
public:
	std::mutex mut;
	// Creates a predefined entity.
	Entity* CreateEntity(std::string name, glm::vec3 position, Team team);
};