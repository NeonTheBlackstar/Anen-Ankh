#pragma once

//Jeœli tylko cokolwiek dotyczy kamery - powinno znaleŸæ siê tutaj
//A w innych klasach byæ tylko wywo³ywane

class Camera
{
public:
	mat4 positionMatrix;
	vec3 positionVector;

public:
	Camera() {};
	~Camera() {};
	void FallDown();
	void GoUp();
	void GoFoward();
	void GoBack();
	void GoLeft();
	void GoRight();
};

void Camera::GoFoward()
{
	positionVector = vec3(positionVector.x, positionVector.y, positionVector.z - 0.5f);

	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z),
		vec3(positionVector.x, positionVector.y, positionVector.z - 1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
}

void Camera::GoBack()
{
	positionVector = vec3(positionVector.x, positionVector.y, positionVector.z + 0.5f);

	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z),
		vec3(positionVector.x, positionVector.y, positionVector.z - 1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
}

void Camera::GoLeft()
{
	positionVector = vec3(positionVector.x - 0.5f, positionVector.y, positionVector.z);

	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z),
		vec3(positionVector.x, positionVector.y, positionVector.z - 1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
}

void Camera::GoRight()
{
	positionVector = vec3(positionVector.x + 0.5f, positionVector.y, positionVector.z);

	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z),
		vec3(positionVector.x, positionVector.y, positionVector.z - 1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
}

void Camera::FallDown()
{
	positionVector = vec3(positionVector.x, positionVector.y + 0.5f, positionVector.z);

	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z),
		vec3(positionVector.x, positionVector.y, positionVector.z-1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
}

void Camera::GoUp()
{
	positionVector = vec3(positionVector.x, positionVector.y - 0.5f, positionVector.z);

	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z),
		vec3(positionVector.x, positionVector.y, positionVector.z - 1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
}
