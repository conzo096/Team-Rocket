#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <sstream>
#include "Singleton.h"
#include "Material.h"
#include "Model.h"

class Material;

struct Effect
{
	unsigned int texture;
	std::string shader;
	Material* material;
};

struct RenderData
{
	// Shader object.
	unsigned int shader;
	// Change to vector later?
	unsigned int texture;
	// Model information.
	int modelVao;
	GLenum drawType;
	int indices;

	// Lighting info to do.
	// **
	// **
	// Model matrix;
	glm::mat4 m;

	Material* mat;
};

class GameEngine : public Singleton<GameEngine>
{
private:

	// The window that is to be rendered too.
	GLFWwindow* window;
	int width;
	int height;
	bool fullScreen;
	glm::mat4 cameraMVP;
	glm::vec3 cameraPos;

	std::vector<RenderData> renderList;
public:

	// The render window.
	GLFWwindow* GetWindow() { return window; }

	void Initialise();
	//void Render(glm::mat4 mvp, Model model, Effect effect);

	// Getters for width and height
	int GetScreenWidth() { return width; }
	int GetScreenHeight() { return height; }
	bool GetFullScreen() { return fullScreen; }
	void SetFullScreen(int val) { fullScreen = val; }
	void SetScreenWidth(int val) { width = val; }
	void SetScreenHeight(int val) { height = val; }
	void SetCameraPos(glm::vec3 pos) { cameraPos = pos; }

	void SetCamera(glm::mat4 camera);
	// Execute the game engine.
	void Start();
	// Cleans up game engine resources.
	void CleanUp();

	void AddToRenderList(RenderData list);
	void Render();

	void BindMaterial(const Material* material, const int shaderID);
	// Helper functions.
	void PrintGlewInfo();
	//void LoadShaders();

};