#include "Renderable.h"
#include "GeometryUtil.h"
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
	model = GeometryUtil::BuildPlane(spacing,xSize,ySize);
	renderInfo.modelVao = model->GetVAO();
	renderInfo.drawType = model->GetType();
	renderInfo.indices = model->GetIndices();
}

void Renderable::SetModel(std::string location)
{
	model = new Model(location);
	renderInfo.modelVao = model->GetVAO();
	renderInfo.drawType = model->GetType();
	renderInfo.indices = model->GetIndices();
}

void Renderable::SetTexture(std::string texName)
{
	//effect->texture = Shader::Get().AddTexture(texName);
	effect->texture = ResourceHandler::Get().GetTexture(texName);
	renderInfo.texture = effect->texture;
}

void Renderable::SetShader(std::string texName)
{
	effect->shader = texName;
	renderInfo.shader = ResourceHandler::Get().GetShader(texName)->GetId();

}

void Renderable::SetMaterial(Material Mat)
{
	std::cout << "Todo : material " << std::endl;
}


void Renderable::Render()
{
	//GameEngine::Get().Render(GetTransform(),*model, *effect);
	renderInfo.m = GetParent()->GetTransform();
	GameEngine::Get().AddToRenderList(renderInfo);

}
