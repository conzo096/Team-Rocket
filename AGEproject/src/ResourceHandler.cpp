#include "ResourceHandler.h"
void ResourceHandler::LoadModels()
{
	std::cout << "Convert to file loading!" << std::endl;
	models.insert(std::pair<std::string, Model*>(std::string("Ship"), new Model("../res/models/Flyer.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Shipyard"), new Model("../res/models/Constructor.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Torus"), new Model("../res/models/Torus2.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Plane"), GeometryUtil::BuildPlane(1, 100, 100)));
	models.insert(std::pair<std::string, Model*>(std::string("BillBoard"), GeometryUtil::BuildPlane(0.5, 4,4)));
}
void ResourceHandler::LoadTextures()
{
	std::cout << "Convert to file loading!" << std::endl;
	textures.insert(std::pair<std::string, unsigned int>(std::string("ConstructorUV"), Texture("../res/textures/ConstructorUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("FlyerUV"), Texture("../res/textures/FlyerUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("debug"), Texture("../res/textures/debug.png").GetTextureId()));
}
void ResourceHandler::LoadShaders()
{
	std::cout << "Convert to file loading!" << std::endl;

		GLShader* shader = new GLShader();
		if (!shader->AddShaderFromFile("../res/shaders/phong.vert", GLShader::VERTEX))
			printf("Vert failed to compile.\n");
		if (!shader->AddShaderFromFile("../res/shaders/phong.frag", GLShader::FRAGMENT))
			printf("Frag failed to compile.\n");
		shader->Link();
		shaders.insert(std::pair<std::string, GLShader*>("Phong", shader));
		GLShader* shader1 = new GLShader();
		if (!shader1->AddShaderFromFile("../res/shaders/Basic.vert", GLShader::VERTEX))
			printf("Vert failed to compile.\n");
		if (!shader1->AddShaderFromFile("../res/shaders/Basic.frag", GLShader::FRAGMENT))
			printf("Frag failed to compile.\n");
		shader1->Link();
		shaders.insert(std::pair<std::string, GLShader*>("Basic", shader1));

		GLShader* shader2 = new GLShader();
		if (!shader2->AddShaderFromFile("../res/shaders/point.vert", GLShader::VERTEX))
			printf("Vert failed to compile.\n");
		if (!shader2->AddShaderFromFile("../res/shaders/point.frag", GLShader::FRAGMENT))
			printf("Frag failed to compile.\n");
		shader->Link();
		shaders.insert(std::pair<std::string, GLShader*>("Point", shader2));
		GLShader* shader3 = new GLShader();
		if (!shader3->AddShaderFromFile("../res/shaders/Particle.vert", GLShader::VERTEX))
			printf("Vert failed to compile.\n");
		if (!shader3->AddShaderFromFile("../res/shaders/Particle.frag", GLShader::FRAGMENT))
			printf("Frag failed to compile.\n");
		shader3->Link();
		shaders.insert(std::pair<std::string, GLShader*>("Particle", shader3));

		//GLShader* shader4 = new GLShader();
		//if (!shader3->AddShaderFromFile("../res/shaders/Particle.vert", GLShader::VERTEX))
		//	printf("Vert failed to compile.\n");
		//if (!shader3->AddShaderFromFile("../res/shaders/Particle.frag", GLShader::FRAGMENT))
		//	printf("Frag failed to compile.\n");
		//shader3->Link();
		//shaders.insert(std::pair<std::string, GLShader*>("Tex", shader4));
}


