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

void LevelLoader::EncodeEntity(Entity* entity, json &objects)
{
	json jEntity;
	if(entity->GetCompatibleComponent<Renderable>() != nullptr)
	{
		jEntity["Type"] = "Renderable";
		jEntity["Name"] = entity->GetName();

		auto tempRenderable = entity->GetCompatibleComponent<Renderable>();
		const auto tempMaterial = tempRenderable->GetMaterial();

		if(tempMaterial.diffuse != vec4(0))
		{
			auto d = tempMaterial.diffuse;
			jEntity["Diffuse"] = {d.r, d.g, d.b, d.w};
		}

		if (tempMaterial.emissive != vec4(0))
		{
			auto d = tempMaterial.emissive;
			jEntity["Emissive"] = { d.r, d.g, d.b, d.w };
		}


	}
	objects.push_back(jEntity);
}

void LevelLoader::LoadLevel(const std::string jsonFile, vector<Entity*> &playerEntities, vector<Entity*> &NPCEntities, vector<Entity*> &neutralEntities)
{
	std::ifstream ifs(jsonFile);
	json j = json::parse(ifs);

	vector<json> objects = j["Objects"];

	for(json object : objects)
	{
		Entity* tempEntity = new Entity;
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
				tempRenderable->SetModel(object["Model"]);
			if (object["Shader"] != "")
				tempRenderable->SetShader(object["Shader"]);
			if (object["Texture"] != "")
				tempRenderable->SetTexture(object["Texture"]);

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
				tempRenderable->SetPlane(values[0], values[1], values[2]);
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


			if(object["SpawnInfos"] != nullptr)
			{
				auto tempStructure = std::make_unique<Structure>();
				SpawnInfo si;
				json sp = object["SpawnInfos"];
				for(auto sis : sp)
				{
					si.buildTime = sis["BuildTime"];
					si.cost = sis["Cost"];
					const string t = sis["UnitType"]; // Can't implicitly convert to string
					si.unitType = t;
					tempStructure->AddSpawnInfo(si);
				}
				tempEntity->AddComponent(move(tempStructure));
			}

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

void LevelLoader::SaveLevel(const std::string jsonFile, vector<Entity*> &playerEntities, vector<Entity*> &NPCEntities, vector<Entity*> &neutralEntities)
{
	std::ofstream o(jsonFile);
	json j;

	json objects = json::array();

	for(auto pE : playerEntities)
	{
		EncodeEntity(pE, objects);
	}
	j["Objects"] = objects;

	o << std::setw(4) << j << std::endl;
}
