#include "Renderable.h"

void Renderable::from_json(const nlohmann::json & j)
{
}

Renderable::Renderable() : mesh(nullptr), effect(nullptr), Component("Rendererable")
{
}

Renderable::~Renderable()
{
}

void Renderable::SetMesh(std::string location)
{
	mesh = &Model(location);
}

void Renderable::SetEffect()
{
}

void Renderable::Render()
{
}
