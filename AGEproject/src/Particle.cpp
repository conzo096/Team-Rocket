#include "Particle.h"
#include "ResourceHandler.h"
BulletParticle::BulletParticle() 
{
	texture = ResourceHandler::Get().GetTexture("debug");
}

BulletParticle::BulletParticle(glm::vec3 pos) : BulletParticle()
{
	position = pos;
	sphere->SetUpBoundingSphere(1, pos);
}

BulletParticle::~BulletParticle()
{
	//if(sphere != NULL)
	//	delete sphere;
	//if(quad != NULL)
	//delete quad;
}


void BulletParticle::Update(double delta)
{
	if (isActive && target != NULL)
	{
		// Set the target to target destination.
		glm::dvec3 moveVec = target->GetPosition() - glm::dvec3(position);
		double distance = glm::length(moveVec);
		moveVec = glm::normalize(moveVec);
		moveVec = moveVec *delta;
		position += moveVec;
		sphere->SetUpBoundingSphere(0.8, position);
		// Check collisions.
		if (target->GetCompatibleComponent<BoundingSphere>() != NULL)
		{
			if(!sphere->DetectSphereSphereIntersection(target->GetComponent<BoundingSphere>()))
			{
				// Damage target then deactivate this bullet.
				target->GetComponent<Targetable>().TakeDamage(damage);
				isActive = false;
			}
		}
	}



}

void BulletParticle::Render()
{
	if (isActive)
	{
		ParticleData particle;
		particle.pos = position;
		particle.tex = texture;
		GameEngine::Get().AddToParticleList(particle);
	}

}