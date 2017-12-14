#include "Game_Camera.h"
#include "UserControls.h"

// Get camera's current rotation
double Game_Camera::GetYaw() const { return yaw; }

// Set camera's current rotation
void Game_Camera::SetYaw(double value)
{
	SetPosition(glm::dvec3(pitch, value, 0.0));
}

// Update camera for this frame
void Game_Camera::Update(double deltaTime)
{
	// The camera's movement speed
	float moveSpeed = 30.0f;

	glfwSetInputMode(GameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Get current cursor position
	glfwGetCursorPos(GameEngine::Get().GetWindow(), &cursorX, &cursorY);

	// Move camera with user controls.
	if (UserControls::Get().IsKeyPressed(std::string("Escape")))
		glfwSetWindowShouldClose(GameEngine::Get().GetWindow(), true);

	if (UserControls::Get().IsKeyPressed(std::string("RotateLeft")) || UserControls::Get().GetAxisValue(std::string("leftRightRightSticker")) < -0.7)
		Rotate(-2 * deltaTime);
	else if (UserControls::Get().IsKeyPressed(std::string("RotateRight")) || UserControls::Get().GetAxisValue(std::string("leftRightRightSticker")) > 0.7)
		Rotate(2 * deltaTime);

	if (UserControls::Get().IsKeyPressed(std::string("Forward"))  || UserControls::Get().GetAxisValue(std::string("upDownLeftSticker")) > 0.7f)
		translation += (glm::vec3(0.0f, 0.0f, 1.0f) * float(deltaTime) * moveSpeed);
	if (UserControls::Get().IsKeyPressed(std::string("Left")) || UserControls::Get().GetAxisValue(std::string("leftRightLeftSticker")) < -0.7f)
		translation += (glm::vec3(-1.0f, 0.0f, 0.0f) * float(deltaTime) * moveSpeed);
	if (UserControls::Get().IsKeyPressed(std::string("Backward")) || UserControls::Get().GetAxisValue(std::string("upDownLeftSticker")) < -0.7f)
		translation += (glm::vec3(0.0f, 0.0f, -1.0f) * float(deltaTime) * moveSpeed);
	if (UserControls::Get().IsKeyPressed(std::string("Right")) || UserControls::Get().GetAxisValue(std::string("leftRightLeftSticker")) > 0.7f)
		translation += (glm::vec3(1.0f, 0.0f, 0.0f) * float(deltaTime) * moveSpeed);
	if (UserControls::Get().IsKeyPressed(std::string("ZoomIn")) || UserControls::Get().GetAxisValue(std::string("leftTrigger")) > 0.7)
		translation += (glm::vec3(0.0f, 1.0f, 0.0f) * float(deltaTime) * moveSpeed);
	if (UserControls::Get().IsKeyPressed(std::string("ZoomOut")) || UserControls::Get().GetAxisValue(std::string("rightTrigger")) > 0.7)
		translation += (glm::vec3(0.0f, -1.0f, 0.0f) * float(deltaTime) * moveSpeed);

	// Calculate the forward direction (spherical co-ordinates to Cartesian co-ordinates)
	glm::dvec3 temp_forward(cosf(pitch) * -sinf(yaw), sinf(pitch), -cosf(yaw) * cosf(pitch));
	// Normalise forward direction
	forward = glm::normalize(temp_forward);

	// Create standard right vector and rotate it by the yaw
	right = glm::dvec3(glm::eulerAngleY(yaw) * glm::dvec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Normalise right
	right = glm::normalize(right);

	// Calculate (perpendicular) up vector using cross product
	up = glm::cross(right, forward);
	// Normalise up
	up = glm::normalize(up);

	// Update position by multiplying translation elements by forward, up and right
	glm::dvec3 trans = translation.x * right;
	trans += translation.y * forward;
	trans += translation.z * glm::dvec3(forward.x, translation.y, forward.z);
	Move(trans);

	// Target vector is position vector plus forward
	target = GetPosition() + forward;

	// Reset the translation vector for the next frame
	translation = glm::vec3(0.0f, 0.0f, 0.0f);

	// Calculate view matrix
	view = glm::lookAt(GetPosition(), target, up);
}

// Rotates the camera by the change in yaw
void Game_Camera::Rotate(double deltaYaw)
{
	yaw -= deltaYaw;
}

// Build projection matrix
void Game_Camera::SetProjection(float fov, float aspect, float near, float far)
{
	projection = glm::perspective(fov, aspect, near, far);
}

void Game_Camera::from_json(const json &j) {}