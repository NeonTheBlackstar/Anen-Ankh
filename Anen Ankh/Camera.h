#pragma once

//Je¿ei tylko cokolwiek dotyczy kamery - powinno znaleŸæ siê tutaj
//A w innych klasach by³o tylko wywo³ywane

class Camera
{
public:
	mat4 positionMatrix;
	vec3 positionVector;
	//Dodane
	vec3 viewVector;
	vec3 UP_vector;
	float movementSpeed = 0.1f;
	bool canGoLeft = true;
	bool canGoRight = true;
	bool canGoFoward = true;
	bool canGoBack = true;
public:
	Camera() {};
	~Camera() {};
	void FallDown();
	void GoUp();
	void GoDown();
	void GoFoward(bool fly);
	void GoBack(bool fly);
	void GoLeft(bool fly);
	void GoRight(bool fly);

	void updateView(vec3 _viewVector);
};

void Camera::updateView(vec3 _viewVector)
{
	double radians = atan2(length(cross(_viewVector, UP_vector)), dot(_viewVector, UP_vector)); // Zakres wartosci od 0 do PI

	if (radians <= 3.0f && radians >= 0.1f)
	{
		viewVector = _viewVector;

		positionMatrix = lookAt(
			positionVector,
			positionVector + viewVector,
			UP_vector);
	}
}

void Camera::GoFoward(bool fly = false)
{

	if (fly)
		positionVector += normalize(viewVector) * movementSpeed * 10.0f;
	else
	{
		vec3 lastPlayerPosition = positionVector;
		positionVector += normalize(vec3(viewVector.x, 0.0f, viewVector.z)) * movementSpeed;
		if (canGoFoward == false)
		{
			if (positionVector.z < lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoBack == false)
		{
			if (positionVector.z > lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoLeft == false)
		{
			if (positionVector.x < lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
		if (canGoRight == false)
		{
			if (positionVector.x > lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
	}

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoBack(bool fly = false)
{
	if (fly)
		positionVector -= normalize(viewVector) * movementSpeed * 10.0f;
	else
	{
		vec3 lastPlayerPosition = positionVector;
		positionVector -= normalize(vec3(viewVector.x, 0.0f, viewVector.z)) * movementSpeed;
		if (canGoFoward == false)
		{
			if (positionVector.z < lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoBack == false)
		{
			if (positionVector.z > lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoLeft == false)
		{
			if (positionVector.x < lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
		if (canGoRight == false)
		{
			if (positionVector.x > lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
	}

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoLeft(bool fly = false)
{
	vec3 crossVector = normalize(cross(viewVector, UP_vector));

	if (fly)
		positionVector -= crossVector * movementSpeed * 10.0f;
	else
	{
		vec3 lastPlayerPosition = positionVector;
		positionVector -= crossVector * movementSpeed;
		if (canGoFoward == false)
		{
			if (positionVector.z < lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoBack == false)
		{
			if (positionVector.z > lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoLeft == false)
		{
			if (positionVector.x < lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
		if (canGoRight == false)
		{
			if (positionVector.x > lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
	}

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}

void Camera::GoRight(bool fly = false)
{
	vec3 crossVector = normalize(cross(viewVector, UP_vector));

	if (fly)
		positionVector += crossVector * movementSpeed * 10.0f;
	else
	{
		vec3 lastPlayerPosition = positionVector;
		positionVector += crossVector * movementSpeed;
		if (canGoFoward == false)
		{
			if (positionVector.z < lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoBack == false)
		{
			if (positionVector.z > lastPlayerPosition.z)
				positionVector = vec3(positionVector.x, positionVector.y, lastPlayerPosition.z);
		}
		if (canGoLeft == false)
		{
			if (positionVector.x < lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
		if (canGoRight == false)
		{
			if (positionVector.x > lastPlayerPosition.x)
				positionVector = vec3(lastPlayerPosition.x, positionVector.y, positionVector.z);
		}
	}

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

void Camera::GoDown()
{
	positionVector = vec3(positionVector.x, positionVector.y + 0.5f, positionVector.z);

	positionMatrix = lookAt(
		positionVector,
		viewVector + positionVector,
		UP_vector);
}