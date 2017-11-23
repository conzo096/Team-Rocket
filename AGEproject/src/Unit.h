#pragma once
#include "Entity.h"
#include "Renderable.h"
#include "Movement.h"
#include "Targetable.h"
#include "GeometryUtil.h"
#include "Particle.h"
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
	// Is the unit currently controller by the player?
	bool isControlled = false;
	// Previous effect.
	glm::vec4 tempCol;

	double weaponRange;
	double sightRange;

	double fireRate = 0.5;
	double timeSinceLastFire;

	bool targetAcquired = false;
	bool canShoot = true;

	// Bullet container.
	std::vector<BulletParticle> projectiles;

	void from_json(const nlohmann::json &j) {};
public:

	Unit() : Component("Unit") {};
	Unit(std::string type) : Component(type) {};
	~Unit() {};


	void SetAction(Action act)
	{
		action = act;
	}

	// Change value for being controlled by player or not.
	void IsController(bool act);

	void SetEntityToTarget(Entity*& target)
	{
		targetEntity = target;
	}

	void SetTeam(int t) { team = t; }
	int GetTeam() { return team; }
	
	virtual void AttackEntity();
	
	void Update(double deltaTime) override;

	void Render();
};