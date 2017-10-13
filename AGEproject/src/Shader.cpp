#include "Shader.h"

Shader *instance = nullptr;
std::map<std::string, GLShader> Shader::shaders;
std::map<std::string, unsigned int> Shader::textures;


unsigned int Shader::AddTexture(std::string name)
{
	if (textures.find(name) == textures.end())
	{
		Texture temp(("../res/textures/" + name + ".png").c_str());
		textures.insert(std::pair<std::string, unsigned int>(name,temp.GetTextureId()));
	}
	return textures[name];
}

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
		shaders.insert(std::pair<std::string, GLShader>(name, shader));
	}
}

void Shader::UseShader(const std::string name, const Effect effect, glm::mat4 mvp, glm::mat4 m, glm::mat4 n, glm::vec3 eye_pos)
{
	if (shaders.find(name) == shaders.end())
	{
		return;
	}
	else if(&effect.texture != nullptr)
	{
		GLShader shader = shaders[name];
		shader.Use();
		if (name == "Basic")
		{
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, value_ptr(mvp));
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glUniformMatrix4fv(shader.GetUniformLocation("M"), 1, GL_FALSE, value_ptr(m));
			glUniformMatrix3fv(shader.GetUniformLocation("N"), 1, GL_FALSE, value_ptr(glm::mat3(n)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, effect.texture);
			return;
		}
		else if (name == "Phong")
		{
			GLint index;

			index = shader.GetUniformLocation("MVP");
			glUniformMatrix4fv(index, 1, GL_FALSE, value_ptr(mvp));
			index = shader.GetUniformLocation("tex");
			glUniform1i(index, 0);
			index = shader.GetUniformLocation("M");
			glUniformMatrix4fv(index, 1, GL_FALSE, value_ptr(m));
			index = shader.GetUniformLocation("N");
			glUniformMatrix3fv(index, 1, GL_FALSE, value_ptr(glm::mat3(n)));
			index = shader.GetUniformLocation("eye_pos");
			glUniform3fv(index, 1, value_ptr(eye_pos));
//			std::cout << "Eye pos: " << eye_pos.x << ", " << eye_pos.y << ", " << eye_pos.z << std::endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, effect.texture);
			return;
		}
		else if (name == "Colour")
		{
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, value_ptr(mvp));
		}

	}

}

GLShader Shader::getShader(const std::string name)
{
	return shaders[name];
}
