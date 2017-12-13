#include "BoundingSphere.h"
#include "Game.h"
void BoundingSphere::Update(double deltaTime)
{
	if (GetParent() != NULL)
	{
		center = GetParent()->GetComponent<Renderable>().GetPosition() + GetParent()->GetPosition();
	}
}