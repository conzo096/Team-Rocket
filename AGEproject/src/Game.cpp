#include "Game.h"
#include "Entity.h"
#include "PointLight.h"
#include "UserControls.h"
//#include "ShipUnit.h"
#include "Targetable.h"
#include "AiPlayer.h"
#include "Barracks.h"
#include "Spawner.h"
void Game::Initialise()
{
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
	tempLightComponent->setLightPosition(glm::vec3(50,30,50));
	tempLightComponent->diffuse = glm::vec4(0.7,0.2,0.4,1);
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

	// Add starting structures.
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Shipyard", glm::vec3(3.5, 2.5, 3.5), player->GetTeam()));
	player->GetEntities().push_back(Spawner::Get().CreateEntity("Worker", glm::vec3(20, 2.5, 20), player->GetTeam()));
	NPC->GetEntities().push_back(Spawner::Get().CreateEntity("Barracks", glm::vec3(90, 2.5, 90), NPC->GetTeam()));

	lastTime = clock();
}

bool Game::Update()
{
	player->Update(NPC->GetEntities());
	NPC->Update(player->GetEntities());
	glm::mat4 camMatrix = free_cam->GetComponent<Free_Camera>().GetProjection() * free_cam->GetComponent<Free_Camera>().GetView();
	GameEngine::Get().SetCamera(camMatrix);
	UserControls::Get().Update(free_cam->GetComponent<Free_Camera>());
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	free_cam->Update(deltaTime);
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{

		entities[n]->Update(deltaTime);
		n++;
	}
	for (std::vector<Entity*>::size_type n = 0; n < player->GetEntities().size();)
	{
		Entity*& e = player->GetEntities()[n];
		if (e->GetCompatibleComponent<Targetable>() != NULL)
			if (e->GetCompatibleComponent<Targetable>()->IsDead())
			{
				player->GetEntities().erase(std::remove(player->GetEntities().begin(), player->GetEntities().end(), e), player->GetEntities().end());
			}
		e->Update(deltaTime);
		n++;
	}

	for (std::vector<Entity*>::size_type n = 0; n < NPC->GetEntities().size();)
	{
		Entity*& e = NPC->GetEntities()[n];
		if (e->GetCompatibleComponent<Targetable>() != NULL)
			if (e->GetCompatibleComponent<Targetable>()->IsDead())
			{
				// Delete this.
				NPC->GetEntities().erase(std::remove(NPC->GetEntities().begin(), NPC->GetEntities().end(), e), NPC->GetEntities().end());
			}
		e->Update(deltaTime);
		n++;
	}
	//printf("%f.9\n", deltaTime);
	if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		return false;
	else
		return true;
}

void Game::Render()
{

//	GameEngine::Get().SetCameraPos(free_cam->GetPosition());
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

	// process events.
	glfwPollEvents();
	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Get().GetWindow());
}