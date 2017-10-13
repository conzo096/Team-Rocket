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
	void bindMaterial(const Material& material, const std::string& name, const std::string& shaderName);

	float range;
	void initialise();
	static int _id;
	static int id_counter;
	glm::vec3 position;
public:
	PointLight();
	~PointLight();

	void SetEffect(const std::string shaderName);
	void setLightPosition(const glm::vec3 position);
	

	float constant  = 1.0f;
	float linear    = 0.001f;
	float quadratic = 0.00001f;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	

	PointLight(const glm::vec3 position, const glm::vec4 diffuse);
	PointLight(const PointLight &obj);

	int getId() { return _id; }

	void Render() override;
};

