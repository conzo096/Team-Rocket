#pragma once
#include "Structure.h"
class Barracks : public Structure
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Barracks() : Structure("Barracks","Troop",100)
	{

	};
	~Barracks() {};

	//void Update(double delta);

};
