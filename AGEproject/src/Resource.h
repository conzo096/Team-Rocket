// This is a resource componenent that the worker unit can harvest from.

#pragma once
#include <glm\common.hpp>
#include <math.h>
#include "Entity.h"
#include "RayCast.h"
#include <glm\gtx\string_cast.hpp>
class Resource : public Component
{
private:
	// How much value the resource value contains, should it increment over times?
	int value = 250;



public:

	Resource() : Component("Resource") {};
	~Resource() {}

	void SetValue(int v) { value = v; }
	int GetValue() { return value; }
	int AddToValue(int v) { value += v; }

	// This resource takes x amount from the resource. Is there parameters that need to be considered?
	int RetrieveResource()
	{
		std::cout << "No Moneys" << std::endl;
		return value;
	}


	void from_json(const nlohmann::json &j) {};
};


