// This is the base class for player interaction with the game.
#pragma once
#include <vector>
#include "Entity.h"
#include "Game_Camera.h"
#include "Free_Camera.h"
enum Team
{
	player, ai, neutral
};

class Player
{

protected:
	// Team the player is on.
	Team team;
	// List of entities that this player contains.
	std::vector<std::shared_ptr<Entity>>entities;
	std::vector<std::shared_ptr<Entity>> resources;
	// The units that the player is controlling.
	std::vector<std::shared_ptr<Entity>> selectedEntities;
	// What enemy unit the player last choose.
	std::shared_ptr<Entity> selectedEnemy;
	// Most recent selected friendly unit
	std::shared_ptr<Entity> selectedFriendly;
	// How much $$$$ the player has, used for unit spawning.
	int balance = 200;

	// Used to limit key calls.
	float timeElapsed = 0.5f;
	float lastClock = 0.0f;
	// should a ghost building be displayed?
	bool showGhostBuilding = false;
	// Is this area valid for the player to request a building?
	bool validSpawn = false;
	int buildingType; 
	// Ghost building.
	Entity ghostBuilding;
	// cost to build object.
	float buildingCost;
	bool attackMove = true;
public:
	

	Team GetTeam() { return team; }
	void SetTeam(Team t) { team = t; }
	std::vector<std::shared_ptr<Entity>>& GetEntities() { return entities; }
	std::vector<std::shared_ptr<Entity>>& GetSelectedEntities() { return selectedEntities; }
	int GetBalance() { return balance; }
	void SetBalance(const int val) { balance = val; }
	std::shared_ptr<Entity>& GetSelectedEntity() { return selectedEnemy; }
	std::shared_ptr<Entity>& GetSelectedFriendlyEntity() { return selectedFriendly; }
	// Change from int type.
	void UpdateGhostBuilding(int type);


	void Update(std::vector<std::shared_ptr<Entity>>& enemyList);
	void HandleInput(std::vector<std::shared_ptr<Entity>>& enemyList);
	void Render();


	// Sort entities by closest to the camera?
	void SortEntities(Free_Camera& camera);
	void SortEntities(Game_Camera& camera);



};

