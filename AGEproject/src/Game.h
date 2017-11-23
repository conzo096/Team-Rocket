#pragma once
#include <vector>
#include <memory>
#include <chrono>

#include "Singleton.h"
#include "GameEngine.h"
#include "Free_Camera.h"
#include "Game_Camera.h"
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
	bool freeCamEnabled;
	bool keyHeld;

public:
	// User.
	Player* player;
	// Ai player.
	AiPlayer* NPC;
	Entity *game_cam = new Entity;
	Entity *free_cam = new Entity;
	std::vector<Entity*> entities; //Figure out how to split this.

	int** GetNavGrid() { return navGrid; };
	dvec3** GetTerrainGrid() { return terrainGrid; };
	void Initialise();
	bool Update();
	void Render();
};