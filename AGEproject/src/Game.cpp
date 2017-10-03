#include "Game.h"

Game *Game::instance = 0;
std::vector<Entity*> Game::entities;
float Game::lastTime = 0.0f;

void Game::Initialise()
{
	Entity* tempEntitiy = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("../res/models/Constructor.obj");
	tempRenderable->SetEffect();
	tempRenderable->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable->UpdateTransforms();
	tempEntitiy->AddComponent(move(tempRenderable));

	entities.push_back(tempEntitiy);

	/*Entity* tempEntitiy2 = new Entity;
	auto tempRenderable2 = std::make_unique<Renderable>();
	tempRenderable2->SetPlane(1,10,10);
	tempRenderable2->SetEffect();
	tempRenderable2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable2->UpdateTransforms();
	tempEntitiy2->AddComponent(move(tempRenderable2));

	entities.push_back(tempEntitiy2);*/
}

void Game::Update()
{
	float deltaTime = (float(clock()) - lastTime) / (float)CLOCKS_PER_SEC;
	lastTime = float(clock());
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		entities[n]->Rotate(glm::vec3(0.01f, 0.01f, 0.0f));
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
