#pragma once
#include "Unit.h"
class Worker : public Unit
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Worker() : Unit("Worker")
	{

	};
	~Worker() {};

	//void Update(double delta);

};