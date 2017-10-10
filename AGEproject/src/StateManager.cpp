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
			state = Menu;
			break;
		case(Menu):
			ShowMainMenu();
			state = Playing;
			break;
		case(Settings):
			break;
		case(Playing):
			Game::Instance()->Update();
			Game::Instance()->Render();

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
