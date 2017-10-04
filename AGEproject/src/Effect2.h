#pragma once
#include "GameEngine.h"
class Effect2
{
	GLuint program;
	std::vector<GLuint> shaders;
public:
	Effect2();
	Effect2(const Effect2 &other) = default;
	Effect2& operator=(const Effect2 &other) = default;
	~Effect2();
	GLuint getProgram() const { return program; }
	void addShader(const std::string &filename, const GLenum type);
	void addShader(const std::vector<std::string> &filenames, const GLenum type);
	void build();
	GLint getUniformLocation(const std::string &name);
};

