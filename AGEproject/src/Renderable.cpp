#include "Renderable.h"

void Renderable::from_json(const nlohmann::json & j) {}

Renderable::Renderable() : model(nullptr), effect(new Effect()), Component("Renderable") {}

Renderable::~Renderable() {}

void Renderable::SetModel(std::string location) 
{
	model = new Model(location);//FIX THIS
}

void Renderable::SetEffect()
{
	effect->texture = GameEngine::Instance()->LoadTextures("../res/textures/debug.png");
}

void Renderable::Render()
{
	Rotate(glm::vec3(0.05, 0.05, 0));
	UpdateTransforms();
	GameEngine::Instance()->Render(GetTransform(),*model, effect->texture);
}
