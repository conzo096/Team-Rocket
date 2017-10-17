#include "Game.h"
#include "Entity.h"

void Game::SpawnUnit(glm::vec3 position, glm::vec2 size)
{
	for (int i = 0; i < 1; i++)
	{
		glm::vec3 spawnPosition = position;
		spawnPosition.y = 15.0;
		if (i % 2 == 0)
		{
			spawnPosition.x += size.x;
			size.x = -size.x;
			size.y = -size.y;
		}
		else
			spawnPosition.z += size.y;
		Entity* tempFlyer = new Entity;
		auto tempRenderable = std::make_unique<Renderable>();
		tempRenderable->SetModel("../res/models/torus2.obj");
		tempRenderable->SetScale(glm::vec3(0.1));
		tempRenderable->SetEffect("Debug");
		tempFlyer->SetPosition(spawnPosition);
		tempRenderable->UpdateTransforms();
		auto tempAirMovement = std::make_unique<AirMovement>();
		tempAirMovement->SetDestination(glm::dvec3(20, 15, 20));
		tempAirMovement->SetSpeed(30.0);
		tempAirMovement->SetAcceleration(0.5);
		tempAirMovement->SetTurnSpeed(180.0);

		tempFlyer->AddComponent(move(tempRenderable));
		tempFlyer->AddComponent(move(tempAirMovement));
		entities.push_back(tempFlyer);
	}
}

void Game::Initialise()
{

	free_cam = new Entity;
	auto cam = std::make_unique<Free_Camera>(glm::half_pi<float>());
	cam->SetPosition(glm::dvec3(10.0, 5.0, 50.0));
	cam->SetProjection((GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);
	free_cam->AddComponent(move(cam));

	Entity* tempEntity = new Entity;
	auto tempRenderable = std::make_unique<Renderable>();
	tempRenderable->SetModel("../res/models/Constructor.obj");
	tempRenderable->SetEffect("ConstructorUV");
	tempEntity->SetPosition(glm::vec3(3.5f, 2.5f, 3.5f));
	tempRenderable->UpdateTransforms();
	auto tempStructure = std::make_unique<Structure>();

//	auto tempBoundingBox = std::make_unique<BoundingSphere>();
//	tempBoundingBox->UpdateTransforms();

//	tempBoundingBox->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
	tempEntity->AddComponent(move(tempRenderable));
//	tempEntity->AddComponent(move(tempBoundingBox));
	tempEntity->AddComponent(move(tempStructure));

	entities.push_back(tempEntity);

	Entity* tempEntity2 = new Entity;
	auto tempRenderable2 = std::make_unique<Renderable>();
	tempRenderable2->SetPlane(1, 100, 100);
	tempRenderable2->SetEffect("debug");
	tempEntity2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	tempRenderable2->UpdateTransforms();

	auto tempBoundingBox2 = std::make_unique<BoundingBox>();
	tempBoundingBox2->SetUpBoundingBox(tempRenderable2->GetModel().GetVertexPositions());

	tempEntity2->AddComponent(move(tempRenderable2));
	tempEntity2->AddComponent(move(tempBoundingBox2));
	entities.push_back(tempEntity2);
	lastTime = clock();
}

void Game::Update()
{
	glm::mat4 camMatrix = free_cam->GetComponent<Free_Camera>().GetProjection() * free_cam->GetComponent<Free_Camera>().GetView();
	GameEngine::Get().SetCamera(camMatrix);
	double deltaTime = (clock() - lastTime) / CLOCKS_PER_SEC;
	lastTime = clock();
	free_cam->Update(deltaTime);
	for (std::vector<Entity*>::size_type n = 0; n < entities.size();)
	{
		//entities[n]->Rotate(glm::vec3(0.01f, 0.01f, 0.0f));
		entities[n]->Update(deltaTime);
		n++;
	}

	//printf("%.9f\n", deltaTime);
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
	glfwSwapBuffers(GameEngine::Get().GetWindow());
}