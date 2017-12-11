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
	AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("noise"));

	while (running)
	{
		glfwPollEvents();
		switch (currentState)
		{
		case(stateSplash):
			ShowSplashScreen();
			currentState = stateMainMenu;
			break;
		case(stateMainMenu):
			select = ShowMainMenu();
			AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("noise"));
			if (select == 0)
			{
				Game::Get().Initialise(); //This will need a new home later.
				currentState = statePlaying;
			}
			else if (select == 1)
			{
				currentState = stateSettings;
			}
			else if (select == 2) { currentState = stateExiting; }
			break;
		case(stateSettings):
			select = ShowSettingsMenu();
			AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("noise"));
			if (select == 1)
			{
				GameEngine::Get().UpdateWindow();
				//currentState = stateMainMenu;
			}
			else if (select == 2) 
			{ 
				currentState = stateMainMenu; 
			}
			else if (select == 0)
			{
				currentState = stateControls;
			}
			else if (select == 7) 
			{ 
				currentState = stateExiting; 
			}
			break;
		case(stateControls):
			select = ShowControlsMenu();
			AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("noise"));
			if (select == 14)
			{
				currentState = stateExiting;
			}
			else
			{
				currentState = stateSettings;
			}
			break;
		case(statePlaying):
			running = Game::Get().Update();
			Game::Get().Render();
			break;
		case(stateExiting):
			running = false;
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + currentState);
			break;
		}
		AudioEngine::Get().Update();
	}
	GameEngine::Get().CleanUp();
}