#pragma once
#include "Camera.h"
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

public:
	// Create free camera
	Free_Camera() : pitch(0.0f), yaw(0.0f), Camera("Free_Camera") {}
	// Destroy free camera
	~Free_Camera() {}
	// Update free camera
	void Update(float deltaTime);

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
};