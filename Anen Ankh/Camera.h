#pragma once

//Je�ei tylko cokolwiek dotyczy kamery - powinno znale�� si� tutaj
//A w innych klasach by�o tylko wywo�ywane

class Camera
{
public:
	mat4 positionMatrix;
	vec3 positionVector;
	//Dodane
	vec3 viewVector;
	vec3 UP_vector;
	float movementSpeed = 0.1f;
public:
	Camera() {};
	~Camera() {};
	void FallDown();
	void GoUp();
	void GoFoward();
	void GoBack();
	void GoLeft();
	void GoRight();

	void updateView(vec3 _viewVector);
};

void Camera::updateView(vec3 _viewVector)
{
	double radians = atan2(length(cross(_viewVector, UP_vector)), dot(_viewVector, UP_vector)); // Zakres wartosci od 0 do PI
	
	//double angle = acos(dot(_viewVector, UP_vector)) / (length(_viewVector) * length(UP_vector));

	printf("!!!!!!!!!ANGLE: %f\n\n", radians);

	if (radians <= 3.0f && radians >= 0.1f)
	{
		viewVector = _viewVector;

		positionMatrix = lookAt(
			positionVector,
			positionVector + viewVector,
			UP_vector);
	}
}

void Camera::GoFoward()
{
	positionVector += normalize(vec3(viewVector.x, 0.0f, viewVector.z)) * movementSpeed;

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoBack()
{
	positionVector -= normalize(vec3(viewVector.x, 0.0f, viewVector.z)) * movementSpeed;

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoLeft()
{
	vec3 crossVector = cross(viewVector, UP_vector);
	positionVector -= crossVector * movementSpeed;

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoRight()
{
	vec3 crossVector = cross(viewVector, UP_vector);
	positionVector += crossVector * movementSpeed;

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::FallDown()
{
	positionVector = vec3(positionVector.x, positionVector.y + 0.5f, positionVector.z);

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoUp()
{
	positionVector = vec3(positionVector.x, positionVector.y - 0.5f, positionVector.z);

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}