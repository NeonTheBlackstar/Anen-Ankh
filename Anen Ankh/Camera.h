#pragma once

//Jeœli tylko cokolwiek dotyczy kamery - powinno znaleŸæ siê tutaj
//A w innych klasach byæ tylko wywo³ywane

class Camera
{
public:
	vec3 position;
	vec3 viewDirection;
	vec3 UP_VECTOR;
	const float MOVEMENT_SPEED = .06f;
	const float ROTATIONAL_SPEED = .0005f;

public:
	Camera();
	Camera(vec3 newPosition);
	~Camera();

	mat4 getWorldToViewMatrix();
	void FallDown();

	/// Camera Movement ///
	void GoFoward();
	void GoBackward();
	void StrafeLeft();
	void StrafeRight();
};

Camera::Camera()
{
	//UP_VECTOR(0.0f, -1.0f, 0.0f), // Zmienic na +1.0f
	UP_VECTOR = vec3(0.0f, 1.0f, 0.0f);
	viewDirection = vec3(0.0f, 0.0f, -1.0f);
	position = vec3(0.0f, 0.0f, 0.0f);
}

Camera::Camera(vec3 newPosition) : Camera()
{
	position = newPosition;
}

Camera::~Camera() 
{
}

mat4 Camera::getWorldToViewMatrix()
{
	return lookAt(
			position,
			viewDirection + position,
			UP_VECTOR
			);
}

void Camera::GoFoward()
{
	position += viewDirection * MOVEMENT_SPEED;

	/*positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y, positionVector.z + 1.0f),
		vec3(positionVector.x, positionVector.y, positionVector.z - 50.0f),
		vec3(0.0f, -1.0f, 0.0f));
	positionVector = vec3(positionVector.x, positionVector.y, positionVector.z + 1.0f);*/
}

void Camera::GoBackward()
{
	position -= viewDirection * MOVEMENT_SPEED;
}

void Camera::StrafeLeft()
{
	vec3 crossVector = cross(viewDirection, UP_VECTOR);
	position += crossVector * MOVEMENT_SPEED;
}

void Camera::StrafeRight()
{
	vec3 crossVector = cross(viewDirection, UP_VECTOR);
	position -= crossVector * MOVEMENT_SPEED;
}

void Camera::FallDown()
{
	/*positionMatrix = lookAt(
		vec3(positionVector.x, positionVector.y - 0.5, positionVector.z),
		vec3(positionVector.x, positionVector.y - 0.5, positionVector.z - 50.f),
		vec3(0.0f, -1.0f, 0.0f));
	positionVector = vec3(positionVector.x, positionVector.y - 0.5, positionVector.z);*/

	//position = vec3(position.x, position.y - 0.5f, position.z);
}