#include "Game.h"
#include "Free_Camera.h"

Game *Game::instance = 0;
std::vector<Entity*> Game::entities;
double Game::lastTime = 0.0f;
Entity Game::cam = Entity();

void Game::Initialise()
{
	auto camera = std::make_unique<Free_Camera>((GameEngine::Instance()->GetScreenWidth() / GameEngine::Instance()->GetScreenHeight()), 90.0f);
	camera->SetPosition(glm::dvec3(10.0, 5.0, 20.0));
	camera->SetProjection(2.414f, 1000);
	cam.AddComponent(move(camera));

	Entity* tempEntity = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("../res/models/Constructor.obj");
	tempRenderable->SetEffect("ConstructorUV");
	tempRenderable->SetPosition(glm::vec3(3.5f, 2.5f, 3.5f));
	tempRenderable->UpdateTransforms();
	tempEntity->AddComponent(move(tempRenderable));

	entities.push_back(tempEntity);

	Entity* tempEntity2 = new Entity;
	auto tempRenderable2 = std::make_unique<Renderable>();
	tempRenderable2->SetPlane(1, 100, 100);
	tempRenderable2->SetEffect("debug");
	tempRenderable2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable2->UpdateTransforms();
	tempEntity2->AddComponent(move(tempRenderable2));

	entities.push_back(tempEntity2);
}

void Game::Update()
{
	glm::mat4 camMatrix = cam.GetComponent<Free_Camera>().GetProjection() * cam.GetComponent<Free_Camera>().GetView();
	GameEngine::Instance()->SetCamera(camMatrix);
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	cam.Update(deltaTime);
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		//entities[n]->Rotate(glm::vec3(0.01f, 0.01f, 0.0f));
		entities[n]->UpdateTransforms();
		entities[n]->Update(deltaTime);
		n++;
	}

	printf("%.9f\n", deltaTime);
}

void Game::Render()
{
	// Clear the opengl buffer.
	glClearColor(0.1, 0.0, 0.4, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_CULL_FACE);

	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		entities[n]->Render();
		n++;
	}

	// process events.
	glfwPollEvents();
	// Swap the window buffers.
	glfwSwapBuffers(GameEngine::Instance()->GetWindow());
}
