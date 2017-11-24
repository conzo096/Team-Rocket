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
#include "GroundMovement.h"
#include "ResourceHandler.h"

#include "Unit.h"
class Entity;
class Player;
class AiPlayer;
class Game : public Singleton<Game>
{
private:
	double lastTime;
	int** navGrid;
	dvec3** terrainGrid;

public:
	// User.
	Player* player;
	// Ai player.
	AiPlayer* NPC;
	Entity *free_cam = new Entity;
	std::vector<Entity*> entities; //Figure out how to split this.

	int** GetNavGrid() { return navGrid; };
	dvec3** GetTerrainGrid() { return terrainGrid; };
	vector<Entity*> FindLocalUnits(int team, dvec3 position, double sightRange);
	void Initialise();
	bool Update();
	void Render();
};