#include "Renderable.h"
#include "GeometryUtil.h"
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
	//model = GeometryUtil::BuildPlane(spacing,xSize,ySize);
	model = ResourceHandler::Get().GetModel("Plane");
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
	effect->material = mat;
	renderInfo.mat = mat;
}


void Renderable::Render()
{
	//GameEngine::Get().Render(GetTransform(),*model, *effect);
	renderInfo.m = GetParent()->GetTransform();
	GameEngine::Get().AddToRenderList(renderInfo);

}

void Renderable::SetProperties(const std::string jsonFile)
{
	std::ifstream ifs(jsonFile);
	json j = json::parse(ifs);

	if(j["Model"] != "") // Models, Shaders and Textures can be empty strings but must exist in the file
		this->SetModel(j["Model"]);
	if(j["Shader"] != "")
		this->SetShader(j["Shader"]);
	if(j["Texture"] != "")
		this->SetTexture(j["Texture"]);

	if(j["Plane"] != nullptr) // If json file contains key "Plane" then...
	{
		json pl = j["Plane"];
		std::vector<float> pls;
		for(const auto elem : pl)
		{
			pls.push_back(elem);
		}
		this->SetPlane(pls[0], pls[1], pls[2]);
	}

	json pos = j["Position"]; // All things have a position so no error checking
	std::vector<float> p;
	for (const auto elem : pos)
	{
		p.push_back(elem);
	}

	this->SetPosition(glm::vec3(p[0], p[1], p[2]));
}

