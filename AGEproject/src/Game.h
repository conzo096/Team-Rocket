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
#include "Player.h"
#include "Unit.h"
class Entity;

class Game : public Singleton<Game>
{
private:
	double lastTime;

public:
	Player player;
	Entity *free_cam = new Entity;

	std::vector<Entity*> entities; //Figure out how to split this.
	Entity* SpawnUnit(glm::vec3 position, glm::vec2 size);

	void Initialise();
	void Update();
	void Render();
};