#pragma once
#include "Renderable.h"


class TurretRenderable : public Renderable
{
public:
	void Update(double deltaTime) override
	{
		std::cout << "Ruairi do this maths!" << std::endl;
	
	}

};