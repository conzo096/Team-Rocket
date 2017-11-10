// This is the base class for player interaction with the game.
#pragma once
#include <vector>
#include "Entity.h"


class Player
{

protected:
	// Team the player is on.
	int team;
	// List of entities that this player contains.
	std::vector<Entity*>entities;
	// The units that the player is controlling.
	std::vector<Entity*> selectedEntities;
	// How much $$$$ the player has, used for unit spawning.
	int balance = 10000;
public:
	

	int GetTeam() { return team; }
	void SetTeam(int t) { team = t; }
	std::vector<Entity*>& GetEntities() { return entities; }

	void Update(std::vector<Entity*>& enemyList);
	void HandleInput(std::vector<Entity*>& enemyList);
	void Render();



};

