#pragma once
#include "Game.h"
#include "Entity.h"

class Structure : public Component
{
private:
	bool building = true;
	bool built;
	float constructionTime;
	float ammountBuilt;
protected:
	void from_json(const nlohmann::json &j);
public:
	Structure();
	~Structure();

	void Build(double delta);
	void Update(double delta) override;

};