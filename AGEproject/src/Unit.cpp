#include "Unit.h"
#include "Game.h"

void Unit::IsController(bool act)
{
	isControlled = act;
	// If it is being selected.
	if (act)
	{
		// Hold current emissive value.
		originalColour = glm::vec4(GetParent()->GetComponent<Renderable>().GetMaterial().emissive);
		// Set objects emissive value to blue (for now). 
		GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(0, 0, 1, 1);
	}
	else
	{
		// Return the emissive colour back to its original value.
		GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(originalColour);
		originalColour = glm::vec4();
	}
}

void Unit::AcquireTarget()
{
	vector<Entity*> localUnits = Game::Get().FindLocalUnits(team ,GetParent()->GetPosition(), sightRange);
	if (localUnits.size() == 0)
	{
		targetAcquired = false;
		targetEntity = NULL;
		return;
	}
	int min, n = 0;
	for (Entity*& e : localUnits)
	{
		if (e->GetCompatibleComponent<Targetable>() != NULL)
		{
			if (distance(GetParent()->GetPosition(), e->GetPosition()) < distance(GetParent()->GetPosition(), localUnits[min]->GetPosition()))
			{
				min = n;
			}
		}
		n++;
	}
	targetAcquired = true;
	targetEntity = localUnits[min];
}

void Unit::AttackEntity()
{
	if (timeSinceLastFire > fireRate)
		canShoot = true;
	// if it is within distance.
	if (targetAcquired)
	{
		// If within range, fire a projectile
		if (glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) < weaponRange && canShoot)
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
			targetAcquired = false;
			// Stop moving.
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
			action = Hold;
		}



	}
}

void Unit::Update(double deltaTime)
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
		AcquireTarget();
		// Move towards entity.
		if (targetEntity != NULL)
		{
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(targetEntity->GetPosition().x, GetParent()->GetPosition().y, targetEntity->GetPosition().z));
		}
		AttackEntity();
	}
	if (action == AttackMove)
	{
		AttackEntity();
	}

	// Update all the bullets.
	for (BulletParticle & b : projectiles)
		b.Update(deltaTime);
	// Remove bullets no longer used.
	projectiles.erase(std::remove_if
	(projectiles.begin(), projectiles.end(), [](const BulletParticle& x)
	{
		return !x.isActive;
	}), projectiles.end());
}

void Unit::Render()
{
	for (BulletParticle & b : projectiles)
		b.Render();
}
