#pragma once
#include <string>
#include <GL\glew.h>
#include <GL\GL.h>

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
class GLShader
{
	

private: 
	GLint program;
	bool linked;
	std::string logString;
	
	// MOVE THIS TO IO class.
	static bool FileExists(const std::string& fileName);

public:


	~GLShader();

	enum GLSLSHADERTYPE
	{
		VERTEX, FRAGMENT, GEOMETRY
	};

	GLShader() { program = glCreateProgram(); }

	GLuint GetId()
	{
		return program;
	}
	bool AddShaderFromFile(const char* fileName, GLSLSHADERTYPE type);
	bool Link();
	bool IsLinked();
	void Use();
	void SetUniform(const char* name, const float val);
	void SetUniform(const char* name, const int val);

	GLuint GetUniformLocation(const char* name);
};