#pragma once
#include "Entity.h"
#include <mutex>
class Targetable : public Component
{
private:
	bool dead;
	float maxHealth;
	float health;
	float thresholdArmour;
	float resistanceArmour;

	// Has it been selected by the player?
	bool isSelected = false;
	// the original colour of the renderable object.
	glm::vec4 originalColour;



	// Safe memory, replace with atomic?
	std::mutex mut;

protected:
	void from_json(const nlohmann::json &j);
public:
	Targetable();
	~Targetable();

	void SetMaxHealth(float newMax) { maxHealth = newMax; };
	void SetHealth(float h) { health = h; }
	void Heal(float ammount) { health += ammount; };
	void SetThreshold(float newThreshold) { thresholdArmour = newThreshold; };
	void SetResistance(float newResistance) { resistanceArmour = newResistance; };

	void TakeDamage(float damage);
	bool IsDead() { return dead; };
	void Update(double delta) override;

	void IsSelected(bool act);
	float GetHealth() { return health; }
};