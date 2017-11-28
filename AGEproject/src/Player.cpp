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


	// If ghost building is active, update location and change boolean.
	if (showGhostBuilding)
	{
		glm::vec3 poi;
		if (!Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
		{
			validSpawn = false;

		}
		else
		{
			ghostBuilding.SetPosition(glm::dvec3(poi.x, 2.5f, poi.z));
			// Check if the position is valid.
			ghostBuilding.Update(0);
			// Check square grid around radius.
			glm::ivec2 sp = glm::ivec2(poi.x, poi.z);
			// Space is valid. Prevent Units walking in this area.
			validSpawn = true;
			// Check by row.  
			for (int i = -ghostBuilding.GetComponent<BoundingSphere>().GetRadius(); i < ghostBuilding.GetComponent<BoundingSphere>().GetRadius(); i++)
			{
				// check by depth.
				for (int j = -ghostBuilding.GetComponent<BoundingSphere>().GetRadius(); j < ghostBuilding.GetComponent<BoundingSphere>().GetRadius(); j++)
				{
					// Get Point to check.
					glm::ivec2 p = sp + glm::ivec2(i, j);
					if (p.x < 0 || p.y < 0 || p.x > 99 || p.y > 99)
					{
						validSpawn = false;
						return;
					}
					// Check nav mesh.
					if (Game::Get().GetNavGridValue(p) == 1)
					{
						validSpawn = false;
						return;
					}
				}
			}

		

		}
	}

}

void Player::HandleInput(std::vector<Entity*>& enemyList)
{
	updateCalled++;

	glm::vec3 poi;
	// if it is a move action, move selected entity.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Move")))
	{
		showGhostBuilding = false;
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
					e->GetCompatibleComponent<Movement>()->SetGoal(poi);
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

	// Select unit or units.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
	{
		// If you are to spawn a building. Create it.
		if (selectedEntities.size() >0)
			if (showGhostBuilding)
			{
				if (Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi) && validSpawn)
				{
					poi.y = 2.5f;
					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, buildingType, poi);
					//validSpawn = false;
					showGhostBuilding = false;
				}
			}


		// If only wanting one entity, remove everything from the current list.
		if (!glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			for (Entity* &e : selectedEntities)
			{
				if (e->GetCompatibleComponent<Unit>() != NULL)
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
				//selectedEntity->GetCompatibleComponent<Structure>()->AddProduct(balance,0);
				if (selectedEntity->GetName() == "Worker")
				{
					showGhostBuilding = true;
					UpdateGhostBuilding(0);
					buildingType = 0;
				}
				else
					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0, glm::vec3(20, 2.5, 0));
			}
			else if (UserControls::Get().IsKeyPressed(std::string("HotKey2")))
			{
				if (selectedEntity->GetName() == "Worker")
				{
					showGhostBuilding = true;
					UpdateGhostBuilding(1);
					buildingType = 1;
				}
				else
					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 1, glm::vec3(20, 2.5, 0));
			}
			else if (UserControls::Get().IsKeyPressed(std::string("HotKey3")))
			{
				//selectedEntity->GetCompatibleComponent<Structure>()->AddProduct(balance, 2);
				if (selectedEntity->GetName() == "Worker")
				{
					showGhostBuilding = true;
					UpdateGhostBuilding(2);
					buildingType = 2;
				}
				else
					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 2,glm::vec3(20,2.5,0));
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

// Render ghost block if there is one.
void Player::Render()
{
	if (showGhostBuilding)
	{
		if (validSpawn)
			ghostBuilding.GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(0, 1, 0, 1);
		else
			ghostBuilding.GetComponent<Renderable>().GetMaterial().emissive = glm::vec4(1, 0, 0, 1);

		ghostBuilding.Render();
	}
}


// Update ghost block visual.
void Player:: UpdateGhostBuilding(int type)
{
	if (ghostBuilding.GetCompatibleComponent<Renderable>() == NULL)
	{
		auto renderable = std::make_unique<Renderable>();
		renderable->SetShader("Phong");
		renderable->SetTexture("debug");
		Material* mat = new Material();
		mat->emissive.a = 0;
		mat->diffuse.a = 0;
		mat->specular.a = 0;
		renderable->SetMaterial(mat);

		//renderable->SetMaterial(new Material());
		//renderable->GetModel().SetType(GL_LINE_STRIP);
		ghostBuilding.AddComponent(move(renderable));
		auto sphere = std::make_unique<BoundingSphere>();
		ghostBuilding.AddComponent(move(sphere));



	}

	if (type == 0)
	{
		ghostBuilding.GetComponent<Renderable>().SetModel("Factory");
	}
	else if (type == 1)
	{
		ghostBuilding.GetComponent<Renderable>().SetModel("VehicleBay");
	}
	else if (type == 2)
	{
		ghostBuilding.GetComponent<Renderable>().SetModel("Hanger");
	}
	ghostBuilding.GetComponent<BoundingSphere>().SetUpBoundingSphere(ghostBuilding.GetComponent<Renderable>().GetModel().GetVertexPositions());
}