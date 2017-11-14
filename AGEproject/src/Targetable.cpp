#include "Targetable.h"

void Targetable::from_json(const nlohmann::json & j)
{
}

Targetable::Targetable() : dead(), maxHealth(), health(maxHealth), thresholdArmour(), resistanceArmour(), Component("Targetable")
{
}

Targetable::~Targetable()
{
}

void Targetable::TakeDamage(float damage)
{
	mut.lock();
	damage *= (1.0f - resistanceArmour);
	damage -= thresholdArmour;
	health -= damage;
	mut.unlock();
}

void Targetable::Update(double delta)
{
	if (health <= 0.0f)
	{
		dead = true;
	}
}
