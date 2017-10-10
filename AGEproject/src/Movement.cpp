#include "Movement.h"

void Movement::from_json(const nlohmann::json & j)
{
}

Movement::Movement() : speed(), accelerationTime(), currentSpeed(), destination(), Component("Movement")
{
}

Movement::Movement(std::string type) : speed(), accelerationTime(), currentSpeed(), destination(), Component(type)
{
}

Movement::~Movement()
{
}

void Movement::Update(double delta)
{

}
