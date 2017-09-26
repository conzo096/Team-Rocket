#include "PointLight.h"



PointLight::PointLight()
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


PointLight::~PointLight()
{
}
