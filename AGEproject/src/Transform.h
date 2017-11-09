#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
private:
	bool changed;
	glm::dvec3 position;
	glm::dquat rotation;
	glm::dvec3 scale;
	glm::dmat4 transform;

public:
	Transform() : scale(glm::dvec3(1.0f)), rotation(glm::dquat()), position(glm::dvec3()) {}

	Transform(glm::dvec3 pos, glm::dvec3 rot, glm::dvec3 scale) : Transform()
	{
		SetPosition(pos);
		SetRotation(rot);
		SetScale(scale);
		changed = true;
	}

	~Transform() {}

	void UpdateTransforms()
	{
		if (changed)
		{
			transform = glm::translate(position) * mat4_cast(rotation) * glm::scale(scale);
			changed = false;
		}
	}
	const bool GetChanged() const { return changed; }

	const glm::dvec3 GetPosition() const { return position; }
	void SetPosition(const glm::dvec3 &v3) { position = v3;  changed = true; }
	void Move(const glm::dvec3 &v3) { SetPosition(position + v3); changed = true; }

	const glm::dquat GetRotation() const { return rotation; }
	//const glm::dvec3 GetRotation() const { }
	void SetRotation(const glm::dquat &q) { rotation = q;  changed = true; }
	void SetRotation(const glm::dvec3 &v3) { rotation = glm::dquat(v3); changed = true;  }
	void Rotate(const glm::dquat &q) { SetRotation(rotation * q); changed = true; }
	
	void Rotate(const glm::dvec3 &v3)
	{
		SetRotation(rotation * glm::dquat(glm::radians(v3))); changed = true;
	}

	const glm::dvec3 GetScale() const { return scale; }
	void SetScale(const glm::dvec3 &v3) { scale = v3;  changed = true; }
	void Scale(const glm::dvec3 &v3) { scale *= v3; changed = true; }

	const glm::dmat4 GetTransform() const { return transform; }
	void SetTransform(const glm::dmat4 m4) { transform = m4; changed = true; }
};