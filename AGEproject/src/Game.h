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
	// Particle that appears when the user selects a location.
	BulletParticle location;
	// How long the particle will last for before stop being rendered.
	double duration = 3.0;
	int** GetNavGrid() { return navGrid; };
	dvec3** GetTerrainGrid() { return terrainGrid; };
	void Initialise();
	bool Update();
	void Render();


	//void UpdateEntityList(int start, int end, double deltaTime);
};