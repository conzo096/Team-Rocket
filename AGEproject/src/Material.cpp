#include "Material.h"

// Here's a bunch of free materials: http://devernay.free.fr/cours/opengl/materials.html

Material::Material()
{
	// Default is emerald:
	emissive  = glm::vec4(0.05, 0.05, 0.05, 1.0);
	diffuse  = glm::vec4(0.07568, 0.61424, 0.07568, 1.0);
	specular = glm::vec4(0.633, 0.727811, 0.633, 1.0);
	shininess = 0.6f;
	shininess *= 128;
}

Material::~Material()
{
}

void Material::Render()
{

}
