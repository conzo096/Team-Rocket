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
	model = new Model(location);//FIX THIS
}

void Renderable::SetEffect()
{
	effect->texture = GameEngine::Instance()->LoadTextures("../res/textures/ConstructorUV.png");
}

void Renderable::Render()
{
	GameEngine::Instance()->Render(GetTransform(),*model, *effect);
}
