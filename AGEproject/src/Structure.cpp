#include "Structure.h"
#include "Spawner.h"
#include "WorkerUnit.h"
void Structure::from_json(const nlohmann::json & j)
{
}

Structure::Structure() : building(true), constructionTime(0.0f), Component("Structure")
{

}

Structure::~Structure()
{
	// Need to empty game grid that this structure has queued as invalid.
	BoundingSphere bs;
	while (!productQueue.empty())
	{
		auto & p = productQueue.front();
		bs.SetUpBoundingSphere(p.radius, p.destination);
		Spawner::Get().UpdateGameGrid(bs,0);
		productQueue.pop();
	}
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

bool Structure::AddProduct(int& bal, int hotkey)
{
	if (spawnData.size() == 0)
		return false;
	if (hotkey > spawnData.size() - 1)
		return false;
	int newBalance = bal - spawnData[hotkey].cost;
	// If the balance is invalid, do not allow object to be queued.
	if (newBalance < 0)
		return false;
	// Check if area is valid.
	if(spawnData[hotkey].unitType == "Base" || spawnData[hotkey].unitType == "Hanger" || spawnData[hotkey].unitType == "Resource" || spawnData[hotkey].unitType == "Factory"
			 || spawnData[hotkey].unitType == "VehicleBay")
	{
		sp.SetUpBoundingSphere(ResourceHandler::Get().GetModel(spawnData[hotkey].unitType)->GetVertexPositions());
		sp.SetCenter(spawnPoint);
		if (!Spawner::Get().CheckGameGrid(sp))
			return false;
	}
	value += spawnData[hotkey].cost;
	//std::cout << GetTeam() << " balance is now: " << newBalance << std::endl;
	bal = newBalance;
	tempProduct.productName = spawnData[hotkey].unitType;
	tempProduct.buildTime = spawnData[hotkey].buildTime;
	tempProduct.destination = spawnPoint;
	productQueue.push(tempProduct);
	return true;
}

void Structure::Produce(double delta)
{
	ammountBuilt += delta;
	if (ammountBuilt >= productQueue.front().buildTime)
	{
		// Spawn unit should be from factory pattern class, not game!
		collectionQueue.push_back(Spawner::Get().CreateEntity(productQueue.front().productName, productQueue.front().destination, team,rank));
		ammountBuilt = 0.0f;
		productQueue.pop();
	}
}

void Structure::Update(double delta)
{

	// Handle worker logic.
	if (GetParent()->GetName() == "Worker" && productQueue.size() > 0)
	{
		GetParent()->GetCompatibleComponent<Worker>()->SetAction(Unit::Build);
		// Make unit go to destination first.
		if (productQueue.front().destination != GetParent()->GetPosition() && productQueue.front().destination != GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
		{
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(productQueue.front().destination);
		}
			if (glm::distance(GetParent()->GetPosition(), glm::dvec3(productQueue.front().destination)) > 3)
				return;
			else
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
		// If it is in range, allow construction.
	}

	if (building)
	{
		Build(delta);
		return;
	}
	if (productQueue.size() != 0)
		Produce(delta);
}

void Structure::Collect(std::vector<std::shared_ptr<Entity>>& ents)
{
	for (std::shared_ptr<Entity>&e : collectionQueue)
		ents.push_back(e);
	collectionQueue.clear();
}
