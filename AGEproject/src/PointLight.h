#pragma once
#include "Entity.h"
#include "Shader.h"

class PointLight : public Component
{
protected:
	Model* model;
	Effect* effect;
	void from_json(const nlohmann::json &j) override;

	void bind(const PointLight& pointLight, const std::string& name, const std::string& shaderName);
//	void bind(const std::vector<PointLight>& pointLights, const std::string& name);
	
	float range;
	void initialise();
	static int _id;
	static int id_counter;
public:
	PointLight();
	~PointLight();

	glm::vec3 position;

	void SetEffect(std::string shaderName);

	float constant  = 1.0f;
	float linear    = 0.7f;
	float quadratic = 1.8f;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	

	PointLight(const glm::vec3 position, const glm::vec4 diffuse);
	PointLight(const PointLight &obj);

	int getId() { return _id; }

	void Render() override;
};

