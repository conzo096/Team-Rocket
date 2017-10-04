#pragma once

#include "GameEngine.h"
#include "PointLight.h"
#include "Effect2.h"
#include "Material.h"

class Renderer
{
	// TODO: Remove as necessary
	GLFWwindow *window;
	bool isRunning;
	unsigned int width, height;
	Effect2 effect;

	// Singleton instance of renderer
	static Renderer *instance;

	// Do not call
	Renderer(){}

	// Do not have copy constructor or assignment constructor
	Renderer(const Renderer&) = delete;
	void operator=(Renderer&) = delete;
public:
	~Renderer() { shutdown(); }
	static void shutdown();

	static GLFWwindow* getWindow() { return instance->window; }

	static bool checkIsRunning() { return instance->isRunning; }
	static void setIsRunning(const bool value) { instance->isRunning = value; }

	static unsigned int getScreenWidth() { return instance->width; }
	static unsigned int getScreenHeight() { return instance->height; }

	static const Effect2& getEffect() { return instance->effect; }

	// Doesn't do anything
	static bool initialise();

	static bool beginRender();
	static void endRender();

	// Clears buffer
	static void clear();
	// Swaps screen buffers
	static void swapBuffers();

	// Bind stuff using renderer
	static void bind(const Effect2 &effect);
	static void bind(const Texture &texture, int index);
	static void bind(const Material &material, const std::string &name);
	static void bind(const PointLight &pointLight, const std::string &name);
	static void bind(const std::vector<PointLight> &pointLights, const std::string &name);

	// Sets the render target of the renderer to the screen
	static void setRenderTarget();
};

