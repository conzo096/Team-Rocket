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
	// The current field of view
	float fieldOfView;

public:
	// Create free camera
	Free_Camera(float fov) : 
		pitch(0.0f), yaw(0.0f), 				   
		cursorX(0.0), cursorY(0.0),		
		fieldOfView(fov),
		Camera("Free_Camera") 
	{
		glfwSetInputMode(GameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Update cursor position
		glfwGetCursorPos(GameEngine::Get().GetWindow(), &cursorX, &cursorY);
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

	// Build projection matrix
	void SetProjection(float aspect, float near, float far)
	{
		projection = glm::perspective(fieldOfView, aspect, near, far);
	}

	// Rotates the camera by the change in pitch and yaw
	void Rotate(float deltaYaw, float deltaPitch)
	{
		pitch += deltaPitch;
		yaw -= deltaYaw;
	}

	// Only here so it compiles.
	void from_json(const json &j);
};