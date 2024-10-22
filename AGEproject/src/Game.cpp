#include "Game.h"
#include "Entity.h"
#include "PointLight.h"
#include "AiPlayer.h"
#include "Spawner.h"
#include "UserControls.h"
#include <thread>
#include <omp.h>
#include "LevelLoader.h"
#include "StateManager.h"
void UpdateEntityList(int start, int end, double deltaTime, std::vector<Entity*>& entities)
{
	for (int i = start; i < end; i++)
	{
		Entity*& e = entities[i];
		e->Update(deltaTime);
	}
}

std::vector<std::shared_ptr<Entity>> Game::FindResources()
{
	vector<std::shared_ptr<Entity>> resources;

	for (vector<std::shared_ptr<Entity>>::size_type n = 0; n < neutralEntities.size();)
	{
		std::shared_ptr<Entity>& e = neutralEntities[n];
		if (e->GetCompatibleComponent<Resource>() != NULL)
		{
			resources.push_back(e);
		}
		n++;
	}

	return resources;
}

std::vector<std::shared_ptr<Entity>> Game::FindLocalUnits(int team, dvec3 position, double sightRange)
{
	vector<std::shared_ptr<Entity>> localUnits;

	if (team == 1)
	{
		for (vector<std::shared_ptr<Entity>>::size_type n = 0; n < player->GetEntities().size();)
		{
			std::shared_ptr<Entity>& e = player->GetEntities()[n];
			if (e->GetCompatibleComponent<Targetable>() != NULL)
			{
				dvec3 pos = dvec3(position.x, 0, position.z);
				dvec3 ePos = dvec3(e->GetPosition().x, 0, e->GetPosition().z);
				if (distance(pos, ePos) <= sightRange)
				{
					localUnits.push_back(e);
				}
			}
			n++;
		}
	}

	if (team == 0)
	{
		for (std::vector<std::shared_ptr<Entity>>::size_type n = 0; n < NPC->GetEntities().size();)
		{
			std::shared_ptr<Entity>& e = NPC->GetEntities()[n];
			if (e->GetCompatibleComponent<Targetable>() != NULL)
			{
				dvec3 pos = dvec3(position.x, 0, position.z);
				dvec3 ePos = dvec3(e->GetPosition().x, 0, e->GetPosition().z);
				if (distance(pos, ePos) <= sightRange)
				{
					localUnits.push_back(e);
				}
			}
			n++;
		}
	}
	return localUnits;
}

vec3 Game::ObtainNearestValidCoordinate(glm::vec3 start, glm::vec3 end)
{
	glm::vec3 point;
	// Check if end point is valid.
	if (navGrid[(int)end.x][(int)end.z] == 0)
	{
		return end;
	}
	else
	{
		// Check in straight line from b until point is found
		glm::vec3 dir = glm::normalize(start - end);
		// Check for so many iterations.
		for (float i = 0; i < GetGridSize(); i++)
		{
			glm::vec3 tPoint = end + (dir*i);
			if (navGrid[(int)tPoint.x][(int)tPoint.z] == 0)
			{
				return tPoint;
			}
		}
	}
	return point;
}

void Game::CheckForWinner(std::vector<std::shared_ptr<Entity>>& entities)
{
	bool enemyBaseFound = false;
	bool playerBaseFound = false;
	for (std::shared_ptr<Entity>& e : entities)
	{
		if (e->GetName() == "Base")
		{
			if (e->GetCompatibleComponent<Structure>() != NULL)
			{
				if (e->GetCompatibleComponent<Structure>()->GetTeam() == Team::ai)
					enemyBaseFound = true;
				if (e->GetCompatibleComponent<Structure>()->GetTeam() == Team::player)
					playerBaseFound = true;
			}
		}
	}

	if (playerBaseFound && !enemyBaseFound)
	{
		gameOver = true;
		winner = Team::player;
		return;
	}
	if (!playerBaseFound && enemyBaseFound)
	{
		gameOver = true;
		winner = Team::ai;
		return;
	}
	if (!playerBaseFound && !enemyBaseFound)
	{
		gameOver = true;
		winner = Team::neutral;
		return;
	}
	return;
}

void ResolveCollisions(std::vector<std::shared_ptr<Entity>>& ents)
{
	for (int i = 0; i < ents.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (i != j)
			{
				// If both objects have a bounding sphere.
				if (ents[i]->GetCompatibleComponent<BoundingSphere>() != NULL && ents[j]->GetCompatibleComponent<BoundingSphere>() != NULL)
				{
					// Calculate if they are colliding.
					if (ents[i]->GetComponent<BoundingSphere>().DetectSphereSphereIntersection(ents[j]->GetComponent<BoundingSphere>()))
					{
						// Get distance between both objects.
						float x = glm::distance(ents[i]->GetPosition(), ents[j]->GetPosition());
						// find out sum radius.
						float sumR = ents[i]->GetComponent<BoundingSphere>().GetRadius() + ents[j]->GetComponent<BoundingSphere>().GetRadius();
						// Calculate how far they need to be moved back.
						double dist = sumR - x;
						// Get direction to move.
						glm::dvec3 dir = ents[i]->GetPosition() - ents[j]->GetPosition();
						dir = glm::normalize(dir);
						// Now if both are units move back equally.
						if (ents[i]->GetCompatibleComponent<Unit>() != NULL && ents[j]->GetCompatibleComponent<Unit>() != NULL)
						{
							ents[i]->SetPosition(ents[i]->GetPosition() + (dir * (dist / 2)));
							ents[j]->SetPosition(ents[j]->GetPosition() + (dir * (dist / 2)));
							ents[i]->UpdateTransforms();
						}
						// If one is a structure, fully move the other.
						else if (ents[i]->GetCompatibleComponent<Unit>() != NULL && ents[j]->GetCompatibleComponent<Structure>() != NULL)
						{
							//					ents[i]->Move(dir * (dist));
							//					ents[i]->UpdateTransforms();
						}
						else if (ents[j]->GetCompatibleComponent<Unit>() != NULL && ents[i]->GetCompatibleComponent<Structure>() != NULL)
						{
							//					ents[j]->Move(-dir * (dist));
							//					ents[j]->UpdateTransforms();
						}
					}
				}
			}
		}
	}
}


void Game::Initialise()
{
	navGrid = new int*[gridSize];
	for (int i = 0; i < gridSize; i++)
		navGrid[i] = new int[gridSize];
	for (int i = 0; i < gridSize; i++)
		for (int j = 0; j < gridSize; j++)
		{
			navGrid[i][j] = 0;
		}
	terrainGrid = new dvec3*[gridSize];
	for (int i = 0; i < gridSize; i++)
	{
		terrainGrid[i] = new dvec3[gridSize];
		for (int j = 0; j < gridSize; j++)
		{
			terrainGrid[i][j] = vec3(i, 0, j);
		}
	}
	player = new Player;
	NPC = new AiPlayer;
	player->SetTeam(Team::player);
	NPC->SetTeam(Team::ai);

	game_cam = new Entity;
	auto cam1 = std::make_unique<Game_Camera>();
	cam1->Rotate( (pi<float>() / 4.0f) + half_pi<float>());
	cam1->SetPosition(glm::dvec3(0.0, 60.0, 0.0));
	cam1->SetProjection(glm::half_pi<float>(), (float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	game_cam->AddComponent(move(cam1));

	free_cam = new Entity;
	auto cam2 = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam2->Rotate(0.0f, -3.0f / pi<float>());
	cam2->SetPosition(glm::dvec3(40.0, 40.0, 70.0));
	cam2->SetProjection((float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	free_cam->AddComponent(move(cam2));
	std::cout << "Cameras initialised" << std::endl;
	freeCamEnabled = false;
	keyHeld = false;

	// Add point light to the scene
	//LevelLoader ll;
	//ll.LoadLevel("./json/LevelSaved.json", player->GetEntities(), NPC->GetEntities(), neutralEntities, player);

	std::shared_ptr<Entity> tempEntity3 = std::make_shared<Entity>();
	auto tempLightComponent = new PointLight(); //std::make_unique<PointLight>();
	tempLightComponent->SetProperties("./json/PointLight.json");
	//tempEntity3->AddComponent(move(tempLightComponent));
	std::cout << tempLightComponent->ambient.r << " " << tempLightComponent->ambient.g << " " << tempLightComponent->ambient.b << " " << tempLightComponent->ambient.a << std::endl;
	GameEngine::Get().AddPointLight(tempLightComponent);
	neutralEntities.push_back(tempEntity3);
	std::cout << "Light initialised" << std::endl;
	// This is the floor.
	std::shared_ptr<Entity> tempEntity2 = std::make_shared<Entity>();
	tempEntity2->SetPosition(glm::vec3(0, 0, 0));
	tempEntity2->UpdateTransforms();
	auto tempRenderable2 = std::make_unique<Renderable>();
	tempRenderable2->SetMaterial(new Material());
	//tempRenderable2->SetPlane(1, 100, 100);
	tempRenderable2->SetProperties("./json/Plane.json");
	tempEntity2->SetPosition(tempRenderable2->GetPosition());
	tempRenderable2->UpdateTransforms();
	auto tempBoundingBox2 = std::make_unique<BoundingBox>();
	tempBoundingBox2->SetUpBoundingBox(tempRenderable2->GetModel().GetVertexPositions());
	// Set custom material.
	Material* mat = new Material();
	mat->emissive = glm::vec4(0.02, 0.02, 0.02, 1);
	tempRenderable2->SetMaterial(mat);
	tempEntity2->AddComponent(move(tempRenderable2));
	tempEntity2->AddComponent(move(tempBoundingBox2));
	neutralEntities.push_back(tempEntity2);


	// Add starting structures. - This is the same for each NEW game. Maybe they can have random starting positions? - Then resources need to be worried about.
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Base", glm::vec3(50.5, 0, 50.5), player->GetTeam()));
	Spawner::Get().UpdateGameGrid(player->GetEntities()[0]->GetComponent<BoundingSphere>(), 1);
	NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Base", glm::vec3(249.5, 0, 249.5), NPC->GetTeam()));
	Spawner::Get().UpdateGameGrid(NPC->GetEntities()[0]->GetComponent<BoundingSphere>(), 1);
	for (int i = 0; i < 10; i++)
	{
		neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(10+(i*2), 0, 30 + (i * -2)), Team::neutral));
		neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(140 + (i * 2), 0, 160 + (i * -2)), Team::neutral));
		neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(270 + (i * 2), 0, 290 + (i * -2)), Team::neutral));
	}

	lastTime = clock();
}


bool Game::Update()
{
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	time += deltaTime;
	lastTime = clock();

	if (!gameOver)
	{
		// Populate the all entity list.
		allEntities.clear();
		//allEntities.resize(player->GetEntities().size() + NPC->GetEntities().size() + neutralEntities.size());

		allEntities.insert(allEntities.end(), neutralEntities.begin(), neutralEntities.end());
		//allEntities.insert(allEntities.end(), player->GetEntities().begin(), player->GetEntities().end());
		allEntities.insert(allEntities.end(), NPC->GetEntities().begin(), NPC->GetEntities().end());
		// all entities now contains all the entities in the game.

		// Let user and update their actions. 
		player->Update(allEntities);

		allEntities.clear();
		allEntities.insert(allEntities.end(), neutralEntities.begin(), neutralEntities.end());
		allEntities.insert(allEntities.end(), player->GetEntities().begin(), player->GetEntities().end());
		NPC->Update(allEntities);

		if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
		{
			LevelLoader ll;
			ll.SaveLevel("./json/LevelSaved.json", player->GetEntities(), NPC->GetEntities(), neutralEntities, player->GetBalance());
			StateManager::Get().currentState = StateManager::statePause;
		}

		if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_L) == GLFW_PRESS)
		{
			LevelLoader ll;
			ll.LoadLevel("./json/LevelSaved.json", player->GetEntities(), NPC->GetEntities(), neutralEntities, player);
		}
		if (glfwGetKey(GameEngine::Get().GetWindow(), GLFW_KEY_K) == GLFW_PRESS)
		{
			LevelLoader ll;
			ll.SaveLevel("./json/LevelSaved.json", player->GetEntities(), NPC->GetEntities(), neutralEntities, player->GetBalance());
		}

		// Update all the entities in the scene.
		
		int i;
		#pragma omp parallel for private(i)
		for (i = 0; i < neutralEntities.size(); i++)
		{
			neutralEntities[i]->Update(deltaTime);
		}
		#pragma omp parallel for private(i)
		for (i = 0; i < player->GetEntities().size(); i++)
		{
			player->GetEntities()[i]->Update(deltaTime);
		}
		#pragma omp parallel for private(i)
		for (i = 0; i < NPC->GetEntities().size(); i++)
		{
			NPC->GetEntities()[i]->Update(deltaTime);
		}
		// reduce duration.
		duration -= deltaTime;
		// Delete any entities in the scene that are required to be removed
		
		//// Handle deletion of entities.
		for (i = 0; i < neutralEntities.size(); i++)
		{
			std::shared_ptr<Entity>& e = neutralEntities[i];
			if (e->GetCompatibleComponent<Targetable>() != NULL)
				if (e->GetCompatibleComponent<Targetable>()->IsDead())
				{
					//	e->~Entity();
					neutralEntities.erase(std::remove(neutralEntities.begin(), neutralEntities.end(), e), neutralEntities.end());
				}
		}
		for (i = 0; i < player->GetEntities().size(); i++)
		{
			std::shared_ptr<Entity>& e = player->GetEntities()[i];
			if (e->GetCompatibleComponent<Targetable>() != NULL)
				if (e->GetCompatibleComponent<Targetable>()->IsDead())
				{
					//	e->~Entity();
					player->GetEntities().erase(std::remove(player->GetEntities().begin(), player->GetEntities().end(), e), player->GetEntities().end());
				}
		}
		for (i = 0; i < NPC->GetEntities().size(); i++)
		{
			std::shared_ptr<Entity> e = NPC->GetEntities()[i];
			if (e->GetCompatibleComponent<Targetable>() != NULL)
				if (e->GetCompatibleComponent<Targetable>()->IsDead())
				{
					NPC->GetEntities().erase(std::remove(NPC->GetEntities().begin(), NPC->GetEntities().end(), e), NPC->GetEntities().end());
				}
		}
	
		allEntities.clear();
		allEntities.insert(allEntities.end(), neutralEntities.begin(), neutralEntities.end());
		allEntities.insert(allEntities.end(), player->GetEntities().begin(), player->GetEntities().end());
		allEntities.insert(allEntities.end(), NPC->GetEntities().begin(), NPC->GetEntities().end());
		CheckForWinner(allEntities);
		player->SortEntities(game_cam->GetComponent<Game_Camera>());
	}
	else if (gameOver)
	{
		timeRemaining -= deltaTime;
		if (timeRemaining <= 0)
		{
			gameOver = false;
			timeRemaining = 5.0f;
			StateManager::Get().currentState = StateManager::State::stateMainMenu;
			player->GetEntities().resize(0);
			player->GetSelectedEntities().resize(0);
			player->GetSelectedFriendlyEntity() = NULL;
			player->GetSelectedEntity() = NULL;
			NPC->GetEntities().resize(0);
			NPC->GetSelectedEntities().resize(0);
			neutralEntities.resize(0);
			std::shared_ptr<Entity> tempEntity3 = std::make_shared<Entity>();
			//auto tempLightComponent = new PointLight(); //std::make_unique<PointLight>();
			//tempLightComponent->SetProperties("./json/PointLight.json");
			////tempEntity3->AddComponent(move(tempLightComponent));
			//std::cout << tempLightComponent->ambient.r << " " << tempLightComponent->ambient.g << " " << tempLightComponent->ambient.b << " " << tempLightComponent->ambient.a << std::endl;
			//GameEngine::Get().AddPointLight(tempLightComponent);

			neutralEntities.push_back(tempEntity3);
			std::cout << "Light initialised" << std::endl;
			// This is the floor.
			std::shared_ptr<Entity> tempEntity2 = std::make_shared<Entity>();
			tempEntity2->SetPosition(glm::vec3(0, 0, 0));
			tempEntity2->UpdateTransforms();
			auto tempRenderable2 = std::make_unique<Renderable>();
			tempRenderable2->SetMaterial(new Material());
			tempRenderable2->SetProperties("./json/Plane.json");
			tempEntity2->SetPosition(tempRenderable2->GetPosition());
			tempRenderable2->UpdateTransforms();
			auto tempBoundingBox2 = std::make_unique<BoundingBox>();
			tempBoundingBox2->SetUpBoundingBox(tempRenderable2->GetModel().GetVertexPositions());
			// Set custom material.
			Material* mat = new Material();
			mat->emissive = glm::vec4(0.02, 0.02, 0.02, 1);
			tempRenderable2->SetMaterial(mat);
			tempEntity2->AddComponent(move(tempRenderable2));
			tempEntity2->AddComponent(move(tempBoundingBox2));
			neutralEntities.push_back(tempEntity2);

			// Add starting structures. - This is the same for each NEW game. Maybe they can have random starting positions? - Then resources need to be worried about.
			player->GetEntities().push_back(Spawner::Get().CreateEntity("Base", glm::vec3(50.5, 0, 50.5), player->GetTeam()));
			Spawner::Get().UpdateGameGrid(player->GetEntities()[0]->GetComponent<BoundingSphere>(), 1);
			NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Base", glm::vec3(249.5, 0, 249.5), NPC->GetTeam()));
			Spawner::Get().UpdateGameGrid(NPC->GetEntities()[0]->GetComponent<BoundingSphere>(), 1);
			for (int i = 0; i < 10; i++)
			{
				neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(10 + (i * 2), 0, 30 + (i * -2)), Team::neutral));
				neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(140 + (i * 2), 0, 160 + (i * -2)), Team::neutral));
				neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(270 + (i * 2), 0, 290 + (i * -2)), Team::neutral));
			}
		}
	}


	if (player->GetSelectedEntity() != NULL)
		ui.UpdateEnemyLabels(player->GetSelectedEntity());
	else
		ui.DeselectEnemyLabel();
	// Update the Game UI.
	ui.Update(deltaTime);

	if (freeCamEnabled)
	{
		glm::mat4 camMatrix = free_cam->GetComponent<Free_Camera>().GetProjection() * free_cam->GetComponent<Free_Camera>().GetView();
		GameEngine::Get().SetCamera(camMatrix);
		UserControls::Get().Update(free_cam->GetComponent<Free_Camera>());
		free_cam->Update(deltaTime);
	}
	else
	{
		glm::mat4 camMatrix = game_cam->GetComponent<Game_Camera>().GetProjection() * game_cam->GetComponent<Game_Camera>().GetView();
		GameEngine::Get().SetCamera(camMatrix);
		UserControls::Get().Update(game_cam->GetComponent<Game_Camera>());
		game_cam->Update(deltaTime);
	}


	// process events.
	glfwPollEvents();
	// Close the window if it has been asked too.
	if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		return false;
	else
		return true;
}

void Game::Render()
{
	if (freeCamEnabled)
	{
		GameEngine::Get().SetCameraPos(free_cam->GetComponent<Free_Camera>().GetPosition());
		GameEngine::Get().SetCameraUp(free_cam->GetComponent<Free_Camera>().GetOrientation());
		GameEngine::Get().SetCameraRight(free_cam->GetComponent<Free_Camera>().GetRight());
	}
	else
	{
		GameEngine::Get().SetCameraPos(game_cam->GetComponent<Game_Camera>().GetPosition());
		GameEngine::Get().SetCameraUp(game_cam->GetComponent<Game_Camera>().GetOrientation());
		GameEngine::Get().SetCameraRight(game_cam->GetComponent<Game_Camera>().GetRight());
	}

	int n;
	#pragma omp parallel for private(n)
	for (n = 0; n < neutralEntities.size(); n++)
	{
		neutralEntities[n]->Render();
	}
	#pragma omp parallel for private(n)
	for (n = 0; n < player->GetEntities().size(); n++)
	{
		player->GetEntities()[n]->Render();
	}
	#pragma omp parallel for private(n)
	for (n = 0; n < NPC->GetEntities().size(); n++)
	{
		NPC->GetEntities()[n]->Render();
	}

	// If there is still duration, render the particle.
	if (duration > 0)
		location.Render();

	// render ghost building.
	player->Render();
	GameEngine::Get().Render();
	ui.Render();
	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Get().GetWindow());
}


void Game::UpdateNavGrid(int val, glm::ivec2 pos)
{
	//	std::cout << "Pos:" << pos.x<< "," << pos.y << " Value:" << navGrid[pos.x][pos.y] << std::endl;
	navGrid[pos.x][pos.y] = val;
	//	std::cout << "Pos:" << pos.x << "," << pos.y << " Value:" << navGrid[pos.x][pos.y] << std::endl;
}

// hacky approach to approximating framerate, causes application to crash on closing.
//static double dts[255];
//static unsigned char dti =0;
//dts[dti] = deltaTime;
//if (dti == 0)
//{
//	double avg = 0;
//	double min = dts[0];
//	double max = dts[0];
//	for (auto f : dts){
//		avg += f;
//		min = (f < min ? f : min);
//		max = (f > max ? f : max);
//	}
//	avg /= 255.0f;
//	printf("fps:%f, avg:%f, min: %f, max:%f\n",1.0/avg, avg, min, max);
//}
//++dti;