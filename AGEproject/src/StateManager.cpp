#include "StateManager.h"

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
			ShowMainMenu();
			state = Menu;
			break;
		case(Menu):
			Game::Instance()->Update();
			Game::Instance()->Render();

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
