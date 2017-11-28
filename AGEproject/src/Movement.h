#pragma once
#include "Entity.h"
#include "ResourceHandler.h"

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
	bool needPath;

	void from_json(const nlohmann::json &j);
public:
	Movement();
	Movement(std::string type);
	~Movement();

	glm::dvec3 GetGoal() { return goal; };
	void SetGoal(glm::dvec3 goal) { this->goal = goal; needPath = true; };
	void SetSpeed(double speed) { this->speed = speed; };
	void SetCurrentSpeed(double currentSpeed) { this->currentSpeed = currentSpeed; };
	void SetTurnSpeed(double turnSpeed) { this->turnSpeed = turnSpeed; };
	void SetAcceleration(double accelerationTime) { this->accelerationTime = accelerationTime; };
	void SetDestination(glm::dvec3 destination) { this->destination = destination; };
	void SetProperties(std::string jsonFile);
	glm::dvec3 GetDestination() { return destination; }
	void Update(double delta) override;
};