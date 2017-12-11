#include "Material.h"

// Here's a bunch of free materials: http://devernay.free.fr/cours/opengl/materials.html

Material::Material()
{
	// Default is emerald:
	emissive = glm::vec4(0.04, 0.04, 0.04, 1.0);
	diffuse = glm::vec4(0.6, 0.6, 0.6, 1.0);
	specular = glm::vec4(0.7, 0.7, 0.7, 1.0);
	shininess = 0.22f;
	shininess *= 128;


}


Material::~Material()
{
}

void Material::Render()
{

}
