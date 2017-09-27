#include "Free_Camera.h"

// Update free camera (delta_time is not used)
void Free_Camera::update(float delta_time)
{
	// Calculate the forward direction (spherical co-ordinates to Cartesian co-ordinates)
	// "Going to try and understand this a bit more." - Jack

	// Normalise forward direction


	// Calculate standard right. Rotate right vector by yaw.

	// Normalise right


	// Calculate up vector

	// Normalise up


	// Update position based on forward, up and right


	// Target vector is position vector plus forward


	// Reset the translation vector for the next frame

	
	// Calculate view matrix

}

// Rotate free camera around the y- and x-axes
void Free_Camera::rotate(float delta_yaw, float delta_pitch)
{
	// Add rotation values to current rotation
	_pitch += delta_pitch;
	_yaw -= delta_yaw;
}

// Move free camera (used in the update with the orientation
// to calculate actual movement)
void Free_Camera::move(const glm::vec3 &translation)
{
	// Add translation vector to current translation
	_translation += translation;
}