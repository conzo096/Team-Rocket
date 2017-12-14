#pragma once
#include "Player.h"
#include "BoundingSphere.h"
#include "Game.h"
class AiPlayer : public Player
{
	int workerCount;

	int factoryCount;
	int vehicleBayCount;
	int hangerCount;
	
	int droneCount;
	int wardenCount;
	int kestrelCount;

	int buildMin = 80;
	int buildMax = 99;

	bool attacking = false;

	vector<std::shared_ptr<Entity>> resources;

public:
	void Update(std::vector<std::shared_ptr<Entity>>& enemyList);

	//Update unit, building and supply counts
	void CheckProperty();

	//Manage the economy
	void MacroCycle();

	//Manage all units
	void ArmyCycle(std::vector<std::shared_ptr<Entity>>& enemyList);
};