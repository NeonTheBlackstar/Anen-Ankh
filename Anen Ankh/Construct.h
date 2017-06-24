#pragma once

#include "Collider.h"
#include "Texture.h"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "Programmed models/Cube.h"
#include "Programmed models/Stairs.h"

class Construct
{
public:
	static enum figureType
	{
		cube, stairs
	};
	CubeCollider3D cubeCollider;
	LineCollider2D lineCollider;

private:
	Texture texture = *(new Texture(""));
	mat4 modelMatrix;
	mat4 parentMatrix;
	string name;
	int selectedFigureType;
	vec3 position;
	vec3 size;
	float rotationAngle;
	vec3 rotationAxis;

public:
	Construct(string name, int selectedFigureType, mat4 parentMatrix, Texture texture, vec3 position, vec3 size, vec3 rotationAxis, float rotationAngle);
	~Construct() {};
	void SetCubeCollider(vec3 positionOfCenter, vec3 difference, vec3 size);
	void SetLineCollider(float firstPositionX, float secondPositionX, float firstPositionY, float secondPositionY);
	void DrawSolid(mat4 playerPosition);
};

Construct::Construct(string name, int selectedFigureType, mat4 parentMatrix, Texture texture = *(new Texture("")), vec3 position = vec3(0, 0, 0), vec3 size = vec3(1, 1, 1), vec3 rotationAxis = vec3(1, 1, 1), float rotationAngle = 0)
{
	this->selectedFigureType = selectedFigureType;
	this->parentMatrix = parentMatrix;
	this->texture = texture;
	this->modelMatrix = parentMatrix;
	this->position = position;//translate(modelMatrix, position);
	this->size = size;//scale(modelMatrix, size);
	this->rotationAngle = rotationAngle;//rotate(modelMatrix, rotationAngle, rotationAxis);
	this->rotationAxis = rotationAxis;
}

void Construct::SetCubeCollider(vec3 positionOfCenter, vec3 difference, vec3 size)
{
	cubeCollider = *(new CubeCollider3D(positionOfCenter, difference, size));
}

void Construct::SetLineCollider(float firstPositionX, float secondPositionX, float firstPositionY, float secondPositionY)
{
	lineCollider = *(new LineCollider2D(firstPositionX, secondPositionX, firstPositionY, secondPositionY));
}

void Construct::DrawSolid(mat4 playerPosition)
{
	mat4 M = mat4(1);

	switch (selectedFigureType)
	{
	case cube:
	    M = playerPosition*modelMatrix;
		M = rotate(M, rotationAngle, rotationAxis);
		M = translate(M, position);
		M = scale(M, size);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(M));

		glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
		glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych

		glVertexPointer(3, GL_FLOAT, 0, myCubeVertices); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
		glTexCoordPointer(2, GL_FLOAT, 0, myCubeTexCoords); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
		glNormalPointer(GL_FLOAT, 0, myCubeVertices); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
		glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Rysuj model
													  //Posprz¹taj po sobie
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		break;

	case stairs:
		M = playerPosition*modelMatrix;
		M = rotate(M, rotationAngle, rotationAxis);
		M = translate(M, position);
		M = scale(M, size);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(M));

		glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
		//glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych

		glVertexPointer(3, GL_FLOAT, 0, stairsVertices); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
		glTexCoordPointer(2, GL_FLOAT, 0, stairsTexCoords); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
		//glNormalPointer(GL_FLOAT, 0, stairsVertices); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
		glDrawArrays(GL_QUADS, 0, stairsVertexCount); //Rysuj model
													  //Posprz¹taj po sobie
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_NORMAL_ARRAY);

		break;
	}
}
