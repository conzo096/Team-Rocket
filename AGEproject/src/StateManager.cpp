#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include <memory>

void StateManager::StateLoop()
{
	GameEngine::Get().Initialise();
	running = true;

	while (running)
	{
		switch (state)
		{
		case(Splash):
			//GameEngine::Instance()->Render();
			state = MainMenu;
			break;
		case(MainMenu):
			select = ShowMainMenu();
			if (select == 0)
			{
				Game::Get().Initialise(); //This will need a new home later.
				state = Playing;
			}
			else if (select == 1)
			{

			}
			else if (select == 2) { state = Exiting; }
			break;
		case(Settings):
			break;
		case(Playing):
			running = Game::Get().Update();
			Game::Get().Render();
			break;
		case(Exiting):
			running = false;
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
			break;
		}
	}
	GameEngine::Get().CleanUp();
}