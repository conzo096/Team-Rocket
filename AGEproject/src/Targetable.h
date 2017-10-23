#pragma once
#include "Entity.h"

class Targetable : public Component
{
private:
	bool dead;
	float maxHealth;
	float health;
	float thresholdArmour;
	float resistanceArmour;

protected:
	void from_json(const nlohmann::json &j);
public:
	Targetable();
	~Targetable();

	void SetMaxHealth(float newMax) { maxHealth = newMax; };
	void Heal(float ammount) { health += ammount; };
	void SetThreshold(float newThreshold) { thresholdArmour = newThreshold; };
	void SetResistance(float newResistance) { resistanceArmour = newResistance; };

	void TakeDamage(float damage);
	bool IsDead() { return dead; };
	void Update(double delta) override;
};