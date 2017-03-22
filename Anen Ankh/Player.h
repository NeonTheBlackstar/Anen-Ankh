#pragma once
#include "Camera.h"

//Klasa na wszystko co zwi¹zane z graczem. Przez playera mo¿na by wywo³ywaæ zarówno pojedyncze funkcje 
//z klas z których dziedziczy, jak i te które wymagaj¹ po³¹czenia rzeczy z kilku klas.
//tak ¿eby tylkowywo³ywaæ playera i mieæ gdzieœ co  siê dzieje tak naprawdê w œrodku
class Player : public Camera
{
public:
	Player() {};
	~Player() {};
};