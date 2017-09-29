#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include <memory>
StateManager *StateManager::instance = 0;

void StateManager::StateLoop()
{
	GameEngine::Instance()->Initialise();
	Game::Instance()->Initialise();
	while (!glfwWindowShouldClose(GameEngine::Instance()->GetWindow()))
	{
		switch (state)
		{
		case(Splash):
			//GameEngine::Instance()->Render();
			Game::Instance()->Render();
			break;
		case(Menu):
			break;
		case(Settings):
			break;
		case(Playing):
			break;
		case(Exiting):
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
			break;
		}
	}
	GameEngine::Instance()->CleanUp();
}
