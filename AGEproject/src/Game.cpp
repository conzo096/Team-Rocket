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

	game_cam = new Entity;
	auto cam1 = std::make_unique<Game_Camera>();
	cam1->Rotate(pi<float>() / -4.0f);
	cam1->SetPosition(glm::dvec3(85.0, 60.0, 85.0));
	cam1->SetProjection(glm::half_pi<float>(), (float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	game_cam->AddComponent(move(cam1));

	free_cam = new Entity;
	auto cam2 = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam2->Rotate(0.0f, -3.0f / pi<float>());
	cam2->SetPosition(glm::dvec3(40.0, 40.0, 70.0));
	cam2->SetProjection((float)(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	free_cam->AddComponent(move(cam2));

	freeCamEnabled = false;
	keyHeld = false;

	// todo: Remove entity creation from this init method.


	// Add point light to the scene
	Entity* tempEntity3 = new Entity;
	auto tempLightComponent = std::make_unique<PointLight>();
	tempLightComponent->SetProperties("./json/PointLight.json");
	tempEntity3->AddComponent(move(tempLightComponent));
	entities.push_back(tempEntity3);

	// Box at corner
	Entity* tempEntity = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetMaterial(new Material());
	tempRenderable->SetProperties("./json/Renderable0.json");
	tempEntity->SetPosition(tempRenderable->GetPosition());
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

	// Box not in corner
	Entity* tempEntityn = new Entity;
	auto tempRenderablen = std::make_unique<Renderable>();
	tempRenderablen->SetMaterial(new Material());
	tempRenderablen->SetProperties("./json/Renderable1.json");
	tempEntityn->SetPosition(tempRenderablen->GetPosition());
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
	tempRenderable2->SetMaterial(new Material());
	tempRenderable2->SetPlane(1, 100, 100);
	tempRenderable2->SetProperties("./json/Plane.json");
	tempEntity2->SetPosition(tempRenderable2->GetPosition());
	tempRenderable2->UpdateTransforms();
	auto tempBoundingBox2 = std::make_unique<BoundingBox>();
	tempBoundingBox2->SetUpBoundingBox(tempRenderable2->GetModel().GetVertexPositions());
	Material* mat = new Material();
	mat->emissive = glm::vec4(0.02, 0.02, 0.02, 1);
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

	if (UserControls::Get().KeyBuffer(std::string("Enter"), keyHeld))
	{
		freeCamEnabled = !freeCamEnabled;
	}

	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	//free_cam->GetComponent<Free_Camera>().Rotate(45,45);

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