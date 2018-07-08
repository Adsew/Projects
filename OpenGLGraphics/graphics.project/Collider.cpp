#include "Core.h"

#include "Collider.h"


IMPLEMENT_DYNAMIC_CLASS(Collider);


void Collider::initialize()
{

}

void Collider::update(float _deltaTime)
{
	if (getGameObject()->transform()->isDirty())
	{
		glm::vec3 colliderPos = max - (max - min) / 2.0f;
		glm::vec3 offset = colliderPos - getGameObject()->transform()->position();
		max -= offset;
		min -= offset;
	}
}

void Collider::load(XMLElement* element)
{
	Component::load(element);
	float width = 0, height = 0;

	XMLElement* data = element->FirstChildElement("width");
	if (data != nullptr)
	{
		data->QueryFloatText(&width);
	}

	data = element->FirstChildElement("height");
	if (data != nullptr)
	{
		data->QueryFloatText(&height);
	}

	min = glm::vec3(-width / 2, -height / 2, -width / 2);
	max = glm::vec3(width / 2, height / 2, width / 2);

	update(0);
}

//Check collision
bool Collider::collides(std::shared_ptr<Collider> &other)
{
	if ((min.x < other->max.x && max.x > other->min.x)
		&& (min.y < other->max.y && max.y > other->min.y)
		&& (min.z < other->max.z && max.z > other->min.z))
	{
		return true;
	}

	return false;
}

bool Collider::collides(Raycast &ray)
{
	bool xInter = false, yInter = false, zInter = false;
	glm::vec3 rayMax;
	glm::vec3 rayMin;

	rayMax.x = ray.origin.x > ray.end.x ? ray.origin.x : ray.end.x;
	rayMax.y = ray.origin.y > ray.end.y ? ray.origin.y : ray.end.y;
	rayMax.z = ray.origin.z > ray.end.z ? ray.origin.z : ray.end.z;
	rayMin.x = ray.origin.x < ray.end.x ? ray.origin.x : ray.end.x;
	rayMin.y = ray.origin.y < ray.end.y ? ray.origin.y : ray.end.y;
	rayMin.z = ray.origin.z < ray.end.z ? ray.origin.z : ray.end.z;

	if ((rayMax.x > min.x && rayMin.x < min.x)
		|| (rayMax.x > max.x && rayMin.x < max.x)
		|| (rayMax.x > min.x && rayMax.x < max.x && rayMin.x > min.x && rayMin.x < max.x))
	{
		xInter = true;
	}
	if ((rayMax.y > min.y && rayMin.y < min.y)
		|| (rayMax.y > max.y && rayMin.y < max.y)
		|| (rayMax.y > min.y && rayMax.y < max.y && rayMin.y > min.y && rayMin.y < max.y))
	{
		yInter = true;
	}
	if ((rayMax.z > min.z && rayMin.z < min.z)
		|| (rayMax.z > max.z && rayMin.z < max.z)
		|| (rayMax.z > min.z && rayMax.z < max.z && rayMin.z > min.z && rayMin.z < max.z))
	{
		zInter = true;
	}

	return xInter && yInter && zInter;
}