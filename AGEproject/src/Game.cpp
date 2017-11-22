#include "Game.h"
#include "Entity.h"
#include "PointLight.h"
#include "AiPlayer.h"
#include "Spawner.h"
#include "UserControls.h"
#include <thread>
#include <omp.h>

void UpdateEntityList(int start, int end, double deltaTime, std::vector<Entity*>& entities)
{
	//return;
	for (int i = start; i < end; i++)
	{
		Entity*& e = entities[i];
		e->Update(deltaTime);
	}
}

void Game::HandleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}


void Game::Initialise()
{

	//glfwSetKeyCallback(GameEngine::Get().GetWindow(), Game::Get().HandleInput);
	navGrid = new int*[100];
	for (int i = 0; i < 100; i++)
		navGrid[i] = new int[100];
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
		{
			if (i > 50 && i < 75 && j>50 && j < 75)
				navGrid[i][j] = 1;
			else
				navGrid[i][j] = 0;
		}
	terrainGrid = new dvec3*[100];
	for (int i = 0; i < 100; i++)
	{
		terrainGrid[i] = new dvec3[100];
		for (int j = 0; j < 100; j++)
		{
			terrainGrid[i][j] = vec3(i, 0, j);
		}
	}
	player = new Player;
	NPC = new AiPlayer;

	player->SetTeam(Team::player);
	NPC->SetTeam(Team::ai);
	free_cam = new Entity;
	auto cam = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam->SetPosition(glm::dvec3(10.0, 5.0, 50.0));
	cam->Rotate(-4 / pi<float>(), -4 / pi<float>());
	//cam->Rotate(4 / pi<float>(), -3 / pi<float>());
	cam->SetProjection((float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	free_cam->AddComponent(move(cam));
	// Add light entity.
	Entity* tempEntity3 = new Entity;
	auto tempLightComponent = std::make_unique<PointLight>();
	tempLightComponent->SetEffect("Phong");
	tempLightComponent->setLightPosition(glm::vec3(50, 30, 50));
	tempLightComponent->diffuse = glm::vec4(0.7, 0.2, 0.4, 1);
	tempEntity3->AddComponent(move(tempLightComponent));
	neutralEntities.push_back(tempEntity3);

	// This is the floor.
	Entity* tempEntity2 = new Entity;
	auto tempRenderable2 = std::make_unique<Renderable>();
	tempRenderable2->SetPlane(1, 100, 100);
	tempRenderable2->SetTexture("debug");
	tempRenderable2->SetShader("Phong");
	tempEntity2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable2->UpdateTransforms();
	auto tempBoundingBox2 = std::make_unique<BoundingBox>();
	tempBoundingBox2->SetUpBoundingBox(tempRenderable2->GetModel().GetVertexPositions());
	// Set custom material.
	Material* mat = new Material();
	mat->emissive = glm::vec4(0.03, 0, 0.07, 1);
	tempRenderable2->SetMaterial(mat);
	tempEntity2->AddComponent(move(tempRenderable2));
	tempEntity2->AddComponent(move(tempBoundingBox2));
	neutralEntities.push_back(tempEntity2);


	// Add starting structures. - This is the same for each NEW game. Maybe they can have random starting positions? - Then resources need to be worried about.
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Base", glm::vec3(3.5, 2.5, 3.5), player->GetTeam()));
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Ship", glm::vec3(3.5, 2.5, 3.5), player->GetTeam()));
	NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Base", glm::vec3(90, 2.5, 90), NPC->GetTeam()));
	// This will be added to a neutral list later.
	neutralEntities.push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(30, 2.5, 30), Team::neutral));

	//This is a "wall"
	Entity* tempEntity77 = new Entity;
	auto tempRenderable77 = std::make_unique<Renderable>();
	tempRenderable77->SetMaterial(new Material());
	tempRenderable77->SetPlane(1, 25, 25);
	tempRenderable77->SetTexture("ConstructorUV");
	tempRenderable77->SetShader("Phong");
	tempEntity77->SetPosition(glm::vec3(50.0f, 2.5f, 50.0f));

	neutralEntities.push_back(move(tempEntity77));

	lastTime = clock();
}


bool Game::Update()
{
	// Populate the all entity list.
	allEntities.clear();
	//allEntities.resize(player->GetEntities().size() + NPC->GetEntities().size() + neutralEntities.size());
	
	allEntities.insert(allEntities.end(), neutralEntities.begin(), neutralEntities.end());	
	allEntities.insert(allEntities.end(), player->GetEntities().begin(), player->GetEntities().end());
	allEntities.insert(allEntities.end(), NPC->GetEntities().begin(), NPC->GetEntities().end());
	// all entities now contains all the entities in the game.

	// Let user and update their actions. 
	player->Update(allEntities);
	NPC->Update(allEntities);
	// Set camera matrix.
	glm::mat4 camMatrix = free_cam->GetComponent<Free_Camera>().GetProjection() * free_cam->GetComponent<Free_Camera>().GetView();
	GameEngine::Get().SetCamera(camMatrix);
	// Update mouse ray.
	UserControls::Get().Update(free_cam->GetComponent<Free_Camera>());
	// Calculate time since last frame. (deltaTime)
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	// Update clock.
	free_cam->Update(deltaTime);
	// reduce duration.
	duration -= deltaTime;
	//// Update neutral entities.
	//int i = 0;
	//#pragma omp parallel for private(i)
	//for (i = 0; i < neutralEntities.size();i++)
	//{
	//	neutralEntities[i]->Update(deltaTime);
	//}
	//// Update user entities.
	//#pragma omp parallel for private(i)
	//for (i = 0; i < player->GetEntities().size();i++)
	//{
	//	Entity*& e = player->GetEntities()[i];
	//	e->Update(deltaTime);
	//}

	//// Update enemy entities.
	//#pragma omp parallel for private(i)
	//for (i = 0; i < NPC->GetEntities().size();i++)
	//{
	//	Entity*& e = NPC->GetEntities()[i];
	//	e->Update(deltaTime);
	//}

	// Update all the entities in the scene.
	int i;
	#pragma omp parallel for private(i)
	for (i = 0; i < allEntities.size();i++)
	{
		allEntities[i]->Update(deltaTime);
	}

	//// Delete any entities in the scene that are required to be removed.
	//for (i = 0; i <allEntities.size(); i++)
	//{
	//	Entity*& e = allEntities[i];
	//	if (e->GetCompatibleComponent<Targetable>() != NULL)
	//	{
	//		if (e->GetCompatibleComponent<Targetable>()->IsDead())
	//		{
	//			allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), e), allEntities.end());
	//		}
	//	}
	//}
	//// Remove bullets no longer used.
	//projectiles.erase(std::remove_if
	//(projectiles.begin(), projectiles.end(), [](const BulletParticle& x)
	//{
	//	return !x.isActive;
	//}), projectiles.end());


	
	//// Handle deletion of entities.
	for (i = 0; i <neutralEntities.size(); i++)
	{
		Entity*& e = neutralEntities[i];
		if (e->GetCompatibleComponent<Targetable>() != NULL)
			if (e->GetCompatibleComponent<Targetable>()->IsDead())
			{
			//	e->~Entity();
				neutralEntities.erase(std::remove(neutralEntities.begin(), neutralEntities.end(), e), neutralEntities.end());
			}
	}
	for (i = 0; i < player->GetEntities().size(); i++)
	{
		Entity*& e = player->GetEntities()[i];
		if (e->GetCompatibleComponent<Targetable>() != NULL)
			if (e->GetCompatibleComponent<Targetable>()->IsDead())
			{
			//	e->~Entity();
				player->GetEntities().erase(std::remove(player->GetEntities().begin(), player->GetEntities().end(), e), player->GetEntities().end());
			}
	}
	for (i = 0; i < NPC->GetEntities().size(); i++)
	{
		Entity* e = NPC->GetEntities()[i];
		if (e->GetCompatibleComponent<Targetable>() != NULL)
			if (e->GetCompatibleComponent<Targetable>()->IsDead())
			{
				NPC->GetEntities().erase(std::remove(NPC->GetEntities().begin(), NPC->GetEntities().end(), e), NPC->GetEntities().end());
			}
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
	GameEngine::Get().SetCameraPos(free_cam->GetComponent<Free_Camera>().GetPosition());
	GameEngine::Get().SetCameraUp(free_cam->GetComponent<Free_Camera>().GetOrientation());
	GameEngine::Get().SetCameraRight(free_cam->GetComponent<Free_Camera>().GetRight());
	
	int n;
	#pragma omp parallel for private(n)
	for (n = 0; n < neutralEntities.size();n++)
	{
		neutralEntities[n]->Render();
	}
	#pragma omp parallel for private(n)
	for (n = 0; n < player->GetEntities().size();n++)
	{
		player->GetEntities()[n]->Render();
	}
	#pragma omp parallel for private(n)
	for (n = 0; n < NPC->GetEntities().size();n++)
	{
		NPC->GetEntities()[n]->Render();
	}


	// If there is still duration, render the particle.
	if (duration > 0)
		location.Render();

	// render ghost building.
	player->Render();
	GameEngine::Get().Render();

	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Get().GetWindow());
}