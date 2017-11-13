// This is a resource componenent that the worker unit can harvest from.

#pragma once
#include "Entity.h"
#include <mutex>
class Resource : public Component
{
private:
	// How much value the resource value contains, should it increment over times?
	int value = 250;

	// Required if resource is finite.
	std::mutex mut;
public:

	Resource() : Component("Resource") {};
	~Resource() {}

	void SetValue(int v) { value = v; }
	int GetValue() { return value; }
	int AddToValue(int v) { value += v; }

	// This resource takes x amount from the resource. Is there parameters that need to be considered?
	int RetrieveResource()
	{
		int newValue = value;
		mut.lock();
		std::cout << "Retrieving resource." << std::endl;
		std::cout << "Pretending to reduce resource." << std::endl;
		mut.unlock();
		return newValue;
	}


	void from_json(const nlohmann::json &j) {};
};


