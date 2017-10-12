#include "BoundingSphere.h"
#include "Game.h"
void BoundingSphere::Update(double deltaTime)
{
	TestIntersection(RayCast(RayCast(Game::Instance()->cam->GetComponent<Free_Camera>())));
	//std::cout << "Update" << std::endl;
}