#include "Game.h"

Game *Game::instance = 0;
std::vector<Entity*> Game::entities;

void Game::Initialise()
{
	Entity* tempEntitiy = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("../res/models/Torus2.obj");
	tempRenderable->SetEffect();
	tempEntitiy->AddComponent(move(tempRenderable));
	entities.push_back(tempEntitiy);
}

void Game::Update()
{
}

void Game::Render()
{
	entities[0]->Render();
}
