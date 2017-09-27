#include "Game.h"
#include <memory>

Game *Game::instance = 0;

void Game::Initialise()
{
	Entity tempEntitiy;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("../res/models/Torus2.obj");
	tempRenderable->SetEffect();
	tempEntitiy.AddComponent(move(tempRenderable));
	entities.push_back(tempEntitiy);
}

void Game::Update()
{
}

void Game::Render()
{
	entities[0].Render();
}
