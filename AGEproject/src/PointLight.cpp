#include "PointLight.h"


PointLight::PointLight() : Component("PointLight")
{
	position = glm::vec3(0.0f, 3.0f, 0.0f);

	constant  = 1.000f;
	linear    = 0.090f;
	quadratic = 0.032f;

	ambient  = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
	diffuse  = glm::vec4(0.80f, 0.80f, 0.80f, 1.00f);
	specular = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
	
	range = 10.0f;

	ambientIntensity  = 0.1f;
	diffuseIntensity  = 1.0f;
	specularIntensity = 0.5f;
}

PointLight::PointLight(const glm::vec3 position, const glm::vec4 diffuse) : Component("PointLight")
{
	this->position = position;
	this->diffuse  = diffuse;

	constant  = 1.000f;
	linear    = 0.090f;
	quadratic = 0.032f;

	ambient  = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
	specular = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);

	range = 10.0f;

	ambientIntensity  = 0.1f;
	diffuseIntensity  = 1.0f;
	specularIntensity = 0.5f;
}

void PointLight::bind(const PointLight& pointLight, const std::string& name)
{
	GLint idx;
	auto effect = Shader::getShader("phong");
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
	// Attenuation
	idx = effect.GetUniformLocation(name + ".constant");
	if (idx != -1)
		glUniform1f(idx, pointLight.constant);
	idx = effect.GetUniformLocation(name + ".linear");
	if (idx != -1)
		glUniform1f(idx, pointLight.linear);
	idx = effect.GetUniformLocation(name + ".quadratic");
	if (idx != -1)
		glUniform1f(idx, pointLight.quadratic);
	// Range
	idx = effect.GetUniformLocation(name + ".range");
	if (idx != -1)
		glUniform1f(idx, pointLight.range);
	// Intensities
	idx = effect.GetUniformLocation(name + ".ambientIntensity");
	if (idx != -1)
		glUniform1f(idx, pointLight.ambientIntensity);
	idx = effect.GetUniformLocation(name + ".diffuseIntensity");
	if (idx != -1)
		glUniform1f(idx, pointLight.diffuseIntensity);
	idx = effect.GetUniformLocation(name + ".specularIntensity");
	if (idx != -1)
		glUniform1f(idx, pointLight.specularIntensity);
}

void PointLight::bind(const std::vector<PointLight>& pointLights, const std::string& name)
{
	unsigned int n = 0;
	for (auto &p : pointLights)
	{
		std::stringstream stream;
		stream << name << '[' << n << ']';
		const auto point_name = stream.str();
		bind(p, point_name);
		++n;
	}
}


PointLight::~PointLight()
{
}

void PointLight::Render()
{
	// Use renderer, bind.
	bind(*this, "pointLight");
}
