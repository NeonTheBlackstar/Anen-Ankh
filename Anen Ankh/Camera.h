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
	void GoFoward();
};

void Camera::GoFoward()
{
	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z + 1.0f),
		vec3(positionVector.x, positionVector.y, positionVector.z - 50.0f),
		vec3(0.0f, -1.0f, 0.0f));
	positionVector = vec3(positionVector.x, positionVector.y, positionVector.z + 1.0f);
}

void Camera::FallDown()
{
	positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y - 0.5, positionVector.z),
		vec3(positionVector.x, positionVector.y - 0.5, positionVector.z - 50.f),
		vec3(0.0f, -1.0f, 0.0f));
	positionVector = vec3(positionVector.x, positionVector.y - 0.5, positionVector.z);
}
