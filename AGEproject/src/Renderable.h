#pragma once
#include "Entity.h"

class Renderable : public Component
{
protected:
	//meshes and stuff

public:
	Renderable();
	~Renderable();

	void SetMesh();
	void Render();
};