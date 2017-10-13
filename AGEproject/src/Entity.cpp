#include "Entity.h"
#include "Renderable.h"
#include "Free_Camera.h"
#include "Structure.h"
#include "AirMovement.h"
#include <algorithm>
#include <iostream>

using namespace std;

//############## COMPONENT ##############

Component::Component(const string &token) : token(token)
{
	entity = nullptr;
	active = false;
}

Component *Component::MakeGeneric(const json &j) {
	const string ctype = j.at("component_type").get<string>();
	Component *cmp;
	cmp = nullptr;
	cmp->from_json(j);

	return cmp;
}

Component::~Component() {
	cout << "Goodbye from Component: " << token << endl;
	entity = nullptr;
	active = false;
	token.clear();
}

bool Component::IsActive() { return active; }

void Component::SetActive(bool b) { active = b; }

void Component::SetParent(Entity* p) { entity = p; }

Entity* Component::GetParent() const { return entity; }



//############## ENTITY ##############

Entity::Entity() { components.clear(); }

Entity::~Entity()
{
	components.clear();
	cout << "Goodbye from Entity: " << name << endl;
}

const string Entity::GetName() const { return name; }

void Entity::SetName(string const &name) { this->name = name; }

void Entity::Update(const double delta)
{
	UpdateTransforms();
	for (auto &c : components)
	{
		c.second->UpdateTransforms();
		c.second->SetTransform(GetTransform() * c.second->GetTransform());
		c.second->Update(delta);
	}
}

void Entity::Render() {
	for (auto &c : components) {
		c.second->Render();
	}
}

/*
void Entity::RemoveComponent(Component &c) {
  // Todo: Test This
  auto position =
	  find_if(components_.begin(), components_.end(), [c](unique_ptr<Component> &p) { return p.get() == &c; });
  if (position != components_.end()) {
	components_.erase(position);
  }
}
*/

/*
void Component::from_json(const nlohmann::json & j, Component & c)
{
const string ctype = j.at("component_type").get<string>();
if (ctype)
}
*/