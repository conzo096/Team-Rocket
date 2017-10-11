#pragma once
#include "Camera.h"
#include "GameEngine.h"
#include "UserControls.h"
#include "glm\gtx\euler_angles.hpp"

class Free_Camera : public Camera
{
private:
	// Current rotation on the x-axis
	float pitch;
	// Current rotation on the y-axis
	float yaw;
	// Camera's translation since previous frame
	glm::dvec3 translation;
	// Cursor's current position
	double cursorX;
	double cursorY;

public:
	// Create free camera
	Free_Camera(float aspect, float fov) : 
		pitch(0.0f), yaw(0.0f), 				   
		cursorX(0.0), cursorY(0.0),					   
		Camera("Free_Camera", aspect, fov) 
	{
	//	glfwSetInputMode(GameEngine::Instance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Update cursor position
		glfwGetCursorPos(GameEngine::Instance()->GetWindow(), &cursorX, &cursorY);
	}
	// Destroy free camera
	~Free_Camera() {}
	// Update free camera
	void Update(double deltaTime) override;

	// Get camera's current rotations
	float GetPitch() const
	{
		return GetPosition().x;
	}
	float GetYaw() const
	{
		return GetPosition().y;
	}

	// Set camera's current rotations
	void SetYaw(float value)
	{
		SetPosition(glm::dvec3(GetPitch(), value, 0.0f));
	}
	void SetPitch(float value)
	{
		SetPosition(glm::dvec3(value, GetYaw(), 0.0f));
	}

	// Only here so it compiles.
	void from_json(const json &j);

	void Rotate(float deltaYaw, float deltaPitch)
	{
		pitch += deltaPitch;
		yaw -= deltaYaw;
	}
};