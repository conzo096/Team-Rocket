#include "Structure.h"

Structure::Structure() : Component("Structure")
{
}

Structure::~Structure()
{
}

void Structure::Build()
{
}

void Structure::Update(double delta)
{
	if (building)
	{
		Build;
		return;
	}
}
