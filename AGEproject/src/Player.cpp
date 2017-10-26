#include "Player.h"
#include "UserControls.h"
#include "BoundingSphere.h"
#include "Game.h"
void Player::Update(std::vector<Entity*>& enemyList)
{
	HandleInput(enemyList);
	// Collect any units that have been produced by your structures.
	std::vector<Entity*> temp;
	for (Entity*&e : entities)
	{
		if (e->GetCompatibleComponent<Structure>() != NULL)
			e->GetComponent<Structure>().Collect(temp);
	}
	for (Entity*&e : temp)
		entities.push_back(e);
}

void Player::HandleInput(std::vector<Entity*>& enemyList)
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
		// If no suitable object has been selected, clear selected list.
		selectedEntities.clear();
	}
	// if it is a move action, move selected entity.
	if (glfwGetMouseButton(GameEngine::Get().GetWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		glm::vec3 poi;
		// Check for point of intersection.
		if (Game::Get().entities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
		{
			// Do not move structures.
			for (Entity*&e : selectedEntities)
			{
				if (e != NULL && e->GetCompatibleComponent<Unit>() != NULL)
				{
					// Override the pause status if it persists.
					e->GetCompatibleComponent<Movement>()->SetActive(true);
					poi.y = e->GetPosition().y;
					e->GetCompatibleComponent<Movement>()->SetDestination(poi);
				}
			}
		}

		// Check for targets. - swap order, better worse case.
		for (Entity*& e : enemyList)
		{
			// If clicking on an enemy.
			if (e->GetCompatibleComponent<BoundingSphere>()->TestIntersection(UserControls::Get().GetRay(), poi))
			{
				// Attack the enemy. 
				for (Entity*& m : selectedEntities)
				{
					m->GetCompatibleComponent<Unit>()->SetEntityToTarget(e);
					m->GetCompatibleComponent<Unit>()->SetAction(Unit::Attack);
				}
			}
		}
	}

	//Handle shortkeys for units/structures.
	if(selectedEntities.size() != 0)
	{ 
		Entity*& selectedEntity = selectedEntities[0];
		if (selectedEntity != NULL)
		{
			// Delete all selected entities.
			if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
			{
				selectedEntities.erase(std::remove(selectedEntities.begin(), selectedEntities.end(), selectedEntity), selectedEntities.end());
				entities.erase(std::remove(entities.begin(), entities.end(), selectedEntity), entities.end());
				selectedEntity = NULL;
				return;
			}

			// Pause all movement components of selected units.
			if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_X) == GLFW_PRESS)
			{
				// Pause all units.
				for (Entity*& e : selectedEntities)
				{
					if (e->GetCompatibleComponent<Movement>() != NULL)
					{
						e->GetCompatibleComponent<Movement>()->SetActive( (e->GetCompatibleComponent<Movement>()->IsActive()) ? false: true);
					}
			}


			}

			if (selectedEntity->GetCompatibleComponent<Structure>() != NULL)
			{
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
				{
					// AddProduct with an int instead. This will change what should spawn in the inherited structures.
					selectedEntity->GetCompatibleComponent<Structure>()->AddProduct("Ship", 2);
				}
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
				{

				}
				if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
				{

				}
				
			}
			else if (selectedEntity->GetCompatibleComponent<Unit>() != NULL)
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
