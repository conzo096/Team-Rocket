#pragma once
#include "Structure.h"
class Base : public Structure
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Base() : Structure("Base")
	{

	};
	~Base() {};

	//void Update(double delta);

};