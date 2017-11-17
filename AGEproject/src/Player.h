// This is the base class for player interaction with the game.
#pragma once
#include <vector>
#include "Entity.h"

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
	std::vector<Entity*>entities;
	// The units that the player is controlling.
	std::vector<Entity*> selectedEntities;
	// How much $$$$ the player has, used for unit spawning.
	int balance = 10000;

	// Used to limit key calls.
	int updateCalled;

public:
	

	Team GetTeam() { return team; }
	void SetTeam(Team t) { team = t; }
	std::vector<Entity*>& GetEntities() { return entities; }

	void Update(std::vector<Entity*>& enemyList);
	void HandleInput(std::vector<Entity*>& enemyList);
	void Render();

};

