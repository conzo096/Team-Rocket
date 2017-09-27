#include "StateManager.h"
#include "Entity.h"
#include "Renderable.h"
#include <memory>
StateManager *StateManager::instance = 0;

void StateManager::StateLoop()
{
	GameEngine::Instance()->Initialise();

	Entity tempEntitiy;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetMesh("../res/models/Torus2.obj");
	tempRenderable->SetEffect();
	tempEntitiy.AddComponent(move(tempRenderable));

	while (!glfwWindowShouldClose(GameEngine::Instance()->GetWindow()))
	{
		switch (state)
		{
		case Splash:
			GameEngine::Instance()->Render();
			break;
		case Menu:
			break;
		case Settings:
			break;
		case Playing:
			break;
		case Exiting:
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
		}
	}
	GameEngine::Instance()->CleanUp();
}
