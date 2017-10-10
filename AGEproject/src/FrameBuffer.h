#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include "Texture.h"
class FrameBuffer
{
private:

	// The OpenGL ID of the frame buffer
	GLuint bufferId;
	// The texture representing the captured frame
	Texture frameTexture;
	// The texture representing the captured depth buffer
	Texture depthTexture;
	// The width of the frame buffer
	GLuint width;
	// The height of the frame buffer
	GLuint height;


public:

	FrameBuffer(){}


	~FrameBuffer() { glDeleteFramebuffers(1, &bufferId); }
	
	GLuint GetFrameBuffer()
	{
		return bufferId;
	}
	
	// Return the texture generated.
	Texture GetFrameTexture()
	{
		return frameTexture;
	}
	
	Texture GetDepthTexture()
	{
		return depthTexture;
	}
	
	// Create the frame buffer.
	void LoadFrameBuffer(int w, int h);

	// Bind render to this buffer.
	void BindFrameBuffer();


};