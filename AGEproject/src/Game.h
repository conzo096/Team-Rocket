#pragma once
#include <vector>
#include <memory>
#include <chrono>

#include "Singleton.h"
#include "GameEngine.h"
#include "Free_Camera.h"
#include "Menu_Camera.h"
#include "Renderable.h"
#include "Structure.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "AirMovement.h"

class Entity;

class Game : public Singleton<Game>
{
private:
	std::vector<Entity*> entities; //Figure out how to split this.
	double lastTime;

public:
	Entity *free_cam = new Entity;

	void SpawnUnit(glm::vec3 position, glm::vec2 size);

	void Initialise();
	bool Update();
	void Render();
};