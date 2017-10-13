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
#include "AirMovement.h"

class Entity;

class Game : public Singleton<Game>
{
private:
	Entity *free_cam = new Entity;
	static std::vector<Entity*> entities;//Figure out how to split this.
	static double lastTime;

public:

	void SpawnUnit(glm::vec3 position, glm::vec2 size);

	void Initialise();
	void Update();
	void Render();
};