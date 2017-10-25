#pragma once
#include "Structure.h"
class Barracks : public Structure
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Barracks() : Structure("Barracks")
	{

	};
	~Barracks() {};

	//void Update(double delta);

};
