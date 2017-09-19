#include "GLShader.h"

#include <fstream>
#include <vector>
bool GLShader::AddShaderFromFile(const char* fileName, GLShader::GLSLSHADERTYPE type)
{
	// If file does not exist exit.
	if (!FileExists(fileName))
	{
		return false;
	}

	std::ifstream ifs(fileName);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	const GLchar *source = (const GLchar *)content.c_str();
	// Create shader depending on type.
	if (type == VERTEX)
	{
		//Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
 		glShaderSource(vertexShader, 1, &source, 0);

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
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &source, 0);
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
		GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &fileName, 0);

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
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
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
}



// This should be move to IO Class.
bool GLShader::FileExists(const std::string& fileName)
{
	std::ifstream infile(fileName);
	bool good =  infile.good();
	if(!good)
		std::fprintf(stderr, "File %s does not exist!", fileName);
	return good;
}