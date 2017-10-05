#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include "Menu.h"
#include <memory>
StateManager *StateManager::instance = 0;

void StateManager::StateLoop()
{
	GameEngine::Instance()->Initialise();
	Game::Instance()->Initialise();
	while (!glfwWindowShouldClose(GameEngine::Instance()->GetWindow()))
	{
		std::vector<char*> texs;
		texs.push_back("../res/textures/debug.png");
		texs.push_back("../res/textures/debug.png");
		texs.push_back("../res/textures/debug.png");
		switch (state)
		{
		case(Splash):
			TMenu(texs).Draw(Shader::GetShader(std::string("Basic")));
			state = Menu;
			break;
		case(Menu):
			//GameEngine::Instance()->Render();
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
