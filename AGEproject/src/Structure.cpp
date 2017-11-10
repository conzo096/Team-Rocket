#include "Structure.h"
#include "Spawner.h"
void Structure::from_json(const nlohmann::json & j)
{
}

Structure::Structure() : building(true), constructionTime(0.0f), Component("Structure")
{

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

void Structure::AddProduct(int& bal,int hotkey)
{
	if (hotkey > spawnData.size() - 1)
		return;
	int newBalance = bal - spawnData[hotkey].cost;
	// If the balance is invalid, do not allow object to be queued.
	if (newBalance < 0)
		return;
	bal = newBalance;
	Product tempProduct;
	tempProduct.productName = spawnData[hotkey].unitType;
	tempProduct.buildTime = spawnData[hotkey].buildTime;
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