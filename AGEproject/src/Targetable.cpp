#include "Targetable.h"
#include "Spawner.h"
#include "Renderable.h"
void Targetable::from_json(const nlohmann::json & j)
{
}

Targetable::Targetable() : dead(), maxHealth(), health(maxHealth), thresholdArmour(), resistanceArmour(), Component("Targetable")
{
}


void Targetable::IsSelected(bool act)
{
	isSelected = act;
	// If it is being selected.
	if (act)
	{
		// Set objects emissive value to blue (for now). 
		GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(0, 0, 1, 1);
	}
	else
	{
		// Return the emissive colour back to its original value.
		GetParent()->GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(originalColour);
	}
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
	if(!isSelected)
		// Hold current emissive value.
		originalColour = glm::vec4(GetParent()->GetComponent<Renderable>().GetMaterial().emissive);
	if (health <= 0.0f)
	{
		dead = true;
	}

	// If it is dead allow units to walk though its occupied area - Can this be put somewhere more sensible?
	if (dead)
	{
		Spawner::Get().UpdateGameGrid(GetParent()->GetComponent<BoundingSphere>(), 0);
	}
}
