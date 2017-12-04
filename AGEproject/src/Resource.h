// This is a resource componenent that the worker unit can harvest from.

#pragma once
#include "Entity.h"
#include <mutex>
class Resource : public Component
{
private:
	// How much value the resource value contains, should it increment over times?
	int value = 2500;

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
		// Unit takes 250 from me.
		int takeAmount = 250;
		value -=250;
		mut.lock();
		std::cout << "Retrieving resource. " << value << std::endl;
		std::cout << "Pretending to reduce resource." << std::endl;
		if (value < 0)
		{
			takeAmount = takeAmount + value;
			value = 0;
			// if it false delete - even better give it a targetable component.
			GetParent()->GetComponent<Targetable>().SetHealth(0);
			GetParent()->GetComponent<Targetable>().Update(0);
		}
		mut.unlock();
		return takeAmount;
	}


	void from_json(const nlohmann::json &j) {};
};


