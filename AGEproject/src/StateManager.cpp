#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include "UserControls.h"
#include <memory>

void StateManager::StateLoop()
{
	GameEngine::Get().Initialise();
	bool running = true;
	int select;

	while (running)
	{
			switch (state)
			{
			case(stateSplash):
				ShowSplashScreen();
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
					state = stateSettings;
				}
				else if (select == 2) { state = stateExiting; }
				break;
			case(stateSettings):
				select = ShowSettingsMenu();
				if (select == 2) { state = stateMainMenu; }
				else if (select == 8) { state = stateExiting; }
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
		glfwPollEvents();
	}
	GameEngine::Get().CleanUp();
}