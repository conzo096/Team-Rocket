#pragma once
#include "Game.h"
#include "Entity.h"
#include <queue>
#include "Player.h"
struct SpawnInfo
{
	// The type of unit that this unit spawns.
	std::string unitType;
	// How long it takes to build said unit.
	float buildTime;
	// How much it costs to build this unit.
	int cost;
};

class Structure : public Component
{
	struct Product
	{
		std::string productName;
		float buildTime;
		glm::vec3 destination;
	};

private:

	// Is the unit currently controller by the player?
	bool isControlled = false;
	// Previous effect.
	glm::vec4 tempCol;

	// Is it currently building a unit?
	bool building;
	float constructionTime;
	float ammountBuilt;
	std::queue<Product> productQueue;
	// Holds items that are created.
	std::vector<Entity*> collectionQueue;
	
	// Types of entities it can spawn.
	std::vector<SpawnInfo> spawnData;
	// The current stored value of this building (how much money has been invested into producing units/other structures.)
	int value;
	// What team this structure belongs to.
	Team team;
protected:
	void from_json(const nlohmann::json &j);
public:
	Structure();
	Structure(std::string type) : Component(type) {};
	Structure(std::string type,std::string unitType,int cost) : Structure(type) {};
	~Structure();

	void Collect(std::vector<Entity*>& ents);
	void Build(double delta);
	void AddProduct(int& bal, int hotkey, glm::vec3 destination);
	void Produce(double delta);
	void Update(double delta) override;
	int GetQueueSize() { return  (int)productQueue.size(); }

	void SetTeam(Team t) { team = t; }
	Team GetTeam() { return team; }
	
	void AddSpawnInfo(SpawnInfo info) { spawnData.push_back(info); }
	std::vector<SpawnInfo> GetSpawnInfo() { return spawnData; }
	// Change value for being controlled by player or not.
	void IsController(bool act);


};