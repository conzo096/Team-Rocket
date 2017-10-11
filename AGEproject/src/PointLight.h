#pragma once
#include "Shader.h"

class PointLight : public Component
{
	void bind(const PointLight& pointLight, const std::string& name, const std::string& shaderName);
//	void bind(const std::vector<PointLight>& pointLights, const std::string& name);
	void from_json(const nlohmann::json &j) override;
	float range;
	void initialise();
	static int _id;
	static int id_counter;
public:
	glm::vec3 position;

	float constant  = 1.0f;
	float linear    = 0.7f;
	float quadratic = 1.8f;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	

	PointLight();
	PointLight(const glm::vec3 position, const glm::vec4 diffuse);
	PointLight(const PointLight &obj);

	int getId() { return _id; }

	~PointLight();

	void Render() override;
};

