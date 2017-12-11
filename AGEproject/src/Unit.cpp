#include "Unit.h"
#include "Game.h"

void Unit::SetAction(Action act)
{
	action = act;
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
		if (GetParent()->GetCompatibleComponent<TurretRenderable>() != NULL)
		{
			GetParent()->GetCompatibleComponent<TurretRenderable>()->SetTarget(targetEntity);
		}
		// If within range, fire a projectile
		if (glm::distance(GetParent()->GetPosition(), targetEntity->GetPosition()) <= weaponRange)
		{
			if (canShoot)//GET WEAPON TO DO THIS
			{
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
				timeSinceLastFire = 0;
				canShoot = false;
				// Find an empty bullet and fire.
				BulletParticle bullet(GetParent()->GetPosition());
				bullet.SetTarget(targetEntity);
				projectiles.push_back(bullet);
			}
		}
		else
		{
			if (action != Hold)
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(targetEntity->GetPosition().x, GetParent()->GetPosition().y, targetEntity->GetPosition().z));
		}



		// Check if enemy is dead and if it is, remove from target.
		if (targetEntity->GetCompatibleComponent<Targetable>()->IsDead())
		{
			targetEntity = NULL;
			targetAcquired = false;
			// Stop moving.
			if (action = Attack)
			{
				GetParent()->GetCompatibleComponent<Movement>()->SetGoal(GetParent()->GetPosition());
				action = Stop;
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
			if (GetParent()->GetCompatibleComponent<Structure>() != NULL)
				if (GetParent()->GetCompatibleComponent<Structure>()->GetQueueSize() < 1)
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
