#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include <memory>

void StateManager::StateLoop()
{
	GameEngine::Get().Initialise();

	while (!glfwWindowShouldClose(GameEngine::Get().GetWindow()))
	{
		switch (state)
		{
		case(Splash):
			//GameEngine::Instance()->Render();
			state = Menu;
			break;
		case(Menu):
			ShowMainMenu();
			Game::Get().Initialise(); //This will need a new home later.
			state = Playing;
			break;
		case(Settings):
			break;
		case(Playing):
			Game::Get().Update();
			Game::Get().Render();
			break;
		case(Exiting):
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
			break;
		}
	}
	GameEngine::Get().CleanUp();
}