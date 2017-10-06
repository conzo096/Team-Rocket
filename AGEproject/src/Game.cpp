#include "Game.h"
#include "Entity.h"

Game *Game::instance = 0;
std::vector<Entity*> Game::entities;
double Game::lastTime = 0.0f;
Entity* Game::cam = new Entity();

void Game::SpawnUnit(glm::vec3 position, glm::vec2 size)
{
	for (int i = 0; i < 1; i++)
	{
		glm::vec3 spawnPosition = position;
		spawnPosition.x += size.x;
		Entity* tempEntity = new Entity;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("../res/models/Constructor.obj");
		tempRenderable->SetEffect("ConstructorUV");
		tempEntity->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempStructure = std::make_unique<Structure>();

		tempEntity->AddComponent(move(tempRenderable));
		tempEntity->AddComponent(move(tempStructure));
		entities.push_back(tempEntity);
		return;
	}
}

void Game::Initialise()
{
	auto camera = std::make_unique<Free_Camera>((GameEngine::Instance()->GetScreenWidth() / GameEngine::Instance()->GetScreenHeight()), 90.0f);
	camera->SetProjection(0.2, 1000);
	cam->AddComponent(move(camera));

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
	tempEntity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable2->UpdateTransforms();
	tempEntity2->AddComponent(move(tempRenderable2));

	entities.push_back(tempEntity2);
}

void Game::Update()
{
	glm::mat4 camMatrix = cam->GetComponent<Free_Camera>().GetProjection() * cam->GetComponent<Free_Camera>().GetView();
	GameEngine::Instance()->SetCamera(camMatrix);
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	cam->Update(deltaTime);
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		//entities[n]->Update(deltaTime);
		//entities[n]->Rotate(glm::vec3(0.01f, 0.01f, 0.0f));
		entities[n]->UpdateTransforms();
		entities[n]->Update(deltaTime);
		n++;
	}



	printf("%.9f\n", deltaTime);
}

void Game::Render()
{
	// Clear the opengl buffer.
	glClearColor(0.1, 0.0, 0.4, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_CULL_FACE);

	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		entities[n]->Render();
		n++;
	}

	// process events.
	glfwPollEvents();
	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Instance()->GetWindow());
}
