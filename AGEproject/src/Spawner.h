#pragma once
#include "Singleton.h"
#include "Entity.h"
class Spawner : public  Singleton<Spawner>
{
public:
	// Creates a predefined entity.
	Entity* CreateEntity(std::string name, glm::vec3 position, int team);
};