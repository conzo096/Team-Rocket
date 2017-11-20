#pragma once
#include "Entity.h"

class Movement : public Component
{
private:

protected:
	glm::dvec3 goal;
	double speed;
	double accelerationTime;
	double currentSpeed;
	double turnSpeed;
	glm::dvec3 destination;

	void from_json(const nlohmann::json &j);
public:
	Movement();
	Movement(std::string type);
	~Movement();

	void SetGoal(glm::dvec3 goal) { this->goal = goal; };
	void SetSpeed(double speed) { this->speed = speed; };
	void SetTurnSpeed(double turnSpeed) { this->turnSpeed = turnSpeed; };
	void SetAcceleration(double accelerationTime) { this->accelerationTime = accelerationTime; };
	void SetDestination(glm::dvec3 destination) { this->destination = destination; };
	glm::dvec3 GetDestination() { return destination; }
	void Update(double delta) override;
};