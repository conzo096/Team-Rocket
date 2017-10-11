#pragma once
#include "Movement.h"

class AirMovement : public Movement
{
private:

protected:
	void from_json(const nlohmann::json &j);
public:
	AirMovement();
	~AirMovement();

	void MoveTo(double delta);
	void TurnTo(double delta);
	void Update(double delta) override;

};