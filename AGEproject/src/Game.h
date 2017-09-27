#pragma once
#include <vector>

#include "GameEngine.h"
#include "Entity.h"
#include "Renderable.h"

class Game
{
private:
	static Game *instance;

public:
	static Game *Instance()
	{
		if (!instance)
		{
			instance = new Game();
		}
		return instance;
	}

	std::vector<Entity> entities;//Figure out how to split this.

	static void Initialise();
	static void Update();
	static void Render();
};