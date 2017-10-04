#include "Shader.h"

Shader *instance = 0;
std::map<std::string, GLShader> Shader::shaders;

void Shader::AddShader(std::string name)
{
	if (shaders.find(name) != shaders.end())
	{
		return;
	}
	else
	{
		GLShader shader;
		if (!shader.AddShaderFromFile(("../res/shaders/" + name + ".vert").c_str(), GLShader::VERTEX))
			std::printf("Vert failed to compile.\n");
		if (!shader.AddShaderFromFile(("../res/shaders/" + name + ".frag").c_str(), GLShader::FRAGMENT))
			std::printf("Frag failed to compile.\n");
		shader.Link();
		shaders.insert(std::pair<std::string,GLShader>(name, shader));
	}
}

void Shader::UseShader(std::string name, Effect effect, glm::mat4 mvp)
{
	if (shaders.find(name) == shaders.end())
	{
		return;
	}
	else
	{
		shaders[name].Use();
		if (name == "Basic")
		{
			glUniformMatrix4fv(shaders[name].GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			glUniform1i(shaders[name].GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, effect.texture);
			return;
		}
		else if (name == "Phong")
		{
			glUniformMatrix4fv(shaders[name].GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			glUniform1i(shaders[name].GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, effect.texture);
			return;
		}
		else if (name == "Colour")
		{
			glUniformMatrix4fv(shaders[name].GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		}

	}

}

GLShader Shader::getShader(const std::string name)
{
	return shaders[name];
}
