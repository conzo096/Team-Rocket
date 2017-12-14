#include "PointLight.h"
#include "Material.h"
#include <glm\gtc\type_ptr.hpp>
#include <assert.h>
int PointLight::id_counter = 0;
int PointLight::_id;

PointLight::PointLight() : Component("PointLight"), model(nullptr), effect(new Effect())
{
	this->position = glm::vec3(0.0f, 3.0f, 0.0f);

	this->ambient  = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
	this->diffuse  = glm::vec4(0.80f, 0.80f, 0.80f, 1.00f);
	this->specular = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
	
	initialise();
}

PointLight::PointLight(const glm::vec3 position, const glm::vec4 diffuse) : Component("PointLight")
{
	this->position = position;
	this->diffuse  = diffuse;

	this->ambient  = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
	this->specular = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);	

	initialise();
}

PointLight::PointLight(const PointLight& obj) : Component("PointLight")
{
	this->position = obj.position;
	this->ambient = obj.ambient;
	this->diffuse = obj.diffuse;
	this->specular = obj.specular;

	initialise();
}

bool saidLightError = false;
void PointLight::bind(const PointLight& pointLight, const std::string& name, const std::string& shaderName)
{
	GLint idx;
	char* s_name = const_cast<char*>(shaderName.c_str());
	auto shader = *ResourceHandler::Get().GetShader(s_name);
	// Colours
	idx = shader.GetUniformLocation(name + ".ambient");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.ambient));
	idx = shader.GetUniformLocation(name + ".diffuse");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.diffuse));
	idx = shader.GetUniformLocation(name + ".specular");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.specular));
	// Position
	idx = shader.GetUniformLocation(name + ".position");
	if (idx != -1)
		glUniform3fv(idx, 1, value_ptr(pointLight.position));
	// Range
	idx = shader.GetUniformLocation(name + ".range");
	if (idx != -1)
		glUniform1f(idx, pointLight.range);
	// Attenuation
	idx = shader.GetUniformLocation(name + ".constant");
	if(idx != -1)
		glUniform1f(idx, pointLight.constant);
	idx = shader.GetUniformLocation(name + ".linear");
	if (idx != -1)
		glUniform1f(idx, pointLight.linear);
	idx = shader.GetUniformLocation(name + ".quadratic");
	if (idx != -1)
		glUniform1f(idx, pointLight.quadratic);

	if (idx == -1 && !saidLightError)
	{
		std::cerr << "Error binding light to shader, could not find uniform: " << name << std::endl;
		std::cerr << "It may not be the only one." << std::endl;
		saidLightError = true;
	}
}

bool saidMatError = false;
void PointLight::bindMaterial(const Material& material, const std::string& name, const std::string& shaderName)
{
	GLint idx;
	char* s_name = const_cast<char*>(shaderName.c_str());
	auto shader = *ResourceHandler::Get().GetShader(s_name);
	idx = shader.GetUniformLocation(name + ".emissive");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(material.emissive));
	idx = shader.GetUniformLocation(name + ".diffuse_reflection");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(material.diffuse));
	idx = shader.GetUniformLocation(name + ".specular_reflection");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(material.specular));
	idx = shader.GetUniformLocation(name + ".shininess");
	if (idx != -1)
		glUniform1f(idx, material.shininess * 128);

	if(idx == -1 && !saidMatError)
	{
		std::cerr << "Error binding material to shader, could not find uniform: " << name << std::endl;
		std::cerr << "It may not be the only one." << std::endl;
		saidMatError = true;
	}
}

void PointLight::from_json(const nlohmann::json& j)
{
	// Leave this blank?
}

void PointLight::SetProperties(const std::string& filename)
{
	std::ifstream ifs(filename);
	json j = json::parse(ifs);

	this->SetEffect(j["Shader"]);

	json pos = j["Position"];
	std::vector<float> p;
	for(const auto elem : pos)
	{
		p.push_back(elem);
	}
	this->position = glm::vec3(p[0], p[1], p[2]);

	json dif = j["Colour"];
	std::vector<float> d;
	for(const auto elem : dif)
	{
		d.push_back(elem);
	}
	this->diffuse = glm::vec4(d[0], d[1], d[2], d[3]);

	this->ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
	this->specular = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
}

/*
void PointLight::bind(const std::vector<PointLight>& pointLights, const std::string& name)
{
	for (auto &p : pointLights)
	{
		std::stringstream stream;
		stream << name << '[' << this->_id << ']';
		const auto point_name = stream.str();
		bind(p, point_name);
	}
}
*/

void PointLight::initialise()
{
	this->_id = id_counter++;
//	const float lightMax = fmaxf(fmaxf(this->ambient.r, this->ambient.g), this->ambient.b);
//	this->range = (-this->linear + sqrtf(this->linear * this->linear - 4 * this->quadratic * (this->constant - 256.0 / 5.0 * lightMax))) / (2 * this->quadratic);
	this->range = 128;
	this->SetPosition(this->position);
}


PointLight::~PointLight()
{
}

void PointLight::SetEffect(const std::string shaderName)
{
	effect->shader = shaderName;
}

void PointLight::setLightPosition(const glm::vec3 position)
{
	this->position = position;
	SetPosition(position); // Sets the transform position
}

void PointLight::Render()
{
	std::stringstream s;
	s << this->_id;
	// Use renderer, bind.
	bind(*this, "point_light[" + s.str() + ']', effect->shader);
}
