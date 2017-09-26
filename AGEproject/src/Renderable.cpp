#include "Renderable.h"

void Renderable::from_json(const nlohmann::json & j)
{
}

Renderable::Renderable() : model(nullptr), Component("Rendererable")
{
}

Renderable::~Renderable()
{
}

void Renderable::SetModel()
{

}

void Renderable::Render()
{
}
