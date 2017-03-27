#pragma once

#include "Collider.h"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glut.h"

class Construct
{
public:
	static enum figureType
	{
		cube
	};
	CubeCollider3D collider;

private:
	mat4 modelMatrix;
	mat4 parentMatrix;
	string name;
	int selectedFigureType;

public:
	Construct(string name, int selectedFigureType, mat4 parentMatrix, vec3 position, vec3 size, vec3 rotationAxis, float rotationAngle);
	~Construct() {};
	void SetCollider(vec3 positionOfCenter, vec3 difference, vec3 size);
	void DrawSolid(mat4 playerPosition);
};

Construct::Construct(string name, int selectedFigureType, mat4 parentMatrix, vec3 position = vec3(0, 0, 0), vec3 size = vec3(1, 1, 1), vec3 rotationAxis = vec3(1, 1, 1), float rotationAngle = 0)
{
	this->selectedFigureType = selectedFigureType;
	this->parentMatrix = parentMatrix;
	modelMatrix = parentMatrix;
	modelMatrix = translate(modelMatrix, position);
	modelMatrix = scale(modelMatrix, size);
	modelMatrix = rotate(modelMatrix, rotationAngle, rotationAxis);
}

void Construct::SetCollider(vec3 positionOfCenter, vec3 difference, vec3 size)
{
	collider = *(new CubeCollider3D(positionOfCenter, difference, size));
}

void Construct::DrawSolid(mat4 playerPosition)
{
	switch (selectedFigureType)
	{
	case cube:
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(playerPosition*modelMatrix));
		glutSolidCube(1);
		break;
	}
}
