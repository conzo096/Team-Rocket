#include "Renderable.h"

void Renderable::from_json(const nlohmann::json & j)
{
}

Renderable::Renderable() : model(nullptr), effect(new Effect()), Component("Rendererable")
{
}

Renderable::~Renderable()
{
}

void Renderable::SetModel(std::string location)
{
	model = &Model(location);//FIX THIS
}

void Renderable::SetEffect()
{
	effect->texture = GameEngine::Instance()->LoadTextures("../res/textures/debug.png");
}

void Renderable::Render()
{
	unsigned int a = effect->texture;
	GameEngine::Instance()->Render(*model, a);
}
