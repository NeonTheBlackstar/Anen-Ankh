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
public:
	vec3 position;

private:
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

class LineCollider2D
{
protected:
	float firstPositionX;
	float secondPositionX;
	float firstPositionY;
	float secondPositionY;

public:
	LineCollider2D() {};
	LineCollider2D(float firstPositionX, float secondPositionX, float firstPositionY, float secondPositionY);
	~LineCollider2D() {};
	bool DetectCollision(float pointPositionX, float pointPositionY);
};

//firstPositionX, secondPositionX, firstPositionY, secondPositionY
LineCollider2D::LineCollider2D(float firstPositionX, float secondPositionX, float firstPositionY, float secondPositionY)
{
	this->firstPositionX = firstPositionX;
	this->secondPositionX = secondPositionX;
	this->firstPositionY = firstPositionY;
	this->secondPositionY = secondPositionY;
}

bool LineCollider2D::DetectCollision(float pointPositionX, float pointPositionY)
{
	/*cout << "firstPositionX " << firstPositionX << " ";
	cout << "secondPositionX " << secondPositionX << " ";
	cout << "firstPositionY " << firstPositionY << " ";
	cout << "secondPositionY " << secondPositionY << " ";
	cout << "pointPositionX " << pointPositionX << " ";
	cout << "pointPositionY " << pointPositionY << endl;
	cout << firstPositionY << "-" << secondPositionY << "=" << firstPositionY - secondPositionY << endl;
	cout << firstPositionY - secondPositionY << "*" << pointPositionX << "=" << (firstPositionY - secondPositionY)*pointPositionX << endl;
	cout << firstPositionX << "-" << secondPositionX << "=" << firstPositionY - secondPositionY << endl;
	cout << firstPositionX - secondPositionX << "*" << pointPositionY << "=" << firstPositionY*(secondPositionY - firstPositionY) << endl;
	cout << firstPositionY*(secondPositionY - firstPositionY) << "+" << (firstPositionY - secondPositionY)*pointPositionX << "=" << (firstPositionY - secondPositionY)*pointPositionX + (firstPositionX - secondPositionX)*pointPositionY << endl << endl;
	cout << secondPositionY << "-" << firstPositionY << "=" << secondPositionY - firstPositionY << endl;
	cout << secondPositionX << "-" << firstPositionX << "=" << secondPositionX - firstPositionX << endl;
	cout << (secondPositionY - firstPositionY) << "/" << (secondPositionX - firstPositionX) << "=" << (secondPositionY - firstPositionY) / (secondPositionX - firstPositionX) << endl;
	cout << firstPositionX << "*" << firstPositionY << "*" << (secondPositionY - firstPositionY) / (secondPositionX - firstPositionX) << "=" << firstPositionY*((secondPositionY - firstPositionY) / (secondPositionX - firstPositionX))*firstPositionX << endl;
	*/

	/*cout << (firstPositionY - secondPositionY)*pointPositionX +
		(firstPositionX - secondPositionX)*pointPositionY +
		firstPositionY*((secondPositionY - firstPositionY) / (secondPositionX - firstPositionX))*firstPositionX*
		(secondPositionX - firstPositionX) << "  ";*/

	if ((firstPositionY - secondPositionY)*pointPositionX +
		(firstPositionX - secondPositionX)*pointPositionY +
		firstPositionY*((secondPositionY - firstPositionY) / (secondPositionX - firstPositionX))*firstPositionX*
		(secondPositionX - firstPositionX) < 0)
	{
		return true;
	}
	else
		return false;
}