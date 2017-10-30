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
		case(stateSplash):
			//GameEngine::Instance()->Render();
			state = stateMainMenu;
			break;
		case(stateMainMenu):
			select = ShowMainMenu();
			if (select == 0)
			{
				Game::Get().Initialise(); //This will need a new home later.
				state = statePlaying;
			}
			else if (select == 1)
			{

			}
			else if (select == 2) { state = stateExiting; }
			break;
		case(stateSettings):
			break;
		case(statePlaying):
			running = Game::Get().Update();
			Game::Get().Render();
			break;
		case(stateExiting):
			running = false;
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
			break;
		}
	}
	GameEngine::Get().CleanUp();
}