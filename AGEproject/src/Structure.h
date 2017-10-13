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
	bool building;
	float constructionTime;
	float ammountBuilt;
	std::queue<Product> productQueue;

protected:
	void from_json(const nlohmann::json &j);
public:
	Structure();
	~Structure();

	void Build(double delta);
	void AddProduct(std::string productName, float buildTime);
	void Produce(double delta);
	void Update(double delta) override;

};