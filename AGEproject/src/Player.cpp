#include "Player.h"
#include "UserControls.h"
#include "BoundingSphere.h"
#include "Game.h"
void Player::Update()
{
	if (!isNPC)
	{
		HandleInput();
	}

	std::vector<Entity*> temp;
	// Check each entity.
	for (Entity*&e : entities)
	{
		// Check if it has a structure, if so collect.
		if (e->GetCompatibleComponent<Structure>() != NULL)
			e->GetComponent<Structure>().Collect(temp);
	}
	for (Entity*&e : temp)
		entities.push_back(e);
}

void Player::HandleInput()
{
	// Handle selection action.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
	{
		if (!glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			selectedEntities.clear();
		}
		// Find if there is an intersection with any entity.
		for (Entity* &e : entities)
		{
			// If a ray intersects with the bounding sphere.
			if (e->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
			{
				selectedEntities.push_back(e);
				return;
			}
		}
		selectedEntities.clear();
	}
	// if it is a move action, move selected entity.
	if (glfwGetMouseButton(GameEngine::Get().GetWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		glm::vec3 poi;
		// Check for point of intersection.
		if (Game::Get().entities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
		{
			// Do not move strucutures.
			for (Entity*&e : selectedEntities)
			{
				if (e != NULL && e->GetCompatibleComponent<AirMovement>() != NULL)
				{
					poi.y = e->GetPosition().y;
					e->GetComponent<AirMovement>().SetDestination(poi);
				}
			}
		}
	}

	//Handle shortkeys for units/structures.
	if(selectedEntities.size() != 0)
	{ 
		Entity* selectedEntity = selectedEntities[0];
		if (selectedEntity != NULL)
		{
			if (selectedEntity->GetCompatibleComponent<Structure>() != NULL)
			{

				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
				{
					selectedEntity->GetCompatibleComponent<Structure>()->AddProduct("TEMP", 2);
				}
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
				{

				}
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
				{

				}
			}
			if (selectedEntity->GetCompatibleComponent<AirMovement>() != NULL)
			{

				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
				{
					selectedEntity->SetScale(glm::vec3(10, 10, 10));
				}
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
				{

				}
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
				{

				}
			}
		}
	}
	glfwPollEvents();



}

void Player::Render()
{
	

}
