#pragma once
#include "Entity.h"
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
	double fireRate = 0.5;
	double timeSinceLastFire;

	// Disable shooting.
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
	void IsController(bool act)
	{	
		isControlled = act;
		// If it is being selected.
		if (act)
		{
			// Hold current emissive value.
			tempCol = glm::vec4(GetParent()->GetComponent<Renderable>().GetMaterial().emissive);
			// Set objects emissive value to blue (for now). 
			GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(0, 0, 1, 1);
		}
		else
		{
			// Return the emissive colour back to its original value.
			GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(tempCol);
			tempCol = glm::vec4();
		}
	}

	void SetEntityToTarget(Entity*& target)
	{
		targetEntity = target;
	}

	void SetTeam(int t) { team = t; }
	int GetTeam() { return team; }
	
	virtual void AttackEntity()
	{
		if (timeSinceLastFire > fireRate)
			canShoot = true;
		// if it is within distance.
		if (targetEntity != NULL)
		{
			// Move towards entity.
			GetParent()->GetCompatibleComponent<Movement>()->SetDestination(glm::vec3(targetEntity->GetPosition().x,GetParent()->GetPosition().y, targetEntity->GetPosition().z));
			// If within range, fire a projectile. - This value should not be hard coded.
			if (glm::distance(GetParent()->GetPosition(),targetEntity->GetPosition()) < 200 && canShoot)
			{
				timeSinceLastFire = 0;
				canShoot = false;
				// Find an empty bullet and fire.
				BulletParticle bullet(GetParent()->GetPosition());
				bullet.SetTarget(targetEntity);
				projectiles.push_back(bullet);
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
		timeSinceLastFire += deltaTime;
		// If hold, do nothing.
		if (action == Hold)
		{
		}
		// If move, keep moving the unit to destination.
		if (action == Move)
		{

		}
		if (action == Attack)
		{
			AttackEntity();
		}

		// Update all the bullets.
		for (BulletParticle & b : projectiles)
			b.Update(deltaTime);
		// Remove bullets no longer used.
		projectiles.erase(std::remove_if
		(projectiles.begin(), projectiles.end(),[](const BulletParticle& x)
			{
				return !x.isActive;
			}), projectiles.end());


	}

	void Render()
	{
		for (BulletParticle & b : projectiles)
			b.Render();
	}

};