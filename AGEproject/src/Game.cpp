#include "Game.h"
#include "Entity.h"
#include "PointLight.h"
#include "UserControls.h"
#include "ShipUnit.h"
#include "Targetable.h"
#include "AiPlayer.h"

void Game::Initialise()
{
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

	player->SetTeam(0);
	NPC->SetTeam(1);
	free_cam = new Entity;
	auto cam = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam->SetPosition(glm::dvec3(10.0, 5.0, 50.0));
	cam->SetProjection((float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	free_cam->AddComponent(move(cam));

	// todo Remove entity creation from this init method.



	// Add a red point light to 0, 0.5, 0
	Entity* tempEntity3 = new Entity;
	auto tempLightComponent = std::make_unique<PointLight>();
	tempLightComponent->SetEffect("Phong");
	tempLightComponent->setLightPosition(glm::vec3(50, 30, 50));
	tempLightComponent->diffuse = glm::vec4(0.7, 0.2, 0.4, 1);
	tempEntity3->AddComponent(move(tempLightComponent));
	entities.push_back(tempEntity3);

	Entity* tempEntity = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("Shipyard");
	tempRenderable->SetShader("Phong");
	tempRenderable->SetMaterial(new Material());
	tempRenderable->SetTexture("ConstructorUV");
	tempEntity->SetPosition(glm::vec3(3.5f, 2.5f, 3.5f));
	tempRenderable->UpdateTransforms();
	auto tempStructure = std::make_unique<Structure>();
	tempStructure->SetTeam(player->GetTeam());
	auto tempBoundSphere = std::make_unique<BoundingSphere>();
	tempBoundSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
	tempEntity->AddComponent(move(tempBoundSphere));
	tempEntity->AddComponent(move(tempRenderable));
	tempEntity->AddComponent(move(tempStructure));
	auto target = std::make_unique<Targetable>();
	target->SetHealth(100);
	tempEntity->AddComponent(move(target));
	player->GetEntities().push_back(tempEntity);


	Entity* tempEntityn = new Entity;
	auto tempRenderablen = std::make_unique<Renderable>();
	tempRenderablen->SetModel("Shipyard");
	tempRenderablen->SetTexture("ConstructorUV");
	tempRenderablen->SetShader("Phong");
	tempRenderablen->SetMaterial(new Material());
	tempEntityn->SetPosition(glm::vec3(30.5f, 2.5f, 30.5f));
	tempRenderablen->UpdateTransforms();
	auto tempStructuren = std::make_unique<Structure>();
	tempStructuren->SetTeam(player->GetTeam());
	auto tempBoundSpheren = std::make_unique<BoundingSphere>();
	tempBoundSpheren->SetUpBoundingSphere(tempRenderablen->GetModel().GetVertexPositions());
	tempEntityn->AddComponent(move(tempBoundSpheren));
	tempEntityn->AddComponent(move(tempRenderablen));
	tempEntityn->AddComponent(move(tempStructuren));
	auto targetn = std::make_unique<Targetable>();
	targetn->SetHealth(100);
	tempEntityn->AddComponent(move(targetn));

	NPC->GetEntities().push_back(tempEntityn);



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
	Material* mat = new Material();
	mat->emissive = glm::vec4(0.03,0,0.07,1);
	tempRenderable2->SetMaterial(mat);
	tempEntity2->AddComponent(move(tempRenderable2));
	tempEntity2->AddComponent(move(tempBoundingBox2));
	entities.push_back(tempEntity2);

	//This is a "wall"
	Entity* tempEntity77 = new Entity;
	auto tempRenderable77 = std::make_unique<Renderable>();
	tempRenderable77->SetMaterial(new Material());
	tempRenderable77->SetPlane(1, 25, 25);
	tempRenderable77->SetTexture("ConstructorUV");
	tempRenderable77->SetShader("Phong");
	tempEntity77->SetPosition(glm::vec3(50.0f, 1.0f, 50.0f));

	tempRenderable77->UpdateTransforms();
	tempEntity77->AddComponent(move(tempRenderable77));
	entities.push_back(tempEntity77);

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