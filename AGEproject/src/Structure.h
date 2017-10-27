#pragma once
#include "Spawner.h"
#include "Entity.h"
#include <queue>

class Spawner;
struct Product
{
	std::string productName;
	float buildTime;
};
class Structure : public Component
{

protected:

	// How much value is stored in the building.
	float worth;

	bool building;
	float constructionTime;
	float ammountBuilt;
	std::queue<Product> productQueue;
	// Holds items that are created.
	std::vector<Entity*> collectionQueue;
	
	// What team this structure belongs to.
	int team;
protected:
	void from_json(const nlohmann::json &j);
public:
	Structure();
	Structure(std::string type) : Component(type) {};
	~Structure();

	void Collect(std::vector<Entity*>& ents);
	void Build(double delta);
	void AddProduct(std::string productName, float buildTime);
	virtual void AddProduct(int value,int deposit);
	void Produce(double delta);
	void Update(double delta) override;

	int GetQueueSize() { return productQueue.size(); }
	void SetTeam(int t) { team = t; }
	int GetTeam() { return team; }
	
};