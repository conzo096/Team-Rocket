#pragma once
#include "Unit.h"
class Troop : public Unit
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Troop() : Unit("Troop")
	{

	};
	~Troop() {};

	//void Update(double delta);

};