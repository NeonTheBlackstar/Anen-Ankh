#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;

class CubeCollider3D
{
private:
	vec3 position;
	//Przesuniêcie wzglêdem centrum rodzica
	vec3 difference;
	vec3 size;

public:
	CubeCollider3D() {};
	CubeCollider3D(vec3 position, vec3 difference, vec3 size);
	~CubeCollider3D() {};
	bool DetectCollision(CubeCollider3D boxCollider);
	void ChangePositionOfCenter(vec3 positionOfObject);
};

CubeCollider3D::CubeCollider3D(vec3 position, vec3 difference, vec3 size)
{
	this->position = position;
	this->difference = difference;
	this->size = size;
}

void CubeCollider3D::ChangePositionOfCenter(vec3 positionOfObject)
{
	position = vec3(difference.x + positionOfObject.x, difference.y + positionOfObject.y, difference.z + positionOfObject.z);
}

bool CubeCollider3D::DetectCollision(CubeCollider3D boxCollider)
{
	if ((((fabs(position.x - boxCollider.position.x)) <= size.x / 2 + boxCollider.size.x / 2) ||
		((fabs(boxCollider.position.x - position.x)) <= size.x / 2 + boxCollider.size.x / 2)) &&
		(((fabs(position.y - boxCollider.position.y)) <= size.y / 2 + boxCollider.size.y / 2) ||
		((fabs(boxCollider.position.y - position.y)) <= size.y / 2 + boxCollider.size.y / 2)) &&
		(((fabs(position.z - boxCollider.position.z)) <= size.z / 2 + boxCollider.size.z / 2) ||
		((fabs(boxCollider.position.z - position.z)) <= size.z / 2 + boxCollider.size.z / 2)))
		return true;
	else
		return false;
}
