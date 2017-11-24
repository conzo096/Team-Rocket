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


	// Contains a list of just the neutral entities in the scene.
	std::vector <Entity*> neutralEntities;
	
	// Contains a list of all entities in the scene.
	std::vector <Entity*> allEntities;

	// Particle that appears when the user selects a location.
	BulletParticle location;
	// How long the particle will last for before stop being rendered.
	double duration = 3.0;
	int** GetNavGrid() { return navGrid; };
	int GetNavGridValue(glm::ivec2 pos) { return navGrid[pos.x][pos.y]; }
	void UpdateNavGrid(int val, glm::ivec2 pos);


	dvec3** GetTerrainGrid() { return terrainGrid; };
	void Initialise();
	bool Update();
	void Render();

	void HandleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	//void UpdateEntityList(int start, int end, double deltaTime);
};