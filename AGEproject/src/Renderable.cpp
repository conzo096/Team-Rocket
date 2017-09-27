#include "Renderable.h"

void Renderable::from_json(const nlohmann::json & j)
{
}

Renderable::Renderable() : mesh(nullptr), effect(new Effect()), Component("Rendererable")
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
	effect->texture = GameEngine::Instance()->LoadTextures("../res/textures/debug.png");
}

void Renderable::Render()
{
}
