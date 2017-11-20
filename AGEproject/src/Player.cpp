#include "Player.h"
#include "UserControls.h"
#include "BoundingSphere.h"
#include "Game.h"
#include "WorkerUnit.h"
void Player::Update(std::vector<Entity*>& enemyList)
{
	HandleInput(enemyList);
	// Collect any units that have been produced by your structures.
	std::vector<Entity*> temp;
	for (Entity*&e : entities)
	{
		if (e->GetCompatibleComponent<Structure>() != NULL)
			e->GetCompatibleComponent<Structure>()->Collect(temp);
		if (e->GetCompatibleComponent<Worker>() != NULL)
			balance += e->GetCompatibleComponent<Worker>()->Collect();
	}
	// Push the collect entities back to the player.
	for (Entity*&e : temp)
		entities.push_back(e);
}

void Player::HandleInput(std::vector<Entity*>& enemyList)
{
	updateCalled++;

	// Select unit or units.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
	{
		// If only wanting one entity, remove everything from the current list.
		if (!glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			for (Entity* &e : selectedEntities)
			{
				if(e->GetCompatibleComponent<Unit>()!=NULL)
					e->GetCompatibleComponent<Unit>()->IsController(false);
				if (e->GetCompatibleComponent<Structure>() != NULL)
					e->GetCompatibleComponent<Structure>()->IsController(false);
			}
			selectedEntities.clear();
		}

		bool objectSelected = false;
		// Now iterate through player units and check if mouse ray intersects with their bounding sphere.
		for (Entity* &e : entities)
		{
			// If a ray intersects with the bounding sphere.
			if (e->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
			{
				if (e->GetCompatibleComponent<Unit>() != NULL)
					e->GetCompatibleComponent<Unit>()->IsController(true);
				if (e->GetCompatibleComponent<Structure>() != NULL)
					e->GetCompatibleComponent<Structure>()->IsController(true);
				selectedEntities.push_back(e);
				return;
			}
		}
		if (!objectSelected)
		{
			// If no suitable object has been selected, clear selected list.
			for (Entity* &e : selectedEntities)
			{
				if (e->GetCompatibleComponent<Unit>() != NULL)
					e->GetCompatibleComponent<Unit>()->IsController(false);
				if (e->GetCompatibleComponent<Structure>() != NULL)
					e->GetCompatibleComponent<Structure>()->IsController(false);
			}
			selectedEntities.clear();
		}
	}
	
	
	
	// if it is a move action, move selected entity.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Move")))
	{
		glm::vec3 poi;
		// Check for point of intersection.
		if (Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
		{
			// Do not move structures.
			for (Entity*&e : selectedEntities)
			{
				if (e != NULL && e->GetCompatibleComponent<Unit>() != NULL)
				{		
					// Override the pause status if it persists.
					e->GetCompatibleComponent<Movement>()->SetActive(true);
					poi.y = (float)e->GetPosition().y;
					e->GetCompatibleComponent<Movement>()->SetDestination(poi);
					e->GetCompatibleComponent<Unit>()->SetAction(Unit::Move);
					// Particle that appears when the user selects a location.
					Game::Get().location = poi;
					Game::Get().duration = 3.0;
				}
			}
		}

		// Check for targets. - swap order, better worse case.
		for (Entity*& e : enemyList)
		{
			// If clicking on an enemy.
			if (e->GetCompatibleComponent<BoundingSphere>() != NULL)
			{
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
			if (UserControls::Get().IsKeyPressed(std::string("Hold")))
			{
				// Pause all units.
				for (Entity*& e : selectedEntities)
				{
					if (e->GetCompatibleComponent<Movement>() != NULL)
					{
						e->GetCompatibleComponent<Movement>()->SetActive((e->GetCompatibleComponent<Movement>()->IsActive()) ? false : true);
					}
				}
			}
		}

		if (selectedEntity->GetCompatibleComponent<Structure>() != NULL && updateCalled >= 4)
		{

			if (UserControls::Get().IsKeyPressed(std::string("HotKey1")))
			{
				selectedEntity->GetCompatibleComponent<Structure>()->AddProduct(balance,0);
			}
			if (UserControls::Get().IsKeyPressed(std::string("HotKey2")))
			{
				selectedEntity->GetCompatibleComponent<Structure>()->AddProduct(balance, 1);
			}
			if (UserControls::Get().IsKeyPressed(std::string("HotKey3")))
			{
				selectedEntity->GetCompatibleComponent<Structure>()->AddProduct(balance, 2);
			}
			updateCalled = 0;
		}
		else if (selectedEntity->GetCompatibleComponent<Unit>() != NULL && updateCalled >= 4)
		{

			if (UserControls::Get().IsKeyPressed(std::string("HotKey1")))
			{
				selectedEntity->SetScale(glm::vec3(10, 10, 10));
			}
			if (UserControls::Get().IsKeyPressed(std::string("HotKey2")))
			{

			}
			if (UserControls::Get().IsKeyPressed(std::string("HotKey3")))
			{

			}
			updateCalled = 0;
		}
	}
}

void Player::Render()
{
	

}
