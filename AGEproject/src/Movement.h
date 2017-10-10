#pragma once
#include "Entity.h"

class Movement : public Component
{
private:
	
protected:
	double speed;
	double accelerationTime;
	double currentSpeed;
	glm::dvec3 destination;

	void from_json(const nlohmann::json &j);
public:
	Movement();
	Movement(std::string type);
	~Movement();

	void SetSpeed(double speed) { this->speed = speed; };
	void SetAcceleration(double accelerationTime) { this->accelerationTime = accelerationTime; };
	void SetDestination(glm::dvec3 destination) { this->destination = destination; };
	void Update(double delta) override;
};