#include "Free_Camera.h"

// Update free camera (delta_time is not used)
void Free_Camera::update(float delta_time)
{
	//// "I'd like to try and understand what's going on here a bit more." - Jack

	//// Calculate the forward direction (spherical co-ordinates to Cartesian co-ordinates)
	//glm::vec3 forward(cosf(_pitch) * -sinf(_yaw), sinf(_pitch), -cosf(_yaw) * cosf(_pitch));
	//// Normalise forward direction
	//forward = glm::normalize(forward);

	//// Create standard right vector and rotate it by the yaw
	//glm::vec3 right = glm::vec3(glm::eulerAngleY(_yaw) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//// Normalise right
	//right = glm::normalize(right);

	//// Calculate (perpendicular) up vector using cross product
	//_orientation = glm::cross(right, forward);
	//// Normalise up
	//_orientation = glm::normalize(_orientation);
	//
	//// Update position by multiplying translation elements by forward, up and right
	//glm::vec3 trans = _translation.x * right;
	//trans += _translation.y * _orientation;
	//trans += _translation.z * forward;
	//_position += trans;

	//// Target vector is position vector plus forward
	//_target = _position + forward;

	//// Reset the translation vector for the next frame
	//_translation = glm::vec3(0.0f, 0.0f, 0.0f);
	//
	//// Calculate view matrix
	//_view = glm::lookAt(_position, _target, _orientation);
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