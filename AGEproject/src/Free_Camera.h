#pragma once
#include "Camera.h"
#include "glm/gtx/euler_angles.hpp"

class Free_Camera : public Camera
{
private:
	// Current rotation on the x-axis
	float _pitch = 0.0f;
	// Current rotation on the y-axis
	float _yaw = 0.0f;
	// Camera's translation since previous frame
	glm::vec3 _translation;
public:
	// Create free camera
//	Free_Camera() : Camera() {}
	// Destroy free camera
	~Free_Camera() {}
	// Update free camera
	void update(float delta_time);
	// Get camera's current rotations
	float get_pitch() const
	{
		return _pitch;
	}
	float get_yaw() const
	{
		return _yaw;
	}
	// Set camera's current rotations
	void set_yaw(float value)
	{
		_yaw = value;
	}
	void set_pitch(float value)
	{
		_pitch = value;
	}
	// Rotates the camera by the given yaw and pitch
	void rotate(float delta_yaw, float delta_pitch);
	// Move the camera
	void move(const glm::vec3 &translation);
};