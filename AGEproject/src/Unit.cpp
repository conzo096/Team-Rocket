#include "Unit.h"
#include "Game.h"

void Unit::SetAction(Action act)
{
	action = act;
}

void Unit::SetUnitWeapon(double weaponRange, double sightRange, double weaponDamage, double fireRate)
{
	this->weaponRange = weaponRange;
	this->sightRange = sightRange;
	this->weaponDamage = weaponDamage;
	this->fireRate = fireRate;
}


void Unit::SetEntityToTarget(std::shared_ptr<Entity>& target)
{
	targetEntity = target;
	targetAcquired = true;
}

void Unit::AcquireTarget()
{
	vector<std::shared_ptr<Entity>> localUnits = Game::Get().FindLocalUnits(team, GetParent()->GetPosition(), sightRange);
	if (localUnits.size() == 0)
	{
		targetAcquired = false;
		targetEntity = NULL;
		return;
	}
	int min = 0;
	int n = 0;
	for (std::shared_ptr<Entity>& e : localUnits)
	{
		if (e->GetCompatibleComponent<Targetable>() != NULL)
		{
			dvec3 pos = dvec3(GetParent()->GetPosition().x, 0, GetParent()->GetPosition().z);
			dvec3 ePos = dvec3(e->GetPosition().x, 0, e->GetPosition().z);
			dvec3 minPos = dvec3(localUnits[min]->GetPosition().x, 0, localUnits[min]->GetPosition().z);
			if (distance(pos, ePos) < distance(pos, minPos))
			{
				min = n;
			}
		}
		n++;
	}
	targetAcquired = true;
	targetEntity = localUnits[min];
}

void Unit::OrderAttackMove(glm::dvec3 target)
{
	action = AttackMove;
	attMoveTarget = target;
	GetParent()->GetCompatibleComponent<Movement>()->SetGoal(attMoveTarget);
}

void Unit::AttackEntity()
{
	if (timeSinceLastFire > fireRate)
		canShoot = true;
	// if it is within distance.
	if (targetAcquired)
	{
		dvec3 pos = dvec3(GetParent()->GetPosition().x, 0, GetParent()->GetPosition().z);
		dvec3 ePos = dvec3(targetEntity->GetPosition().x, 0, targetEntity->GetPosition().z);
		if (GetParent()->GetCompatibleComponent<TurretRenderable>() != NULL)
		{
			GetParent()->GetCompatibleComponent<TurretRenderable>()->SetTarget(targetEntity);
		}
		// If within range, fire a projectile
		if (glm::distance(pos, ePos) < weaponRange)
		{
			GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
		}
		else
		{
			if (action != Hold)
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(targetEntity->GetPosition().x, GetParent()->GetPosition().y, targetEntity->GetPosition().z));
		}

		if (glm::distance(pos, ePos) <= weaponRange + 10)
		{
			if (canShoot)
			{
				timeSinceLastFire = 0;
				canShoot = false;
				// Find an empty bullet and fire.
				BulletParticle bullet(GetParent()->GetPosition());
				bullet.SetBulletDamage(weaponDamage);
				bullet.SetTarget(targetEntity);
				projectiles.push_back(bullet);
			}
		}

		// Check if enemy is dead and if it is, remove from target.
		if (targetEntity->GetCompatibleComponent<Targetable>()->IsDead() || glm::distance(pos, ePos) > sightRange)
		{
			targetEntity = NULL;
			targetAcquired = false;
			// Stop moving.
			if (action == Attack)
			{
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
 				action = Stop;
			}
			else
				if (action == AttackMove)
				{
					GetParent()->GetCompatibleComponent<Movement>()->SetGoal(attMoveTarget);
				}
			if (GetParent()->GetCompatibleComponent<TurretRenderable>() != NULL)
			{
				GetParent()->GetCompatibleComponent<TurretRenderable>()->ClearTarget();
			}
		}
	}
}

void Unit::Update(double deltaTime)
{
	timeSinceLastFire += deltaTime;

	if (action == Move)
	{
		if (GetParent()->GetPosition() == GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
			action = Stop;
	}

	if (action == Stop)
	{
		AcquireTarget();
		if (targetEntity != NULL)
		{
			action = Attack;
		}
	}

	if (action == Attack)
	{
		if (targetEntity != NULL)
		{
			AttackEntity();
		}
	}

	if (action == AttackMove)
	{
		AcquireTarget();
		if (targetEntity != NULL)
		{
			AttackEntity();
		}
	}
	if (action == Hold)
	{
		GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
		AcquireTarget();
		if (targetEntity != NULL)
		{
			AttackEntity();
		}
	}
	if (action == Harvest)
	{
		HarvestResource();
	}
	if (action == Build)
	{
		targetAcquired = false;
		targetEntity = NULL;
		if (GetParent()->GetCompatibleComponent<Structure>() != NULL)
			if (GetParent()->GetCompatibleComponent<Structure>()->GetQueueSize() < 1 && GetParent()->GetPosition() == GetParent()->GetCompatibleComponent<Movement>()->GetGoal())
			{
				action = Stop;
			}
		BuildStructure();
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
