#pragma once

#include "Camera.h"
#include "Collider.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

//Klasa na wszystko co zwi�zane z graczem. Przez playera mo�na by wywo�ywa� zar�wno pojedyncze funkcje 
//z klas z kt�rych dziedziczy, jak i te kt�re wymagaj� po��czenia rzeczy z kilku klas.
//tak �eby tylkowywo�ywa� playera i mie� gdzie� co  si� dzieje tak naprawd� w �rodku
class Player : public Camera
{
public:
	CubeCollider3D collider;
	bool noClip = false;

public:
	Player(mat4 positionMatrix, vec3 positionVector);
	Player(mat4 positionMatrix, vec3 positionVector, vec3 _viewVector, vec3 _UP_vector);
	~Player();
	void SetCollider();
};

Player::~Player()
{
	delete &collider;
}

Player::Player(mat4 positionMatrix, vec3 positionVector)
{
	this->positionMatrix = positionMatrix;
	this->positionVector = positionVector;
	collider = *(new CubeCollider3D(vec3(positionVector.x, positionVector.y - 2.5, positionVector.z), vec3(0, -1, 0), vec3(1, 3, 1)));
};

Player::Player(mat4 positionMatrix, vec3 positionVector, vec3 _viewVector, vec3 _UP_vector)
{
	this->positionMatrix = positionMatrix;
	this->positionVector = positionVector;
	this->viewVector = _viewVector;
	this->UP_vector = _UP_vector;
	collider = *(new CubeCollider3D(vec3(positionVector.x, positionVector.y - 2.5, positionVector.z), vec3(0, -2.5, 0), vec3(1, 5, 1)));
};

void Player::SetCollider()
{
	collider.position = vec3(positionVector.x, positionVector.y - 2.5, positionVector.z);
}