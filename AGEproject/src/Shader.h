#pragma once
#include <memory>
#include <map>
#include <GL\glew.h>
#include <GL\GL.h>
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "GLShader.h"
#include "Entity.h"
#include "Texture.h"
#include "Singleton.h"
class Shader : public Singleton<Shader>
{
private:

	std::map<std::string, GLShader> shaders;
	std::map<std::string, unsigned int> textures;

public:
	
	unsigned int AddTexture(std::string name);
	void AddShader(std::string name);
	void UseShader(std::string name, Effect effect, glm::mat4 mvp, glm::mat4 m, glm::mat4 n, glm::vec3 eye_pos);

	GLShader GetShader(char* loc)
	{
		return shaders.find(loc)->second;
	}
};