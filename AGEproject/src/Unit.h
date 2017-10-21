#pragma once
#include "Entity.h"
class Unit : public Component
{
	enum Action {Move,Attack,AttackMove,Hold};
private:

	// What action this unit is to perform.
	Action action;

	// What entity is it looking to attack?
	Entity* targetEntity = NULL;

protected:
	void from_json(const nlohmann::json &j);
public:
	Unit();
	~Unit();


	void SetAction(Action act)
	{
		action = act;
	}

	void SetEntityToTarget(Entity* target)
	{
		targetEntity = target;
	}

	virtual void AttackEntity() {}

	void MoveToDestination()
	{
	
	}
	void AttackMove()
	{
	
	}
	void Update(double deltaTime) override
	{
		// If hold, do nothing.
		if (action == Hold)
			return;
		// If move, keep moving the unit to destination.
		if (action == Move)
		{
			// Get the equilivent movement component and call it.
		}
		if (action == Attack)
		{
			AttackEntity();
		}

		// 
	
	
	}

};