#pragma once
#include "Camera.h"
#include "GameEngine.h"
#include "UserControls.h"

class Menu_Camera : public Camera
{
private:
	// Cursor's current position
	double cursorX;
	double cursorY;

public:
	Menu_Camera() :
		cursorX(0.0), cursorY(0.0),
		Camera("Menu_Camera")
	{
		glfwSetInputMode(GameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Update cursor position
		glfwGetCursorPos(GameEngine::Get().GetWindow(), &cursorX, &cursorY);
	}
	// Destroy free camera
	~Menu_Camera() {}

	// Update free camera
	void Update(double deltaTime) override;

	// Build projection matrix
	void SetProjection(float fov, float aspect, float near, float far)
	{
		projection = glm::perspective(fov, aspect, near, far);
	}

	// Only here so it compiles.
	void from_json(const json &j);
};