#include "Renderable.h"
#include "GeometryUtil.h"
#include "BoundingSphere.h"
void Renderable::from_json(const nlohmann::json & j)
{
}

Renderable::Renderable() : model(nullptr),effect(new Effect()), Component("Rendererable")
{
}

Renderable::~Renderable()
{
}

void Renderable::SetPlane(float spacing, unsigned int xSize, unsigned int ySize)
{
	model = GeometryUtil::BuildPlane(spacing,xSize,ySize);
	//model = ResourceHandler::Get().GetModel("Plane");
	renderInfo.modelVao = model->GetVAO();
	renderInfo.drawType = model->GetType();
	renderInfo.indices = model->GetIndices();
}

void Renderable::SetModel(std::string location)
{
	model =  ResourceHandler::Get().GetModel((location));
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

void Renderable::SetMaterial(Material* mat)
{
	//std::cout << "Todo : material " << std::endl;
	// Free current material.
	if (effect->material != NULL)
		free(effect->material);
	effect->material = mat;
	renderInfo.mat = mat;
}


void Renderable::Render()
{
	//GameEngine::Get().Render(GetTransform(),*model, *effect);
	renderInfo.m = GetTransform();

	// Move later.
	if (GetParent()->GetCompatibleComponent<BoundingSphere>() != NULL)
	{
		renderInfo.sphereRadius = GetParent()->GetComponent<BoundingSphere>().GetRadius();
		renderInfo.boundingPoint = GetParent()->GetComponent<BoundingSphere>().GetCenter();
	}


	GameEngine::Get().AddToRenderList(renderInfo);

}
