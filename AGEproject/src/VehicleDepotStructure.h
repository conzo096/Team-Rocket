#pragma once
#include "Structure.h"
class VehicleDepot : public Structure
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	VehicleDepot() : Structure("VehicleDepot")
	{

	};
	~VehicleDepot() {};

	//void Update(double delta);

};
