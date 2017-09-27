#pragma once
#include <vector>

#include "GameEngine.h"
#include "Entity.h"

class Game
{
private:

public:
	std::vector<Entity> entities;//Figure out how to split this.

	void Initialise();
	void Update();
	void Render();
};