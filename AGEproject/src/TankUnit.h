#pragma once
#include "Unit.h"
class Tank : public Unit
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Tank() : Unit("Tank")
	{

	};
	~Tank() {};

	//void Update(double delta);

};