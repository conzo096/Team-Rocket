#include "Structure.h"

void Structure::from_json(const nlohmann::json & j)
{
}

Structure::Structure() : building(true), constructionTime(10.0f), Component("Structure")
{
}

Structure::~Structure()
{
}

void Structure::Build(double delta)
{
	ammountBuilt += (float)delta;
	if (ammountBuilt >= constructionTime)
	{
		building = false;
	}
}

void Structure::Update(double delta)
{
	if (building)
	{
		Build(delta);
		return;
	}
	if (!built)
	{
		Game::Instance()->SpawnUnit(GetParent()->GetPosition(), glm::vec2(7, 7));
		built = true;
	}
}
