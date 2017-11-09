#pragma once
#include "Entity.h"
#include "Movement.h"
#include "BoundingSphere.h"
#include "Targetable.h"
#include "Quad.h"
class BulletParticle
{
private:
	// All particles should share this.
	float lifeSpan;
	int texture;
	// For bullets only.

	// Target entity. - I wanna shoot this.
	Entity* target = NULL;
	// How much does the bullet do?
	float damage = 20;
	BoundingSphere* sphere = new BoundingSphere;
	Quad* quad;

	glm::vec3 position;

protected:

public:

	bool isActive = true;

	void SetBulletDamage(float d) { damage = d; }
	void SetTarget(Entity* t) { target = t; }
	BulletParticle();
	BulletParticle(glm::vec3 pos);
	~BulletParticle();
	void Update(double delta);
	void Render();
	bool IsActive() { return isActive; }
	int GetTexture() { return texture; }
	glm::vec3 GetPosition() { return position; }
};