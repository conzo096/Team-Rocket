#include "BoundingSphere.h"
#include "Game.h"
void BoundingSphere::Update(double deltaTime)
{
	TestIntersection(RayCast(RayCast(Game::Get().free_cam->GetComponent<Free_Camera>())),poi);
	//std::cout << "Update" << std::endl;
}