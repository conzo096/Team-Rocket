#pragma once
#pragma once
#include "Structure.h"
class Shipyard : public Structure
{
private:

protected:
	void from_json(const nlohmann::json &j) {};
public:
	Shipyard() : Structure("Shipyard")
	{

	};
	~Shipyard() {};

	//void Update(double delta);

	void AddProduct(int val, int deposit)
	{
		worth += deposit;
		Product tempProduct;
		if (val == 0)
		{
			tempProduct.productName = "Ship";
			tempProduct.buildTime = 5;
		}
		if (val == 1)
		{
			tempProduct.productName = "Ship";
			tempProduct.buildTime = 5;

		}
		if (val == 2)
		{
			tempProduct.productName = "Ship";
			tempProduct.buildTime = 5;

		}
		productQueue.push(tempProduct);
	
	}
};