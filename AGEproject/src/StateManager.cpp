#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include "UserControls.h"
#include "AudioEngine.h"
#include "LevelLoader.h"
#include <memory>

void StateManager::StateLoop()
{
	GameEngine::Get().Initialise();
	shader = ResourceHandler::Get().GetShader("Basic");
	bool running = true;
	int select;
	Game::Get().Initialise(); //This will need a new home later.

	AudioEngine::Get().LoadSound(ResourceHandler::Get().GetAudio("Theme"), false, true, false);
	AudioEngine::Get().LoadSound(ResourceHandler::Get().GetAudio("Advance"), false, false, false);
	AudioEngine::Get().LoadSound(ResourceHandler::Get().GetAudio("Return"), false, false, false);

	AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Theme"), glm::dvec3(0.0, 0.0, 0.0), -15.0f);

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
			if (select == 0)
			{
				AudioEngine::Get().StopAllChannels();
				AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Advance"), glm::dvec3(0.0, 0.0, 0.0), -12.0f);
				Game::Get().Initialise(); // This will need a new home later.
										  // Does it though?
				currentState = statePlaying;
			}
			else if (select == 1)
			{
				//AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Advance"), glm::dvec3(0.0, 0.0, 0.0), -12.0f);
				currentState = stateSettings;
			}
			else if (select == 2) { currentState = stateExiting; }
			break;
		case(stateSettings):
			select = ShowSettingsMenu();
			if (select == 0)
			{
				//AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Advance"), glm::dvec3(0.0, 0.0, 0.0), -12.0f);
				currentState = stateControls;
			}
			else if (select == 1)
			{
				//AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Advance"), glm::dvec3(0.0, 0.0, 0.0), -12.0f);
				GameEngine::Get().UpdateWindow();
			}
			else if (select == 2) 
			{ 
				//AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Return"), glm::dvec3(0.0, 0.0, 0.0), -12.0f);
				currentState = stateMainMenu; 
			}
			else if (select == 7) 
			{ 
				currentState = stateExiting; 
			}
			break;
		case(stateControls):
			select = ShowControlsMenu();
			if (select == 14)
			{
			//	AudioEngine::Get().PlaySoundOnThread(ResourceHandler::Get().GetAudio("Return"), glm::dvec3(0.0, 0.0, 0.0), -12.0f);
				currentState = stateSettings;
			}
			else if (select == 15)
			{
				currentState = stateExiting;
			}
			break;
		case(statePlaying):
			running = Game::Get().Update();
			Game::Get().Render();
			break;
		case(stateExiting):
			running = false;
			break;
		case statePause:
			ShowPauseScreen();
			currentState = statePlaying;
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + currentState);
			break;
		}
		AudioEngine::Get().Update();
	}
	GameEngine::Get().CleanUp();
}