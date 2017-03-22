#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Level.h"
#include "Player.h"

using namespace glm;

//Klasa b�d�ca klas� organizuj�c� gr�. Odpowiada za wczytanie najpierw menu, 
//potem przechodzenie mi�dzy poziomami, za�adowanie ponownie poziomupo �mierci... 
//Tego typu rzeczy. Na razie jest nam �rednio potrzebna, dop�ki mamy tylko jeden level,
//ale potem b�dzie potrzebna.

class Game
{
private:
	Player player = *(new Player());
	Level levelOne = *(new Level());

public:
	Game() {};
	~Game() {};

	//Nie mamy �adnych zmian perspektywyw grze - chyba �e np. obr�cimy j� po �mierci (�e si� k�adzie)
	//Ale info o �mierci i potrzebie zrelo�dowania systemu i tak odbiera game, wi�c mo�na zrobi� to tutaj.
	mat4 SetPerspective();
	mat4 SetPlayerPosition();	
	//Macierzy Modelu ni ma bo to raczej sprawa poszczeg�lnych leveli

	void showLevelOne();
};

mat4 Game::SetPerspective()
{
	float aspect = (float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN);
	mat4 PMatrix = glm::perspective(50.0f, aspect, 0.1f, 100.0f);
	return PMatrix;
}

mat4 Game::SetPlayerPosition()
{
	mat4 VMatrix = lookAt(
		vec3(0.0f, 0.0f, -5.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f));
	return VMatrix;
}

void Game::showLevelOne()
{
	//Tutaj reload przy �mierci, przej�cie do nastepnego poziomu przy wygranej itp.
	levelOne.setTeapot();
}