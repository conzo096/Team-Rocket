#include "Shader.h"

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
			printf("Vert failed to compile.\n");
		if (!shader.AddShaderFromFile(("../res/shaders/" + name + ".frag").c_str(), GLShader::FRAGMENT))
			printf("Frag failed to compile.\n");
		shader.Link();
		shaders.insert(std::pair<std::string, GLShader>(name, shader));
	}
}

void Shader::UseShader(std::string name, Effect effect, glm::mat4 mvp, glm::mat4 m, glm::mat4 n, glm::vec3 eye_pos)
{
	if (shaders.find(name) == shaders.end())
	{
		return;
	}
	else if(&effect.texture != nullptr)
	{
		char* s_name = const_cast<char*>(name.c_str());
		GLShader shader = GetShader(s_name);
		shader.Use();
		if (name == "Basic")
		{
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, value_ptr(mvp));
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glUniformMatrix4fv(shader.GetUniformLocation("M"), 1, GL_FALSE, value_ptr(m));
			glUniformMatrix3fv(shader.GetUniformLocation("N"), 1, GL_FALSE, value_ptr(glm::mat3(n)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, effect.texture);
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
		}
		else if (name == "Colour")
		{
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, value_ptr(mvp));
		}

	}

}