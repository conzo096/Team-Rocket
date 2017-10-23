#pragma once
#include "Entity.h"
#include "Movement.h"
#include "Targetable.h"
class Unit : public Component
{

public:
	enum Action { Move, Attack, AttackMove, Hold };
protected:
	// What action this unit is to perform.
	Action action = Move;
	// What entity is it looking to attack?
	Entity* targetEntity = NULL;
	// What team is this unit on?
	int team;


	void from_json(const nlohmann::json &j) {};
public:

	Unit() : Component("Unit") {};
	Unit(std::string type) : Component(type) {};
	~Unit() {};


	void SetAction(Action act)
	{
		action = act;
	}

	void SetEntityToTarget(Entity*& target)
	{
		targetEntity = target;
	}

	void SetTeam(int t) { team = t; }
	int GetTeam() { return team; }
	
	virtual void AttackEntity()
	{
		// if it is within distance.
		if (targetEntity != NULL)
		{
			// Move towards entity.
			GetParent()->GetCompatibleComponent<Movement>()->SetDestination(targetEntity->GetPosition());
	//		if ((targetEntity->GetPosition()- GetPosition()).length() <2)
			{
				// Damage enemy.
				targetEntity->GetCompatibleComponent<Targetable>()->TakeDamage(0.8);
			}
			// Check if enemy is dead and if it is, remove from target.
			if (targetEntity->GetCompatibleComponent<Targetable>()->IsDead())
			{

				targetEntity = NULL;
				// Stop moving.
				GetParent()->GetCompatibleComponent<Movement>()->SetDestination(GetParent()->GetPosition());
				action = Hold;
			}
		}
	}
	
	void Update(double deltaTime) override
	{

		// If hold, do nothing.
		if (action == Hold)
			return;
		// If move, keep moving the unit to destination.
		if (action == Move)
		{

		}
		if (action == Attack)
		{
			AttackEntity();
		}	
	}

};