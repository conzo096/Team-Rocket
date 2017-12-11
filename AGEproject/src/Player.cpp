#include "Player.h"
#include "UserControls.h"
#include "BoundingSphere.h"
#include "Game.h"
#include "WorkerUnit.h"
#include "Spawner.h"
#include "Targetable.h"
#include <time.h> 
void Player::Update(std::vector<std::shared_ptr<Entity>>& enemyList)
{
	HandleInput(enemyList);
	// Collect any units that have been produced by your structures.
	std::vector<std::shared_ptr<Entity>> temp;
	for (std::shared_ptr<Entity>&e : entities)
	{
		if (e->GetCompatibleComponent<Structure>() != NULL)
			e->GetCompatibleComponent<Structure>()->Collect(temp);
		if (e->GetCompatibleComponent<Worker>() != NULL)
			balance += e->GetCompatibleComponent<Worker>()->Collect();
	}
	// Push the collect entities back to the player.
	for (std::shared_ptr<Entity>&e : temp)
	{
		if (e->GetCompatibleComponent<Movement>() != NULL)
			e->GetCompatibleComponent<Movement>()->SetGoal(glm::vec3(20, 0, 20));
		entities.push_back(e);
	}

	// If ghost building is active, update location and change boolean.
	if (showGhostBuilding)
	{
		glm::vec3 poi;
		if (!Game::Get().neutralEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
		{
			validSpawn = false;
		}
		else
		{
			ghostBuilding.SetPosition(glm::dvec3(poi.x, 0.0f, poi.z));
			// Check if the position is valid.
			ghostBuilding.Update(0);
			// Check square grid around radius.
			glm::ivec2 sp = glm::ivec2(poi.x, poi.z);
			// Space is valid. Prevent Units walking in this area.
			validSpawn = true;

			if (!Spawner::Get().CheckGameGrid(ghostBuilding.GetComponent<BoundingSphere>()))
				validSpawn = false;
		}
		// Check if the player can afford the building.
		if (buildingCost > balance)
		{
			validSpawn = false;
		}
	}
	lastClock = clock();

}

void Player::HandleInput(std::vector<std::shared_ptr<Entity>>& enemyList)
{
	timeElapsed += (float)(clock() - lastClock)/CLOCKS_PER_SEC;

	glm::vec3 poi;
	// if it is a move action, move selected entity.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Move")) || UserControls::Get().IsJoystickPressed(std::string("A"),UserControls::ControllerAction::BUTTON))
	{
		showGhostBuilding = false;
		// Check for point of intersection.
		if (Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
		{
			// Do not move structures.
			for (std::shared_ptr<Entity>&e : selectedEntities)
			{
				if (e != NULL && e->GetCompatibleComponent<Unit>() != NULL)
				{		
					// Override the pause status if it persists.
					e->GetCompatibleComponent<Movement>()->SetActive(true);
					poi.y = (float)e->GetPosition().y;
					glm::dvec3 t = Game::Get().ObtainNearestValidCoordinate(e->GetPosition(), poi);
					e->GetCompatibleComponent<Movement>()->SetGoal(t);
					e->GetCompatibleComponent<Unit>()->SetAction(Unit::Move);
				}
			}
			// Particle that appears when the user selects a location.
			glm::vec3 t = poi;
			t.y = 0;
			Game::Get().location = t;
			Game::Get().duration = 3.0;
		}

		// Check for targets. - swap order, better worse case.
		for (auto& e : enemyList)
		{
			// If clicking on an enemy.
			if (e->GetCompatibleComponent<BoundingSphere>() != NULL)
			{
				if (e->GetCompatibleComponent<BoundingSphere>()->TestIntersection(UserControls::Get().GetRay(), poi))
				{
					// Attack the enemy. 
					for (std::shared_ptr<Entity>& m : selectedEntities)
					{
						if (m->GetCompatibleComponent<Unit>() != NULL)
						{
							m->GetCompatibleComponent<Unit>()->SetEntityToTarget(e);
							m->GetCompatibleComponent<Unit>()->SetAction(Unit::Attack);
							// If selected unit is a resource and target is resource, harvest instead.
							if (m->GetName() == "Worker" && e->GetName() == "Resource")
								m->GetCompatibleComponent<Unit>()->SetAction(Unit::Harvest);
						}
					}
				}
			}
		}
	}

	// Select unit or units.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Action")) || UserControls::Get().IsJoystickPressed(std::string("X"), UserControls::ControllerAction::BUTTON))
	{
		selectedFriendly = NULL;
		selectedEnemy = NULL;
		// If you are to spawn a building. Create it.
		if (selectedEntities.size() >0)
			if (showGhostBuilding)
			{
				if (Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))// && validSpawn)
				{
					poi.y = 0;
					// Set spawnLocation.
					selectedEntities[0]->GetCompatibleComponent<Structure>()->SetSpawnPoint(poi);
					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, buildingType);
					Spawner::Get().UpdateGameGrid(ghostBuilding.GetComponent<BoundingSphere>(), 1);
					showGhostBuilding = false;
				}
			}

		// If only wanting one entity, remove everything from the current list.
		if (!glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !UserControls::Get().IsJoystickPressed(std::string("leftShoulder"), UserControls::ControllerAction::BUTTON))
		{
			for (std::shared_ptr<Entity> &e : selectedEntities)
			{	
				e->GetComponent<Targetable>().IsSelected(false);
			}
			selectedEntities.clear();
		}
		

		bool objectSelected = false;
		// Now iterate through player units and check if mouse ray intersects with their bounding sphere.
		for (std::shared_ptr<Entity> &e : entities)
		{
			// If a ray intersects with the bounding sphere.
			if (e->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
			{
				e->GetComponent<Targetable>().IsSelected(true);
				selectedEntities.push_back(e);
				return;
			}
		}
		// Display enemy info.
		for (std::shared_ptr<Entity> &e : enemyList)
		{
			// If a ray intersects with the bounding sphere.
			if(e->GetCompatibleComponent<BoundingSphere>() != NULL)
				if (e->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
				{
					selectedEnemy = e;
				}
		}
		if (!objectSelected)
		{
			// If no suitable object has been selected, clear selected list.
			for (std::shared_ptr<Entity>&e : selectedEntities)
			{
				e->GetComponent<Targetable>().IsSelected(false);
			}
			selectedEntities.clear();
		}
	}
	//Handle shortkeys for units/structures.
	if(selectedEntities.size() > 0)
	{ 
		std::shared_ptr<Entity>& selectedEntity = selectedEntities[0];
		selectedFriendly = selectedEntities[0];
		if (selectedEntity != NULL)
		{
			// Delete all selected entities.
			if (UserControls::Get().IsKeyPressed(std::string("HotKey4")) || UserControls::Get().IsJoystickPressed(std::string("dLeft"), UserControls::ControllerAction::BUTTON))
			{
				for (auto& ent : selectedEntities)
					ent->GetComponent<Targetable>().SetHealth(0);
				selectedEntity = NULL;
				selectedEntities.clear();
				return;
			}
			// Pause all movement components of selected units.
			if (UserControls::Get().IsKeyPressed(std::string("Hold")) || UserControls::Get().IsJoystickPressed(std::string("Y"), UserControls::ControllerAction::BUTTON))
			{
				// Pause all units.
				for (std::shared_ptr<Entity>& e : selectedEntities)
				{
					if (e->GetCompatibleComponent<Movement>() != NULL)
					{
						e->GetCompatibleComponent<Movement>()->SetActive((e->GetCompatibleComponent<Movement>()->IsActive()) ? false : true);
					}
				}
			}
		}


		// Handle actions that the structures and units can undertake.
		if (selectedEntity->GetCompatibleComponent<Structure>() != NULL && timeElapsed >= 0.5f)
		{
			if (UserControls::Get().IsKeyPressed(std::string("HotKey1")) || UserControls::Get().IsJoystickPressed(std::string("dUp")))
			{
				if (selectedEntity->GetName() == "Worker")
				{
					showGhostBuilding = true;
					UpdateGhostBuilding(0);
					buildingType = 0;
				}
				else
				{
					// This is spawner near the object towards the center - change to towards door.
					glm::vec3 spawnLoc;
					spawnLoc.y = 0.0f;
					spawnLoc.x = static_cast<float>(selectedEntities[0]->GetPosition().x + 5.0f);
					spawnLoc.y = static_cast<float>(selectedEntities[0]->GetPosition().y + 5.0f);

					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);

				}
				timeElapsed = 0;
			}
			else if (UserControls::Get().IsKeyPressed(std::string("HotKey2")) || UserControls::Get().IsJoystickPressed(std::string("dRight")))
			{
				if (selectedEntity->GetName() == "Worker")
				{
					showGhostBuilding = true;
					UpdateGhostBuilding(1);
					buildingType = 1;
				}
				else
				{
			/*		glm::vec3 spawnLoc;
					spawnLoc.y = 0.0f;
					spawnLoc.x = static_cast<float>(selectedEntities[0]->GetPosition().x + 5.0f);
					spawnLoc.y = static_cast<float>(selectedEntities[0]->GetPosition().y + 5.0f);

					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 1);*/
					if (selectedEntities[0]->GetCompatibleComponent<Structure>()->BuyRankUpdate(balance))
						std::cout << "Success" << std::endl;

				}
				timeElapsed = 0;
			}
			else if (UserControls::Get().IsKeyPressed(std::string("HotKey3")) || UserControls::Get().IsJoystickPressed(std::string("dDown")))
			{
				if (selectedEntity->GetName() == "Worker")
				{
					showGhostBuilding = true;
					UpdateGhostBuilding(2);
					buildingType = 2;
				}
				else
				{
					glm::vec3 spawnLoc;
					spawnLoc.y = 0.0f;
					spawnLoc.x = selectedEntities[0]->GetPosition().x + 5.0f;
					spawnLoc.y = selectedEntities[0]->GetPosition().y + 5.0f;

					selectedEntities[0]->GetCompatibleComponent<Structure>()->AddProduct(balance, 2);
				}
				timeElapsed = 0;
			}
		}
		else if (selectedEntity->GetCompatibleComponent<Unit>() != NULL && timeElapsed >= 0.5f)
		{

			if (UserControls::Get().IsKeyPressed(std::string("HotKey1")))
			{
				timeElapsed = 0;
			}
			if (UserControls::Get().IsKeyPressed(std::string("HotKey2")))
			{
				timeElapsed = 0;
			}
			if (UserControls::Get().IsKeyPressed(std::string("HotKey3")))
			{
				timeElapsed = 0;
			}
		}
	}


	// Need to handle.
	// - Selection	x
	// - Deselection 
	// - Movement x
	// - Targeting x
	// Keyboard/controller commands (Such as spawning units).


	//// Handle all possible action commands.
	//if (UserControls::Get().IsMouseButtonPressed(std::string("Action")) || UserControls::Get().IsJoystickPressed(std::string("A"), UserControls::ControllerAction::BUTTON))
	//{
	//	//if(!UserControls::Get().IsKeyPressed(std::string("Hold")))
	//	//	selectedEntities.clear();
	//	// Select all entities that are over the mouse.
	//	for (std::shared_ptr<Entity>& ent : entities)
	//	{
	//		if (std::find(selectedEntities.begin(), selectedEntities.end(), ent) == selectedEntities.end())
	//			if (ent->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
	//			{
	//				selectedEntities.push_back(ent);
	//				selectedFriendly = ent;
	//			}
	//	}

	//	// If there is a ghost building get the first worker to build it.
	//	if (validSpawn && showGhostBuilding)
	//	{
	//		for (std::shared_ptr<Entity>& ent : selectedEntities)
	//		{
	//			if (ent->GetName() == "Worker")
	//			{
	//				ent->GetComponent<Structure>().SetSpawnPoint(ghostBuilding.GetPosition());
	//				ent->GetComponent<Structure>().AddProduct(balance, buildingType);
	//				ent->GetCompatibleComponent<Unit>()->SetAction(Unit::Build);
	//				showGhostBuilding = false;
	//				break;
	//			}
	//		}
	//	}

	//	// Now check all the entities - Only want the first one there.
	//	for (std::shared_ptr<Entity>& ent : enemyList)
	//	{
	//		if (ent->GetCompatibleComponent<BoundingSphere>() != NULL)
	//		{
	//			if (ent->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
	//			{
	//				selectedEnemy = ent;
	//				break;
	//			}
	//		}
	//	}
	//}
	//// Handles moving / attacking.
	//if (UserControls::Get().IsMouseButtonPressed(std::string("Move")) || UserControls::Get().IsJoystickPressed(std::string("X"), UserControls::ControllerAction::BUTTON))
	//{
	//	selectedEnemy = NULL;
	//	// Find point of intersection with the game plane.
	//	if (Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
	//	{
	//		showGhostBuilding = false;
	//		// Check for point of intersection.
	//		if (Game::Get().allEntities[1]->GetComponent<BoundingBox>().CheckForMouseIntersection(UserControls::Get().GetRay(), poi))
	//		{
	//			// Do not move structures.
	//			for (std::shared_ptr<Entity>&e : selectedEntities)
	//			{
	//				if (e != NULL && e->GetCompatibleComponent<Unit>() != NULL)
	//				{
	//					// Override the pause status if it persists.
	//					e->GetCompatibleComponent<Movement>()->SetActive(true);
	//					poi.y = (float)e->GetPosition().y;
	//					e->GetCompatibleComponent<Movement>()->SetGoal(poi);
	//					e->GetCompatibleComponent<Unit>()->SetAction(Unit::Move);
	//				}
	//			}
	//			// Particle that appears when the user selects a location.
	//			glm::vec3 t = poi;
	//			t.y = 0;
	//			Game::Get().location = t;
	//			Game::Get().duration = 3.0;
	//		}
	//		// If you are selected on an enemy attack it instead.
	//		for (std::shared_ptr<Entity>&e : selectedEntities)
	//		{
	//			for (std::shared_ptr<Entity>&f : enemyList)
	//			{
	//				if (f->GetCompatibleComponent<BoundingSphere>() != NULL)
	//				{
	//					if (f->GetComponent<BoundingSphere>().TestIntersection(UserControls::Get().GetRay()))
	//					{
	//						if (e != NULL && e->GetCompatibleComponent<Unit>() != NULL && f != NULL && f->GetCompatibleComponent<Targetable>() != NULL)
	//						{
	//							// The selected entities attack this instead.
	//							e->GetCompatibleComponent<Unit>()->SetEntityToTarget(f);
	//							e->GetCompatibleComponent<Unit>()->SetAction(Unit::Attack);
	//							if (e->GetName() == "Worker" && f->GetName() == "Resource")
	//								e->GetCompatibleComponent<Unit>()->SetAction(Unit::Harvest);
	//						}
	//					}
	//				}
	//			}
	//		
	//		}
	//	}
	//}
	//// Handles Hotkeys of structures and units.
	//if (selectedEntities.size() > 0)
	//{
	//	if (UserControls::Get().IsKeyPressed(std::string("HotKey1")) && timeElapsed >= 0.5f)
	//	{
	//		for (int i = 0; i < selectedEntities.size(); i++)
	//		{
	//			timeElapsed = 0;
	//			if (selectedEntities[i]->GetCompatibleComponent<Structure>() != NULL)
	//			{
	//				if (selectedEntities[i]->GetName() == "Worker")
	//				{
	//					showGhostBuilding = true;
	//					UpdateGhostBuilding(0);
	//					buildingType = 0;
	//				}
	//				else
	//				{
	//					// This is spawner near the object towards the center - change to towards door.
	//					glm::vec3 spawnLoc;
	//					spawnLoc.y = 0.0f;
	//					spawnLoc.x = static_cast<float>(selectedEntities[i]->GetPosition().x + 5.0f);
	//					spawnLoc.y = static_cast<float>(selectedEntities[i]->GetPosition().y + 5.0f);

	//					selectedEntities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 0);
	//				}
	//			}
	//		}
	//	}
	//	if (UserControls::Get().IsKeyPressed(std::string("HotKey2")) && timeElapsed >= 0.5f)
	//	{
	//		timeElapsed = 0;
	//		for (int i = 0; i < selectedEntities.size(); i++)
	//		{
	//			if (selectedEntities[i]->GetCompatibleComponent<Structure>() != NULL)
	//			{
	//				if (selectedEntities[i]->GetName() == "Worker")
	//				{
	//					showGhostBuilding = true;
	//					UpdateGhostBuilding(1);
	//					buildingType = 1;
	//				}
	//				else
	//				{
	//					// This is spawner near the object towards the center - change to towards door.
	//					glm::vec3 spawnLoc;
	//					spawnLoc.y = 0.0f;
	//					spawnLoc.x = static_cast<float>(selectedEntities[i]->GetPosition().x + 5.0f);
	//					spawnLoc.y = static_cast<float>(selectedEntities[i]->GetPosition().y + 5.0f);

	//					selectedEntities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 1);
	//				}
	//			}
	//		}
	//	}
	//	if (UserControls::Get().IsKeyPressed(std::string("HotKey3")) && timeElapsed >= 0.5f)
	//	{
	//		for (int i = 0; i < selectedEntities.size(); i++)
	//		{
	//			timeElapsed = 0;
	//			if (selectedEntities[i]->GetCompatibleComponent<Structure>() != NULL)
	//			{
	//				if (selectedEntities[i]->GetName() == "Worker")
	//				{
	//					showGhostBuilding = true;
	//					UpdateGhostBuilding(2);
	//					buildingType = 2;
	//				}
	//				else
	//				{
	//					// This is spawner near the object towards the center - change to towards door.
	//					glm::vec3 spawnLoc;
	//					spawnLoc.y = 0.0f;
	//					spawnLoc.x = static_cast<float>(selectedEntities[i]->GetPosition().x + 5.0f);
	//					spawnLoc.y = static_cast<float>(selectedEntities[i]->GetPosition().y + 5.0f);

	//					selectedEntities[i]->GetCompatibleComponent<Structure>()->AddProduct(balance, 2);
	//				}
	//			}
	//		}
	//	}
	//}
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
		for (int i = 0; i < selectedEntities.size(); i++)
		{
			if (selectedEntities[i]->GetName() == "Worker")
			{
				buildingCost = selectedEntities[i]->GetComponent<Structure>().GetSpawnInfo()[0].cost;
				break;
			}
		}
	}
	else if (type == 1)
	{
		ghostBuilding.GetComponent<Renderable>().SetModel("VehicleBay");
		for (int i = 0; i < selectedEntities.size(); i++)
		{
			if (selectedEntities[i]->GetName() == "Worker")
			{
				buildingCost = selectedEntities[i]->GetComponent<Structure>().GetSpawnInfo()[1].cost;
				break;
			}
		}
	}
	else if (type == 2)
	{
		ghostBuilding.GetComponent<Renderable>().SetModel("Hanger");
		for (int i = 0; i < selectedEntities.size(); i++)
		{
			if (selectedEntities[i]->GetName() == "Worker")
			{
				buildingCost = selectedEntities[i]->GetComponent<Structure>().GetSpawnInfo()[2].cost;
				break;
			}
		}
	}
	ghostBuilding.GetComponent<Renderable>().SetPosition(vec3(0, -ghostBuilding.GetComponent<Renderable>().GetModel().GetLowestYPosition(), 0));
	ghostBuilding.GetComponent<BoundingSphere>().SetUpBoundingSphere(ghostBuilding.GetComponent<Renderable>().GetModel().GetVertexPositions());
}


void Player::SortEntities(Free_Camera& camera)
{

}
void Player::SortEntities(Game_Camera& camera)
{
	const glm::dvec3 camPos = camera.GetPosition();
	std::sort(entities.begin(), entities.end(), [camPos](const std::shared_ptr<Entity>& lhs, const std::shared_ptr<Entity>& rhs)
	{
		return	glm::distance(lhs->GetPosition(), camPos) < glm::distance(rhs->GetPosition(), camPos);
	});
	std::cout << entities[0]->GetName() << std::endl;


}