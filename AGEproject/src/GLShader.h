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
	int GetUniformLocation(const char * name);
	
	// MOVE THIS TO IO class.
	static bool FileExists(const std::string& fileName);

public:

	enum GLSLSHADERTYPE
	{
		VERTEX, FRAGMENT, GEOMETRY
	};

	GLShader() { program = glCreateProgram(); }

	bool AddShaderFromFile(const char* fileName, GLSLSHADERTYPE type);
	bool Link();
	bool IsLinked();
	void Use();
	std::string Log();
	int GetHandle();

	void BindAttributeLocation(GLuint location, const char* name);
	void BindFragmentDataLocation(GLuint location, const char* name);

	void SetUniform(const char* name, const glm::vec3& v);
	void SetUniform(const char* name, const glm::vec4& v);
	void SetUniform(const char* name, const glm::mat3& m);
	void SetUniform(const char* name, const glm::mat4& m);
	void SetUniform(const char* name, const float val);
	void SetUniform(const char* name, const int val);
	void SetUniform(const char* name, const bool val);

	void PrintActiveUniforms();
	void PrintActiveAttribs();


};