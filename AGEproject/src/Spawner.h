#pragma once
#include "Singleton.h"
#include <mutex>
#include "Entity.h"
#include "Player.h"
#include "BoundingSphere.h"
class Spawner : public  Singleton<Spawner>
{
public:
	std::mutex mut;
	std::mutex gameGridMut;
	//Check if the Entity can be spawned in the area requested.
	bool CheckGameGrid(BoundingSphere& sphere);
	void UpdateGameGrid(BoundingSphere& sphere, int value = 1);
	// Find a valid spawnPoint just outside bounding radius.
	glm::vec3 FindValidSpawnPoint(BoundingSphere& sphere);
	

	// More accurate approach
	bool CheckGameGrid(glm::vec3 position, int length, int width);
	void UpdateGameGrid(glm::vec3 position, int length, int width, int value = 1);
	glm::vec3 FindValidSpawnPoint(glm::vec3 position, int length, int width);
	// Creates a predefined entity.
	std::shared_ptr<Entity> CreateEntity(std::string name, glm::vec3 position, Team team);

};