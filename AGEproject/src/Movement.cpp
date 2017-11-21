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

void Movement::SetProperties(std::string jsonFile)
{
	std::ifstream ifs(jsonFile);
	json j = json::parse(ifs);

	if(j["Goal"] != nullptr)
	{
		json goal = j["Goal"];
		std::vector<float> g;
		for (const auto elem : goal)
		{
			g.push_back(elem);
		}
		this->SetGoal(glm::dvec3(g[0], g[1], g[2]));
	}

	if(j["Destination"] != nullptr)
	{
		json destination = j["Destination"];
		std::vector<float> d;
		for(const auto elem : destination)
		{
			d.push_back(elem);
		}
		this->SetDestination(glm::dvec3(d[0], d[1], d[2]));
	}

	if (j["Speed"] != "")
		this->SetSpeed(j["Speed"]);

	if (j["Acceleration"] != "")
		this->SetAcceleration(j["Acceleration"]);

	if (j["TurnSpeed"] != "")
		this->SetTurnSpeed(j["TurnSpeed"]);
}

void Movement::Update(double delta)
{

}
