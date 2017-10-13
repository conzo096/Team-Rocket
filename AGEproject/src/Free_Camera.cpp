#include "Free_Camera.h"

// Update free camera for this frame
void Free_Camera::Update(double deltaTime)
{
	glfwSetInputMode(GameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// The ratio of pixels to rotation
	double ratioWidth = fieldOfView / static_cast<float>(GameEngine::Get().GetScreenWidth());
	double ratioHeight = ( fieldOfView * (static_cast<float>(GameEngine::Get().GetScreenHeight()) 
									  / static_cast<float>(GameEngine::Get().GetScreenWidth())) ) 
									  / static_cast<float>(GameEngine::Get().GetScreenHeight());

	double currentX;
	double currentY;

	// The camera's movement speed
	float moveSpeed = 20.0f;

	// Get current cursor position
	glfwGetCursorPos(GameEngine::Get().GetWindow(), &currentX, &currentY);

	// Calculate delta of cursor positions from last frame
	double deltaX = (currentX - cursorX);
	double deltaY = (cursorY - currentY);


	// Multiply deltas by ratios to get change in orientation
	deltaX *= ratioWidth;
	deltaY *= ratioHeight;

	// Rotate camera by deltas
	Rotate(deltaX, deltaY);

	if (UserControls::Get().IsKeyPressed(std::string("RotateLeft")))
		GetParent()->Rotate(glm::vec3(30, 0, 0) * float(deltaTime) * moveSpeed);
	if (UserControls::Get().IsKeyPressed(std::string("RotateRight")))
		GetParent()->Rotate(glm::vec3(-30, 0, 0) * float(deltaTime) * moveSpeed);

	// Move camera with user controls.
	if (UserControls::Get().IsKeyPressed(std::string("Forward")))
		(translation += (glm::vec3(0.0f, 0.0f, 1.0f) * float(deltaTime) * moveSpeed));
	if (UserControls::Get().IsKeyPressed(std::string("Left")))
		(translation += (glm::vec3(-1.0f, 0.0f, 0.0f) * float(deltaTime) * moveSpeed));
	if (UserControls::Get().IsKeyPressed(std::string("Backward")))
		(translation += (glm::vec3(0.0f, 0.0f, -1.0f) * float(deltaTime) * moveSpeed));
	if (UserControls::Get().IsKeyPressed(std::string("Right")))
		(translation += (glm::vec3(1.0f, 0.0f, 0.0f) * float(deltaTime) * moveSpeed));
	if (UserControls::Get().IsKeyPressed(std::string("Up")))
		(translation += (glm::vec3(0.0f, 1.0f, 0.0f) * float(deltaTime) * moveSpeed));
	if (UserControls::Get().IsKeyPressed(std::string("Down")))
		(translation += (glm::vec3(0.0f, -1.0f, 0.0f) * float(deltaTime) * moveSpeed));
	if (UserControls::Get().IsKeyPressed(std::string("Escape")))
		glfwSetWindowShouldClose(GameEngine::Get().GetWindow(), true);

	// Calculate the forward direction (spherical co-ordinates to Cartesian co-ordinates)
	glm::dvec3 forward(cosf(pitch) * -sinf(yaw), sinf(pitch), -cosf(yaw) * cosf(pitch));
	// Normalise forward direction
	forward = glm::normalize(forward);

	// Create standard right vector and rotate it by the yaw
	glm::dvec3 right = glm::dvec3(glm::eulerAngleY(yaw) * glm::dvec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Normalise right
	right = glm::normalize(right);

	// Calculate (perpendicular) up vector using cross product
	orientation = glm::cross(right, forward);
	// Normalise up
	orientation = glm::normalize(orientation);

	// Update position by multiplying translation elements by forward, up and right
	glm::dvec3 trans = translation.x * right;
	trans += translation.y * orientation;
	trans += translation.z * forward;
	Move(trans);

	// Target vector is position vector plus forward
	target = GetPosition() + forward;

	// Reset the translation vector for the next frame
	translation = glm::vec3(0.0f, 0.0f, 0.0f);

	// Calculate view matrix
	view = glm::lookAt(GetPosition(), target, orientation); //glm::eulerAngles(GetRotation()));

	// Update cursor position
	glfwGetCursorPos(GameEngine::Get().GetWindow(), &cursorX, &cursorY);
}

void Free_Camera::from_json(const json &j) {}
