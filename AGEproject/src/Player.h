// This is the base class for player interaction with the game.
#pragma once
#include <vector>
#include "Entity.h"

enum Team {TeamOne, TeamTwo, TeamThree};

class Player
{

private:

public:
	// Team the player is on.
	Team team;
	// is this player an npc.
	bool isNPC = false;
	// List of entities that this player contains.
	std::vector<Entity*>entities;
	
	// This will be converted to a vector later.
	std::vector<Entity*> selectedEntities;
	
	

	void Update();
	void HandleInput();
	void Render();









};

