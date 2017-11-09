#pragma once
#include "Game.h"
#include "Entity.h"
#include <queue>

class Structure : public Component
{
	struct Product
	{
		std::string productName;
		float buildTime;
	};

private:

	// Is the unit currently controller by the player?
	bool isControlled = false;
	// Previous effect.
	glm::vec4 tempCol;

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
	void Produce(double delta);
	void Update(double delta) override;
	int GetQueueSize() { return  (int)productQueue.size(); }

	void SetTeam(int t) { team = t; }
	int GetTeam() { return team; }
	

	// Change value for being controlled by player or not.
	void IsController(bool act)
	{
		isControlled = act;
		// If it is being selected.
		if (act)
		{
			// Hold current emissive value.
			tempCol = glm::vec4(GetParent()->GetComponent<Renderable>().GetMaterial().emissive);
			// Set objects emissive value to blue (for now). 
			GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(0, 0, 1, 1);
		}
		else
		{
			// Return the emissive colour back to its original value.
			GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(tempCol);
			tempCol = glm::vec4();
		}
	}


};