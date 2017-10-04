#include "Effect2.h"
#include <fstream>


bool read_file(const std::string &filename, std::string &content)
{
	// Create filestream
	std::ifstream file(filename, std::ios_base::in);
	// Check that file exists.  If not, return false
	if (file.bad())
		return false;

	// File is good.  Read contents
	std::stringstream buffer;
	buffer << file.rdbuf();

	// Get contents from the file
	content = buffer.str();

	// Close file and return true
	file.close();
	return true;
}

bool checkFileExists(const std::string &filename)
{
	struct stat buffer;
	return stat(filename.c_str(), &buffer) == 0;
}

Effect2::Effect2()
{
}


Effect2::~Effect2()
{
}

void Effect2::addShader(const std::string& filename, const GLenum type)
{
	assert(checkFileExists(filename));

	std::string content;

	if (!read_file(filename, content))
	{
		std::cerr << "Shader fail, shader: " << filename << std::endl;
		return;
	}
	const auto id = glCreateShader(type);
	auto source = content.c_str();
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);
	GLint compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		GLsizei length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetShaderInfoLog(id, length, &length, &log[0]);
		std::cerr << "Shader failed to compile... " << filename << std::endl;
		std::cerr << &log[0] << std::endl;
		glDeleteShader(id);
	}
	std::clog << "LOG - " << filename << " added to effect" << std::endl;
	shaders.push_back(id);
}

void Effect2::addShader(const std::vector<std::string>& filenames, const GLenum type)
{
	assert(filenames.size() > 0);
	
	for (auto &name : filenames)
		checkFileExists(name);

	std::vector<std::string> fileContents;

	for(auto &name : filenames)
	{
		std::string content;
		if (!read_file(name, content))
		{
			std::cerr << "Shader fail when adding multiple shaders at once." << std::endl;
		}
		else
			fileContents.push_back(content);
	}

	const auto id = glCreateShader(type);

	const char **source = new const char*[fileContents.size()];
	for(unsigned int i = 0; i < fileContents.size(); ++i)
		source[i] = fileContents[i].c_str();
	
	glShaderSource(id, fileContents.size(), source, nullptr);
	glCompileShader(id);
	delete[] source;

	GLint compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		GLsizei length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetShaderInfoLog(id, length, &length, &log[0]);
		std::cerr << "Shader fail, when you called addShader for multiple shaders." << std::endl;
		std::cerr << &log[0] << std::endl;
		glDeleteShader(id);
	}
	std::clog << "LOG - " << std::endl;
	for (auto &name : filenames)
		std::clog << "\t" << name << std::endl;
	std::clog << " added to effect" << std::endl;
	shaders.push_back(id);
}

void Effect2::build()
{
	program = glCreateProgram();
	for(auto &id : shaders)
	{
		glAttachShader(program, id);
	}
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		GLsizei length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetProgramInfoLog(program, length, &length, &log[0]);
		std::cerr << "Shader program fail" << std::endl;
		std::cerr << &log[0] << std::endl;
		for(auto s :shaders)
		{
			glDetachShader(program, s);
			glDeleteShader(s);
		}
		glDeleteProgram(program);
	}
	std::clog << "Effect built!" << std::endl;
}

GLint Effect2::getUniformLocation(const std::string& name)
{
	const auto loc = glGetUniformLocation(program, name.c_str());
	return loc;
}
