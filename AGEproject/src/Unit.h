#pragma once
#include "Entity.h"
#include "Movement.h"
class Unit : public Component
{
enum Action {Move,Attack,AttackMove,Hold};

protected:
	// What action this unit is to perform.
	Action action = Move;
	// What entity is it looking to attack?
	Entity* targetEntity = NULL;
	// Type of movement the unit has.
	Movement* movement;

	void from_json(const nlohmann::json &j) {};
public:
	Unit() : Component("Unit") {};
	Unit(std::string type) : Component(type) {};
	~Unit() {};


	void SetAction(Action act)
	{
		action = act;
	}

	void SetEntityToTarget(Entity* target)
	{
		targetEntity = target;
	}

	void SetMovement(Movement* move)
	{
		move->SetParent(GetParent());
		movement = move;
		movement->SetParent(GetParent());
	}
	Movement& GetMovement() { return *movement; }
	virtual void AttackEntity() {}
	
	void Update(double deltaTime) override
	{
		if (movement->GetParent() == NULL)
			movement->SetParent(GetParent());
		// If hold, do nothing.
		if (action == Hold)
			return;
		// If move, keep moving the unit to destination.
		if (action == Move)
		{
			// Get the equilivent movement component and call it.
			movement->Update(deltaTime);
		}
		if (action == Attack)
		{
			AttackEntity();
		}	
	}

};