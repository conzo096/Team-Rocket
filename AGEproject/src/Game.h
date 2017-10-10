#pragma once
#include <vector>
#include <memory>
#include <chrono>

#include "GameEngine.h"
#include "Free_Camera.h"
#include "Renderable.h"
#include "Structure.h"
#include "BoundingBox.h"

class Entity;

class Game
{
private:
	static Game *instance;
	static Entity *cam;
	static std::vector<Entity*> entities;//Figure out how to split this.
	static double lastTime;
public:
	static Game *Instance()
	{
		if (!instance)
		{
			instance = new Game();
		}
		return instance;
	}

	static void SpawnUnit(glm::vec3 position, glm::vec2 size);

	static void Initialise();
	static void Update();
	static void Render();
};