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

	// This will be converted to a vector later.
	std::vector<Entity*> selectedEntities;
public:
	

	int GetTeam() { return team; }
	void SetTeam(int t) { team = t; }
	std::vector<Entity*>& GetEntities() { return entities; }

	void Update(std::vector<Entity*>& enemyList);
	void HandleInput(std::vector<Entity*>& enemyList);
	void Render();









};

