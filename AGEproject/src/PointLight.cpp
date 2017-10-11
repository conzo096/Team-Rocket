#include "PointLight.h"

int PointLight::id_counter = 0;
int PointLight::_id;

PointLight::PointLight() : Component("PointLight")
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

void PointLight::bind(const PointLight& pointLight, const std::string& name, const std::string& shaderName)
{
	GLint idx;
	auto effect = Shader::getShader(shaderName);
	// Colours
	idx = effect.GetUniformLocation(name + ".ambient");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.ambient));
	idx = effect.GetUniformLocation(name + ".diffuse");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.diffuse));
	idx = effect.GetUniformLocation(name + ".specular");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.specular));
	// Position
	idx = effect.GetUniformLocation(name + ".position");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.position));
	// Range
	idx = effect.GetUniformLocation(name + ".range");
	if (idx != -1)
		glUniform1f(idx, pointLight.range);
}

void PointLight::from_json(const nlohmann::json& j)
{

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
	const float lightMax = fmaxf(fmaxf(this->ambient.r, this->ambient.g), this->ambient.b);
	this->range = (-this->linear + sqrtf(this->linear * this->linear - 4 * this->quadratic * (this->constant - 256.0 / 5.0 * lightMax))) / (2 * this->quadratic);
}


PointLight::~PointLight()
{
}

void PointLight::Render()
{
	std::stringstream s;
	s << this->_id;
	// Use renderer, bind.
	bind(*this, "point_light[" + s.str() + ']', "Phong");
}
