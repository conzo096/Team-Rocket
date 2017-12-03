#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include "UserControls.h"
#include "AudioEngine.h"
#include <memory>

void StateManager::StateLoop()
{
	GameEngine::Get().Initialise();
	bool running = true;
	int select;
	AudioEngine::Get().LoadSound(ResourceHandler::Get().GetAudio("noise"), false, false, false);
	while (running)
	{
		glfwPollEvents();
		switch (state)
		{
		case(stateSplash):
			AudioEngine::Get().PlaySound(ResourceHandler::Get().GetAudio("noise"));
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
			AudioEngine::Get().PlaySound(ResourceHandler::Get().GetAudio("noise"));
			select = ShowSettingsMenu();
			if (select == 1)
			{
				GameEngine::Get().UpdateWindow();
				state = stateMainMenu;
			}
			else if (select == 2) 
			{ 
				state = stateMainMenu; 
			}
			else if (select == 0)
			{
				//state = stateControls;
			}
			else if (select == 7) 
			{ 
				state = stateExiting; 
			}
			break;
		case(stateControls):
			AudioEngine::Get().PlaySound(ResourceHandler::Get().GetAudio("noise"));
			select = ShowControlsMenu();
			// Handle result.
			state = stateMainMenu;
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
		AudioEngine::Get().Update();
	}
	GameEngine::Get().CleanUp();
}