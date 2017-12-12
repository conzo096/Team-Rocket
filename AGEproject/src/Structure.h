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
		// radius it occupies.
		float radius;

	};

private:
	// Is it currently building a unit?
	bool building;
	float constructionTime;
	double ammountBuilt;
	std::queue<Product> productQueue;
	// Holds items that are created.
	std::vector<std::shared_ptr<Entity>> collectionQueue;
	// Types of entities it can spawn.
	std::vector<SpawnInfo> spawnData;
	// The current stored value of this building (how much money has been invested into producing units/other structures.)
	int value;
	// What team this structure belongs to.
	Team team;
	// Where units created from this will come from.
	glm::vec3 spawnPoint;
	// The rank of the structure.
	int rank = 0;
	int updateCost = 550;

protected:
	void from_json(const nlohmann::json &j);
public:
	Structure();
	Structure(std::string type) : Component(type) {};
	Structure(std::string type,std::string unitType,int cost) : Structure(type) {};
	~Structure();

	void Collect(std::vector<std::shared_ptr<Entity>>& ents);
	void Build(double delta);
	bool AddProduct(int& bal, int hotkey);
	void Produce(double delta);
	void Update(double delta) override;
	int GetQueueSize() { return  (int)productQueue.size(); }
	// Get next thing in queue.
	Product GetNextProduct() { return productQueue.front(); }
	void SetTeam(Team t) { team = t; }
	Team GetTeam() { return team; }
	
	void AddSpawnInfo(SpawnInfo info) { spawnData.push_back(info); }
	std::vector<SpawnInfo> GetSpawnInfo() { return spawnData; }

	glm::vec3 GetSpawnPoint() { return spawnPoint; }
	void SetSpawnPoint(glm::vec3 sp) { spawnPoint = sp; }

	void SetRank(int r) { rank = r; }
	int GetRank() { return rank; }
	void SetUpdateCost(int uc) { updateCost = uc; }
	int GetUpdateCost() { return updateCost; }

	bool BuyRankUpdate(int& bal)
	{
		int newBal = bal;
		// If Rank if 0 ... apply first update
		if (rank == 0)
		{
			// First upgrade costs x amount.
			newBal -= updateCost;
			// It it can afford it.
			if (newBal >= 0)
			{
				updateCost = 750;
				rank++;
				bal = newBal;
				return true;
			}
		}
		// If Rank if 0 ... apply first update
		else if (rank == 1)
		{
			// First upgrade costs x amount.
			newBal -= updateCost;
			// It it can afford it.
			if (newBal >= 0)
			{
				rank++;
				bal = newBal;
				return true;
			}
		}
		else
			return false;
	}
};