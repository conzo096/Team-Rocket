#include "LevelLoader.h"
#include "PointLight.h"


void LevelLoader::SetMovement(json j, Movement &m)
{
	if (j["Goal"] != nullptr)
	{
		json goal = j["Goal"];
		std::vector<float> g;
		for (const auto elem : goal)
		{
			g.push_back(elem);
		}
		m.SetGoal(glm::dvec3(g[0], g[1], g[2]));
	}

	// Destination is the current way point - no need to save
	/*
	if (j["Destination"] != nullptr)
	{
		json destination = j["Destination"];
		std::vector<float> d;
		for (const auto elem : destination)
		{
			d.push_back(elem);
		}
		m.SetDestination(glm::dvec3(d[0], d[1], d[2]));
	}
	*/

	if (j["Speed"] != "")
		m.SetSpeed(j["Speed"]);

	if (j["Acceleration"] != "")
		m.SetAcceleration(j["Acceleration"]);

	if (j["TurnSpeed"] != "")
		m.SetTurnSpeed(j["TurnSpeed"]);
}

Team LevelLoader::GetTeam(const string team)
{
	if (team == "Player")
		return player;
	if (team == "NPC")
		return ai;
	if (team == "Neutral")
		return neutral;
	return neutral;
}

void LevelLoader::EncodeEntity(std::shared_ptr<Entity> entity, json &objects, string entityList)
{
	json jEntity;
	if(entity->GetCompatibleComponent<Renderable>() != nullptr)
	{
		jEntity["Type"] = "Renderable";
		jEntity["Name"] = entity->GetName();
		jEntity["EntityList"] = entityList;

		auto tempRenderable = entity->GetCompatibleComponent<Renderable>();
		const auto tempMaterial = tempRenderable->GetMaterial();

		jEntity["Model"] = entity->modelName;
		jEntity["Shader"] = entity->shaderName;
		jEntity["Texture"] = entity->textureName;

		auto p = tempRenderable->GetPosition();
		jEntity["Position"] = { p.x, p.y, p.z };

		if(tempMaterial.diffuse != vec4(0) && entity->GetCompatibleComponent<Resource>() != nullptr)
		{
			auto d = tempMaterial.diffuse;
			jEntity["Diffuse"] = {d.r, d.g, d.b, d.w};
		}

		if (tempMaterial.emissive != vec4(0) && entity->GetCompatibleComponent<BoundingBox>() != nullptr)
		{
			auto d = tempMaterial.emissive;
			jEntity["Emissive"] = { d.r, d.g, d.b, d.w };
		}

		if (entity->GetCompatibleComponent<Ship>() != nullptr)
			jEntity["TypeCode"] = "Ship";
		else if (entity->GetCompatibleComponent<Troop>() != nullptr)
			jEntity["TypeCode"] = "Troop";
		else if (entity->GetCompatibleComponent<Tank>() != nullptr)
			jEntity["TypeCode"] = "Tank";
		else if (entity->GetCompatibleComponent<Worker>() != nullptr)
			jEntity["TypeCode"] = "Worker";
		else if (entity->GetCompatibleComponent<Shipyard>() != nullptr)
			jEntity["TypeCode"] = "Shipyard";
		else if (entity->GetCompatibleComponent<Resource>() != nullptr)
			jEntity["TypeCode"] = "Resource";
		else if (entity->GetCompatibleComponent<Barracks>() != nullptr)
			jEntity["TypeCode"] = "Barracks";

		if(entity->GetCompatibleComponent<Movement>() != nullptr)
		{
			json mov;

			if (entity->GetCompatibleComponent<GroundMovement>() != nullptr)
				mov["Type"] = "Ground";
			else if (entity->GetCompatibleComponent<AirMovement>() != nullptr)
				mov["Type"] = "Air";

			auto movComp = entity->GetCompatibleComponent<Movement>();
			auto g = movComp->GetGoal();
			mov["Goal"] = { g.x, g.y, g.z };
			mov["Speed"] = movComp->GetSpeed();
			mov["Acceleration"] = movComp->GetAcceleration();
			mov["TurnSpeed"] = movComp->GetTurnSpeed();
			
			jEntity["Movement"] = mov;
		}

		// If it's a plane (eveything else has a bounding sphere)
		if(entity->GetCompatibleComponent<BoundingBox>() != nullptr)
		{
			auto dims = tempRenderable->planeDimensions;
			jEntity["Plane"] = { dims[0], dims[1], dims[2] };
		}

		auto structure = entity->GetCompatibleComponent<Structure>();
			
		if(structure != nullptr)
		{
			json spawnInfos = json::array();

			auto si = structure->GetSpawnInfo();

			for(auto s : si)
			{
				json sEntity;
				sEntity["BuildTime"] = s.buildTime;
				sEntity["Cost"] = s.cost;
				sEntity["UnitType"] = s.unitType;
				spawnInfos.push_back(sEntity);
			}
			jEntity["SpawnInfos"] = spawnInfos;
		}
	}
	else if(entity->GetCompatibleComponent<PointLight>() != nullptr)
	{
		jEntity["Type"] = "Light";

		auto pL = entity->GetCompatibleComponent<PointLight>();

		jEntity["Shader"] = pL->GetEffect();

		auto p = pL->GetPosition();
		jEntity["Position"] = { p.x, p.y, p.z };

		auto c = pL->diffuse;
		jEntity["Colour"] = { c.r, c.g, c.b, c.w };
	}
	objects.push_back(jEntity);
}

void LevelLoader::LoadLevel(const std::string jsonFile, vector<std::shared_ptr<Entity>> &playerEntities, vector<std::shared_ptr<Entity>> &NPCEntities, vector<std::shared_ptr<Entity>> &neutralEntities, Player* player)
{
	std::ifstream ifs(jsonFile);
	json j = json::parse(ifs);

	player->SetBalance(j["Balance"]);

	vector<json> objects = j["Objects"];

	for(json object : objects)
	{
		std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
		if(object["Type"] == "Renderable")
		{
			tempEntity->SetName(object["Name"]);

			auto tempRenderable = std::make_unique<Renderable>();
			tempRenderable->SetMaterial(new Material());

			if(object["Diffuse"] != nullptr)
			{
				json dif = object["Diffuse"];
				vector<float> comps;
				for(const auto d : dif)
				{
					comps.push_back(d);
				}
				tempRenderable->GetMaterial().diffuse = glm::vec4(comps[0], comps[1], comps[2], comps[3]);
			}
			if(object["Emissive"] != nullptr)
			{
				json emi = object["Emissive"];
				vector<float> comps;
				for(const auto e : emi)
				{
					comps.push_back(e);
				}
				tempRenderable->GetMaterial().emissive = glm::vec4(comps[0], comps[1], comps[2], comps[3]);
			}

			if (object["Model"] != "")
			{
				const string s = object["Model"];
				tempEntity->modelName = s;
				tempRenderable->SetModel(s);
			}
			if (object["Shader"] != "")
			{
				const string s = object["Shader"];
				tempEntity->shaderName = s;
				tempRenderable->SetShader(s);
			}
			if (object["Texture"] != "")
			{
				const string s = object["Texture"];
				tempEntity->textureName = s;
				tempRenderable->SetTexture(s);
			}

			json pos = object["Position"];
			vector<float> positions;
			for(const auto elem : pos)
			{
				positions.push_back(elem);
			}
			tempRenderable->SetPosition(glm::dvec3(positions[0], positions[1], positions[2]));
			tempEntity->SetPosition(tempRenderable->GetPosition());
			tempRenderable->UpdateTransforms();
			// tempEntity has tempRenderable added later as it is still needed for the bounding sphere

			json movement = object["Movement"];
			if(movement != nullptr)
			{
				if (movement["Type"] == "Ground")
				{
					auto mov = std::make_unique<GroundMovement>();
					SetMovement(movement, *mov);
					tempEntity->AddComponent(move(mov));
				}
				else if(movement["Type"] == "Air")
				{
					auto mov = std::make_unique<AirMovement>();
					SetMovement(movement, *mov);
					tempEntity->AddComponent(move(mov));
				}
			}

			if(object["Plane"] != nullptr)
			{
				json pl = object["Plane"];
				vector<float> values;
				for(const auto p : pl)
				{
					values.push_back(p);
				}
				tempRenderable->SetPlane(static_cast<unsigned int>(values[0]), static_cast<unsigned int>(values[1]), static_cast<unsigned int>(values[2]));
				auto tempBoundingBox = std::make_unique<BoundingBox>();
				// ReSharper disable once CppMsExtBindingRValueToLvalueReference
				tempBoundingBox->SetUpBoundingBox(tempRenderable->GetModel().GetVertexPositions());
				tempEntity->AddComponent(move(tempBoundingBox));
			}
			else
			{
				auto tempBoundingSphere = std::make_unique<BoundingSphere>();
				// ReSharper disable once CppMsExtBindingRValueToLvalueReference
				tempBoundingSphere->SetUpBoundingSphere(tempRenderable->GetModel().GetVertexPositions());
				tempEntity->AddComponent(move(tempBoundingSphere));
			}
			tempEntity->AddComponent(move(tempRenderable));




			auto target = std::make_unique<Targetable>();
			target->SetHealth(100);
			tempEntity->AddComponent(move(target));

			if(object["TypeCode"] == "Ship")
			{
				auto tempUnit = std::make_unique<Ship>();
				tempUnit->SetTeam(GetTeam(object["EntityList"]));
				tempEntity->AddComponent(move(tempUnit));
			}
			else if(object["TypeCode"] == "Troop")
			{
				auto tempUnit = std::make_unique<Troop>();
				tempUnit->SetTeam(GetTeam(object["EntityList"]));
				tempEntity->AddComponent(move(tempUnit));
			}
			else if(object["TypeCode"] == "Tank")
			{
				auto tempUnit = std::make_unique<Tank>();
				tempUnit->SetTeam(GetTeam(object["EntityList"]));
				tempEntity->AddComponent(move(tempUnit));
			}
			else if(object["TypeCode"] == "Worker")
			{
				auto tempUnit = std::make_unique<Worker>();
				tempUnit->SetTeam(GetTeam(object["EntityList"]));
				tempEntity->AddComponent(move(tempUnit));
			}
			else if(object["TypeCode"] == "Shipyard")
			{
				auto tempUnit = std::make_unique<Shipyard>();
				tempUnit->SetTeam(GetTeam(object["EntityList"]));
				tempEntity->AddComponent(move(tempUnit));
			}
			else if(object["TypeCode"] == "Resource")
			{
				auto tempUnit = std::make_unique<Resource>();
			//	Resources don't have a team
				tempEntity->AddComponent(move(tempUnit));
			}
			else if(object["TypeCode"] == "Barracks")
			{
				auto tempUnit = std::make_unique<Barracks>();
				tempUnit->SetTeam(GetTeam(object["EntityList"]));
				tempEntity->AddComponent(move(tempUnit));
			}

			if (object["SpawnInfos"] != nullptr)
			{
				Structure* tempStructure = tempEntity->GetCompatibleComponent<Structure>();
				if(tempStructure != nullptr)
				{
					SpawnInfo si;
					json sp = object["SpawnInfos"];
					for (auto sis : sp)
					{
						si.buildTime = sis["BuildTime"];
						si.cost = sis["Cost"];
						const string t = sis["UnitType"]; // Can't implicitly convert to string
						si.unitType = t;
						tempStructure->AddSpawnInfo(si);
					}
				}
				else
				{
					auto tempStructure2 = std::make_unique<Structure>();
					SpawnInfo si;
					json sp = object["SpawnInfos"];
					for (auto sis : sp)
					{
						si.buildTime = sis["BuildTime"];
						si.cost = sis["Cost"];
						const string t = sis["UnitType"]; // Can't implicitly convert to string
						si.unitType = t;
						tempStructure2->AddSpawnInfo(si);
					}
					tempEntity->AddComponent(move(tempStructure2));
				}
			}

			if(object["EntityList"] == "Player")
			{
				playerEntities.push_back(tempEntity);
			}
			else if (object["EntityList"] == "NPC")
			{
				NPCEntities.push_back(tempEntity);
			}
			else if (object["EntityList"] == "Neutral")
			{
				neutralEntities.push_back(tempEntity);
			}
		}
		else if(object["Type"] == "Light")
		{
			auto pL = std::make_unique<PointLight>();
			pL->SetEffect(object["Shader"]);

			json pos = object["Position"];
			std::vector<float> p;
			for (const auto elem : pos)
			{
				p.push_back(elem);
			}
			pL->setLightPosition(glm::vec3(p[0], p[1], p[2]));

			json dif = object["Colour"];
			std::vector<float> d;
			for (const auto elem : dif)
			{
				d.push_back(elem);
			}
			pL->diffuse = glm::vec4(d[0], d[1], d[2], d[3]);

			pL->ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.00f);
			pL->specular = glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);

			tempEntity->AddComponent(move(pL));
			neutralEntities.push_back(tempEntity);
		}
	}

}

void LevelLoader::SaveLevel(const std::string jsonFile, vector<std::shared_ptr<Entity>> &playerEntities, vector<std::shared_ptr<Entity>> &NPCEntities, vector<std::shared_ptr<Entity>> &neutralEntities, int balance)
{
	std::ofstream o(jsonFile);
	json j;

	j["Balance"] = balance;

	json objects = json::array();

	for(auto pE : playerEntities)
	{
		EncodeEntity(pE, objects, "Player");
	}
	for (auto pE : NPCEntities)
	{
		EncodeEntity(pE, objects, "NPC");
	}
	for (auto pE : neutralEntities)
	{
		EncodeEntity(pE, objects, "Neutral");
	}

	j["Objects"] = objects;

	o << std::setw(4) << j << std::endl;
}
