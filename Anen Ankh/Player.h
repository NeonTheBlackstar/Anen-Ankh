#pragma once
#include "Camera.h"

//Klasa na wszystko co zwi�zane z graczem. Przez playera mo�na by wywo�ywa� zar�wno pojedyncze funkcje 
//z klas z kt�rych dziedziczy, jak i te kt�re wymagaj� po��czenia rzeczy z kilku klas.
//tak �eby tylkowywo�ywa� playera i mie� gdzie� co  si� dzieje tak naprawd� w �rodku
class Player : public Camera
{
public:
	Player() {};
	~Player() {};
};