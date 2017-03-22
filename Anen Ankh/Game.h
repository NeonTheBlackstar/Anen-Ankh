#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Level.h"
#include "Player.h"

using namespace glm;

//Klasa bêd¹ca klas¹ organizuj¹c¹ grê. Odpowiada za wczytanie najpierw menu, 
//potem przechodzenie miêdzy poziomami, za³adowanie ponownie poziomupo œmierci... 
//Tego typu rzeczy. Na razie jest nam œrednio potrzebna, dopóki mamy tylko jeden level,
//ale potem bêdzie potrzebna.

class Game
{
private:
	Player player = *(new Player());
	Level levelOne = *(new Level());

public:
	Game() {};
	~Game() {};

	//Nie mamy ¿adnych zmian perspektywyw grze - chyba ¿e np. obrócimy j¹ po œmierci (¿e siê k³adzie)
	//Ale info o œmierci i potrzebie zrelo³dowania systemu i tak odbiera game, wiêc mo¿na zrobiæ to tutaj.
	mat4 SetPerspective();
	mat4 SetPlayerPosition();	
	//Macierzy Modelu ni ma bo to raczej sprawa poszczególnych leveli

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
	//Tutaj reload przy œmierci, przejœcie do nastepnego poziomu przy wygranej itp.
	levelOne.setTeapot();
}