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
#include "UIManager.h"
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


	// Contains a list of just the neutral entities in the scene.
	std::vector <Entity*> neutralEntities;
	
	// Contains a list of all entities in the scene.
	std::vector <Entity*> allEntities;

	// Particle that appears when the user selects a location.
	BulletParticle location;
	// How long the particle will last for before stop being rendered.
	double duration = 3.0;

	// UI class - contains screen data info, should be on a seperate frame buffer?
	UIManager ui;

	// Get player class.
	Player& GetPlayer() { return *player; }

	int** GetNavGrid() { return navGrid; };
	int GetNavGridValue(glm::ivec2 pos) { return navGrid[pos.x][pos.y]; }
	void UpdateNavGrid(int val, glm::ivec2 pos);


	dvec3** GetTerrainGrid() { return terrainGrid; };
	vector<Entity*> FindLocalUnits(int team, dvec3 position, double sightRange);
	void Initialise();
	bool Update();
	void Render();

	void HandleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	//void UpdateEntityList(int start, int end, double deltaTime);
};