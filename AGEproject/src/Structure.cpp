#include "Structure.h"
#include "Spawner.h"
void Structure::from_json(const nlohmann::json & j)
{
}

Structure::Structure() : building(true), constructionTime(0.0f), Component("Structure")
{
	//for(int i = 0; i < 100; i++)
	//	AddProduct("TEMP", 1.0f);
}

Structure::~Structure()
{
}

void Structure::Build(double delta)
{
	ammountBuilt += (float)delta;
	if (ammountBuilt >= constructionTime)
	{
		building = false;
		ammountBuilt = 0.0f;
	}
}

void Structure::AddProduct(std::string productName, float buildTime)
{

	Product tempProduct;
	tempProduct.productName = productName;
	tempProduct.buildTime = buildTime;
	std::cout << "Before: " << productQueue.size() << std::endl;
	productQueue.push(tempProduct);
	std::cout << "After: " << productQueue.size() << std::endl;
}

void Structure::Produce(double delta)
{	
	ammountBuilt += delta;
	if (ammountBuilt >= productQueue.front().buildTime)
	{
		// Spawn unit should be from factory pattern class, not game!
		collectionQueue.push_back(Spawner::Get().CreateEntity(productQueue.front().productName,GetParent()->GetPosition(),team));
		ammountBuilt = 0.0f;
		productQueue.pop();
	}
}

void Structure::Update(double delta)
{
	if (building)
	{
		Build(delta);
		return;
	}
	if(productQueue.size() != 0)
		Produce(delta);
}

void Structure::Collect(std::vector<Entity*>& ents)
{
	for (Entity*&e:collectionQueue)
		ents.push_back(e);
	collectionQueue.clear();
	
}