#include "Game.h"
#include "Entity.h"
#include "PointLight.h"

std::vector<Entity*> Game::entities;
double Game::lastTime;

void Game::SpawnUnit(glm::vec3 position, glm::vec2 size)
{
	for (int i = 0; i < 1; i++)
	{
		glm::vec3 spawnPosition = position;
		if (i % 2 == 0)
		{
			spawnPosition.x;
			spawnPosition.z;
			size.x = -size.x;
			size.y = -size.y;
		}
		else
		{

		}
		Entity* tempFlyer = new Entity;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("../res/models/Flyer.obj");
		tempRenderable->SetEffect("FlyerUV");
		tempFlyer->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<AirMovement>();
		tempAirMovement->SetDestination(glm::dvec3(20, 15, 20));
		tempAirMovement->SetSpeed(15.0);
		tempAirMovement->SetAcceleration(0.5);
		tempAirMovement->SetTurnSpeed(200.0);

		tempFlyer->AddComponent(move(tempRenderable));
		tempFlyer->AddComponent(move(tempAirMovement));
		entities.push_back(tempFlyer);
		return;
	}
}

void Game::Initialise()
{
	free_cam = new Entity;
	auto cam = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam->SetPosition(glm::dvec3(10.0, 5.0, 50.0));
	cam->SetProjection(GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight(), 2.414f, 1000);
	std::cout << GameEngine::Get().GetScreenHeight();
	free_cam->AddComponent(move(cam));

	// Add a red point light to 0, 0.5, 0
	Entity* tempEntity3 = new Entity;
	for(int i = 1; i < 5; i++)
	{
		for(int j = 1; j < 5; j++)
		{
			auto tempLightComponent = std::make_unique<PointLight>();
			tempLightComponent->SetEffect("Phong");
			tempLightComponent->setLightPosition(glm::vec3(i * 30 - 30, 10, j * 30 - 30));
			tempLightComponent->diffuse = glm::vec4(i / 4, j / 4, i % j / 8, 1);
			tempEntity3->AddComponent(move(tempLightComponent));
		}
	}
	entities.push_back(tempEntity3);

	Entity* tempEntity = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("../res/models/Constructor.obj");
	tempRenderable->SetEffect("ConstructorUV");
	tempEntity->SetPosition(glm::vec3(3.5f, 2.5f, 3.5f));
	tempRenderable->UpdateTransforms();
	auto tempStructure = std::make_unique<Structure>();

	tempEntity->AddComponent(move(tempRenderable));
	tempEntity->AddComponent(move(tempStructure));

	entities.push_back(tempEntity);

	Entity* tempEntity2 = new Entity;
	auto tempRenderable2 = std::make_unique<Renderable>();
	tempRenderable2->SetPlane(1, 100, 100);
	tempRenderable2->SetEffect("debug");
	tempEntity2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable2->UpdateTransforms();
	tempEntity2->AddComponent(move(tempRenderable2));

	entities.push_back(tempEntity2);
	lastTime = clock();
}

void Game::Update()
{
	glm::mat4 camMatrix = free_cam->GetComponent<Free_Camera>().GetProjection() * free_cam->GetComponent<Free_Camera>().GetView();
	GameEngine::Get().SetCamera(camMatrix);
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	free_cam->Update(deltaTime);
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		//entities[n]->Rotate(glm::vec3(0.01f, 0.01f, 0.0f));
		entities[n]->Update(deltaTime);
		n++;
	}

//	printf("%f.9\n", deltaTime);
}

void Game::Render()
{
	// Clear the opengl buffer.
	glClearColor(0.1, 0.0, 0.4, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_CULL_FACE);

//	GameEngine::Get().SetCameraPos(free_cam->GetPosition());
	GameEngine::Get().SetCameraPos(free_cam->GetComponent<Free_Camera>().GetPosition());

	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		entities[n]->Render();
		n++;
	}

	// process events.
	glfwPollEvents();
	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Get().GetWindow());
}