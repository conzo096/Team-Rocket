#pragma once
#include "Camera.h"
#include "GameEngine.h"

class Game_Camera : public Camera
{
private:
	// Fixed rotation on the x-axis
	const double pitch = -3.0 / glm::pi<double>();
	// Current rotation on the y-axis
	double yaw;
	// Camera's translation since previous frame
	glm::dvec3 translation;
	// Cursor's current position
	double cursorX;
	double cursorY;

public:
	Game_Camera() :
		cursorX(0.0), cursorY(0.0),
		Camera("Game_Camera")
	{
		glfwSetInputMode(GameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Update cursor position
		glfwGetCursorPos(GameEngine::Get().GetWindow(), &cursorX, &cursorY);
	}
	
	~Game_Camera() {}
	
	// Get camera's current rotation
	double GetYaw() const;

	// Set camera's current rotation
	void SetYaw(double value);

	// Update free camera
	void Update(double deltaTime) override;

	// Rotates the camera by the change in yaw
	void Rotate(double deltaYaw);

	// Build projection matrix
	void SetProjection(float fov, float aspect, float near, float far);

	// Only here so it compiles.
	void from_json(const json &j);
};