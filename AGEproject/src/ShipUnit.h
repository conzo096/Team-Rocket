#pragma once
#include "Unit.h"
#include "AirMovement.h"
class Ship : public Unit
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Ship() : Unit("Ship")
	{
	//	movement = new AirMovement;
	//	movement->SetParent(this->GetParent());
	};
	~Ship() {};

	//void Update(double delta) override;

};