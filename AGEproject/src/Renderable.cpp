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

void Renderable::SetPlane(float spacing, unsigned int xSize, unsigned int ySize)
{
	model = new Model();
	model->CreatePlane(spacing, xSize, ySize);
	model->SetStrip(true);
}

void Renderable::SetModel(std::string location)
{
	model = new Model(location);
	model->SetStrip(false);
}

void Renderable::SetEffect(std::string texName)
{
	effect->texture = Shader::Get().AddTexture(texName);
}

void Renderable::Render()
{
	GameEngine::Get().Render(GetTransform(),*model, *effect);
}
