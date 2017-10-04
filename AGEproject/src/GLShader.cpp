#include "GLShader.h"

#include <fstream>
#include <vector>

GLShader::~GLShader()
{

}


bool GLShader::AddShaderFromFile(const char* fileName, GLSLSHADERTYPE type)
{
	// If file does not exist exit.
	if (!FileExists(fileName))
	{
		return false;
	}

	std::ifstream ifs(fileName);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	const GLchar *source = static_cast<const GLchar *>(content.c_str());
	// Create shader depending on type.
	if (type == VERTEX)
	{
		//Create an empty vertex shader handle
		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
 		glShaderSource(vertexShader, 1, &source, nullptr);

		//Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			//We don't need the shader anymore.
			glDeleteShader(vertexShader);
			return false;
		}
		glAttachShader(program, vertexShader);
//		glDeleteShader(vertexShader);
	}
	else if (type == FRAGMENT)
	{
		//Create an empty vertex shader handle
		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &source, nullptr);
		glCompileShader(fragmentShader);
		GLint isCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			//We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			return false;
		}
		glAttachShader(program, fragmentShader);
//		glDeleteShader(fragmentShader);
	}
	// Need to apply check to test if it is geometry.
	else if (type == GEOMETRY)
	{
		//Create an empty vertex shader handle
		const GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &source, nullptr);

		//Compile the vertex shader
		glCompileShader(geometryShader);

		GLint isCompiled = 0;
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);
			//We don't need the shader anymore.
			glDeleteShader(geometryShader);
			return false;
		}
		glAttachShader(program, geometryShader);
	}

		return true;
}


bool GLShader::Link()
{
	glLinkProgram(program);
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, static_cast<int *>(&isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		//We don't need the program anymore.
		glDeleteProgram(program);
		return false;
	}
	return true;
}

// Checks if the shader has been linked together successfully or not.
bool GLShader::IsLinked()
{
	GLint isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
		return false;
	return true;
}


void GLShader::SetUniform(const char* name, float val)
{
	const GLint loc = glGetUniformLocation(program, name);
	if (loc != -1)
	{
		glUniform1f(loc, val);
	}
	else
		fprintf(stderr, "%s is not a parameter!",name);
}
void GLShader::SetUniform(const char* name, int val)
{
	const GLint loc = glGetUniformLocation(program, name);
	if (loc != -1)
	{
		glUniform1i(loc, val);
	}
	else
		fprintf(stderr,"%s is not a parameter!",name);
}

// Returns -1 if location does not exist.
GLuint GLShader::GetUniformLocation(const char* name)
{
	const GLuint loc = glGetUniformLocation(program, name);
	return loc;
}

GLuint GLShader::GetUniformLocation(std::string name)
{
	const GLuint loc = glGetUniformLocation(program, name.c_str());
	return loc;
}

void GLShader::Use()
{
	glUseProgram(program);
}

// This should be move to IO Class.
bool GLShader::FileExists(const std::string& fileName)
{
	std::ifstream infile(fileName);
	const bool good =  infile.good();
	if(!good)
		fprintf(stderr, "File %s does not exist!", fileName.c_str());
	return good;
}