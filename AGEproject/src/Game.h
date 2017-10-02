#pragma once
#include <vector>
#include <memory>
#include <chrono>

#include "GameEngine.h"
#include "Entity.h"
#include "Renderable.h"

class Game
{
private:
	static Game *instance;
	static std::vector<Entity*> entities;//Figure out how to split this.
	static float lastTime;
public:
	static Game *Instance()
	{
		if (!instance)
		{
			instance = new Game();
		}
		return instance;
	}

	static void Initialise();
	static void Update();
	static void Render();
};