#pragma once
#include "Entity.h"

class Structure : Component
{
private:
	bool building;
public:
	Structure();
	~Structure();

	void Build();
	void Update(double delta) override;

};