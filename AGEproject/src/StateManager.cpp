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
		glfwPollEvents();
		switch (state)
		{
		case(stateSplash):
			ShowSplashScreen();
			state = stateControls;
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
			else if (select == 7) { state = stateExiting; }
			break;
		case(stateControls):
			select = ShowControlsMenu();
			// Handle result.
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