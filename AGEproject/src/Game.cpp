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
	tempRenderable->SetPosition(glm::vec3(0.0f, 0.0f, -50.0f));
	tempRenderable->UpdateTransforms();
	tempEntitiy->AddComponent(move(tempRenderable));

	entities.push_back(tempEntitiy);
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
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		entities[n]->Render();
		n++;
	}
}
