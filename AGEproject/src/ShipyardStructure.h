#pragma once
#pragma once
#include "Structure.h"
class Shipyard : public Structure
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Shipyard() : Structure("Shipyard")
	{

	};
	~Shipyard() {};

	//void Update(double delta);

};