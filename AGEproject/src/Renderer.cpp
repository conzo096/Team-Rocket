#include "Renderer.h"
#include <glm/gtc/type_ptr.inl>

Renderer *Renderer::instance = nullptr;

void Renderer::shutdown()
{
	instance->isRunning = false;
	glfwTerminate();
}

bool Renderer::initialise()
{
	// Let's leave this for now
	// Should probably set this window to a reference of the game engine's
	instance->window = GameEngine::Instance()->GetWindow();
	return true;
}

bool Renderer::beginRender()
{
	if(!instance->isRunning)
		return false;
	clear();
	return true;
}

void Renderer::endRender()
{
	if (!instance->isRunning)
		return;
	swapBuffers();
	glfwPollEvents();
}

void Renderer::clear()
{
	if(!instance->isRunning)
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::swapBuffers()
{
	if (!instance->isRunning)
		return;
	glfwSwapBuffers(instance->window);
}

void Renderer::bind(const Effect2& effect)
{
	assert(effect.getProgram != nullptr);
	instance->effect = effect;
	glUseProgram(effect.getProgram());
}

// Make sure this is fixed before you try to use it.
void Renderer::bind(const Texture& texture, int index)
{
	// TODO: Maybe fix texture so it can do this.
	assert(texture.id != 0);
	assert(index >= 0);
	glActiveTexture(GL_TEXTURE0 + index);
//	glBindTexture(texture.type, texture.id);
}

void Renderer::bind(const Material& material, const std::string& name)
{
	GLint idx;
	
	idx = instance->effect.getUniformLocation(name + ".emissive");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(material.emissive));

	idx = instance->effect.getUniformLocation(name + ".diffuseReflection");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(material.diffuse));

	idx = instance->effect.getUniformLocation(name + ".specularReflection");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(material.specular));

	idx = instance->effect.getUniformLocation(name + ".shininess");
	if (idx != -1)
		glUniform1f(idx, material.shininess);
}

void Renderer::bind(const PointLight& pointLight, const std::string& name)
{
	GLint idx;
	// Colours
	idx = instance->effect.getUniformLocation(name + ".ambient");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.ambient));
	idx = instance->effect.getUniformLocation(name + ".diffuse");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.diffuse));
	idx = instance->effect.getUniformLocation(name + ".specular");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.specular));
	// Position
	idx = instance->effect.getUniformLocation(name + ".position");
	if (idx != -1)
		glUniform4fv(idx, 1, value_ptr(pointLight.position));
	// Attenuation
	idx = instance->effect.getUniformLocation(name + ".constant");
	if (idx != -1)
		glUniform1f(idx, pointLight.constant);
	idx = instance->effect.getUniformLocation(name + ".linear");
	if (idx != -1)
		glUniform1f(idx, pointLight.linear);
	idx = instance->effect.getUniformLocation(name + ".quadratic");
	if (idx != -1)
		glUniform1f(idx, pointLight.quadratic);
	// Range
	idx = instance->effect.getUniformLocation(name + ".range");
	if (idx != -1)
		glUniform1f(idx, pointLight.range);
	// Intensities
	idx = instance->effect.getUniformLocation(name + ".ambientIntensity");
	if (idx != -1)
		glUniform1f(idx, pointLight.ambientIntensity);
	idx = instance->effect.getUniformLocation(name + ".diffuseIntensity");
	if (idx != -1)
		glUniform1f(idx, pointLight.diffuseIntensity);
	idx = instance->effect.getUniformLocation(name + ".specularIntensity");
	if (idx != -1)
		glUniform1f(idx, pointLight.specularIntensity);
}

void Renderer::bind(const std::vector<PointLight>& pointLights, const std::string& name)
{
	unsigned int n = 0;
	for(auto &p : pointLights)
	{
		std::stringstream stream;
		stream << name << '[' << n << ']';
		const auto point_name = stream.str();
		bind(p, point_name);
		++n;
	}
}

void Renderer::setRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
