#include "Game.h"
#include "Entity.h"
#include "PointLight.h"
#include "AiPlayer.h"
#include "Spawner.h"

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

void Game::Initialise()
{
	grid = new int*[100];
	for (int i = 0; i < 100; i++)
		grid[i] = new int[100];
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
		{
			if (i > 50 && i < 75 && j>50 && j < 75)
				grid[i][j] = 1;
			else
				grid[i][j] = 0;
		}


	player = new Player;
	NPC = new AiPlayer;

	player->SetTeam(0);
	NPC->SetTeam(1);
	free_cam = new Entity;
	auto cam = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam->SetPosition(glm::dvec3(10.0, 5.0, 50.0));
	cam->SetProjection((float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	free_cam->AddComponent(move(cam));
	// Add light entity.
	Entity* tempEntity3 = new Entity;
	auto tempLightComponent = std::make_unique<PointLight>();
	tempLightComponent->SetEffect("Phong");
	tempLightComponent->setLightPosition(glm::vec3(50, 30, 50));
	tempLightComponent->diffuse = glm::vec4(0.7, 0.2, 0.4, 1);
	tempEntity3->AddComponent(move(tempLightComponent));
	entities.push_back(tempEntity3);

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
	mat->emissive = glm::vec4(0.03,0,0.07,1);
	tempRenderable2->SetMaterial(mat);
	tempEntity2->AddComponent(move(tempRenderable2));
	tempEntity2->AddComponent(move(tempBoundingBox2));
	entities.push_back(tempEntity2);
	// Add starting structures. - This is the same for each NEW game. Maybe they can have random starting positions? - Then resources need to be worried about.
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Shipyard", glm::vec3(3.5, 2.5, 3.5), player->GetTeam()));
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Worker", glm::vec3(20, 2.5, 20), player->GetTeam()));

	
	
	
	NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Shipyard", glm::vec3(90, 2.5, 90), NPC->GetTeam()));
//	NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Worker", glm::vec3(70, 2.5, 70), NPC->GetTeam()));

	// This will be added to a neutral list later.
	NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Resource", glm::vec3(30,2.5,30), NPC->GetTeam()));

	/*Entity* tempEntity77 = new Entity;
	auto tempRenderable77 = std::make_unique<Renderable>();
	tempRenderable77->SetMaterial(new Material());
	tempRenderable77->SetPlane(1, 25, 25);
	tempRenderable77->SetTexture("ConstructorUV");
	tempRenderable77->SetShader("Phong");
	tempEntity77->SetPosition(glm::vec3(50.0f, 1.0f, 50.0f));

	tempRenderable77->UpdateTransforms();
	tempEntity77->AddComponent(move(tempRenderable77));
	entities.push_back(tempEntity77);*/
	lastTime = clock();
}

std::thread threads[1];

bool Game::Update()
{
	// Get user and ai actions.
	player->Update(NPC->GetEntities());
	NPC->Update(player->GetEntities());
	// Set camera matrix.
	glm::mat4 camMatrix = free_cam->GetComponent<Free_Camera>().GetProjection() * free_cam->GetComponent<Free_Camera>().GetView();
	GameEngine::Get().SetCamera(camMatrix);
	// Update mouse ray.
	UserControls::Get().Update(free_cam->GetComponent<Free_Camera>());
	// Calculate time since last frame. (deltaTime)
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	// Update camera.
	free_cam->Update(deltaTime);

	// Update neutral entities.
	int i = 0;
//	#pragma omp parallel for private(i)
	for (i = 0; i < entities.size();i++)
	{
		entities[i]->Update(deltaTime);
	}

	// Update user entities.
	// Spread this around several threads. - Find a better approach later.
	int range = player->GetEntities().size()/2;
	int start, end;
	// Delegate work around threads.
	for (int i = 0; i < 1; i++)
	{
		start = i*range;
		end = (i+1)*range;
		// this works but is very bad on performance!
	//	threads[i] = (std::thread(UpdateEntityList,start,end,deltaTime,std::ref(player->GetEntities())));
	}
	
	//// Calculate final range on main thread.
	//UpdateEntityList(1*range, 2*range, deltaTime, player->GetEntities());

	//// wait and join on all threads.
	//for (std::thread &t : threads)
	//	t.join();

	//#pragma omp parallel for private(i)
	for (i = 0; i < player->GetEntities().size();i++)
	{
		Entity*& e = player->GetEntities()[i];
		/*if (e->GetCompatibleComponent<Targetable>() != NULL)
			if (e->GetCompatibleComponent<Targetable>()->IsDead())
			{
				player->GetEntities().erase(std::remove(player->GetEntities().begin(), player->GetEntities().end(), e), player->GetEntities().end());
			}*/
		e->Update(deltaTime);
	}


	// Update enemy entities.
//	#pragma omp parallel for private(i)
	for (i = 0; i < NPC->GetEntities().size();i++)
	{
		Entity*& e = NPC->GetEntities()[i];
		//if (e->GetCompatibleComponent<Targetable>() != NULL)
		//	if (e->GetCompatibleComponent<Targetable>()->IsDead())
		//	{
		//		// Delete this.
		//		NPC->GetEntities().erase(std::remove(NPC->GetEntities().begin(), NPC->GetEntities().end(), e), NPC->GetEntities().end());
		//	}
		e->Update(deltaTime);
	}



	// Need to delete dead entities at some time - serial.


	// hacky approach to approximating framerate, causes application to crash on closing.
	static double dts[255];
	static unsigned char dti =0;
	dts[dti] = deltaTime;
	if (dti == 0)
	{
		double avg = 0;
		double min = dts[0];
		double max = dts[0];
		for (auto f : dts){
			avg += f;
			min = (f < min ? f : min);
			max = (f > max ? f : max);
		}
		avg /= 255.0f;
		printf("fps:%f, avg:%f, min: %f, max:%f\n",1.0/avg, avg, min, max);
	}
	++dti;


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
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		entities[n]->Render();
		n++;
	}

	for (std::vector<Entity*>::size_type n = 0; n < player->GetEntities().size();)
	{
		player->GetEntities()[n]->Render();
		n++;
	}

	for (std::vector<Entity*>::size_type n = 0; n < NPC->GetEntities().size();)
	{
		NPC->GetEntities()[n]->Render();
		n++;
	}

	GameEngine::Get().Render();

	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Get().GetWindow());
}