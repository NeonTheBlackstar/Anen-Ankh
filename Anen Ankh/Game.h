#pragma once

#include "Level.h"
#include "Construct.h"
#include "Player.h"
#include "Room.h"

//Klasa b�d�ca klas� organizuj�c� gr�. Odpowiada za wczytanie najpierw menu, 
//potem przechodzenie mi�dzy poziomami, za�adowanie ponownie poziomu po �mierci... 
//Tego typu rzeczy. Na razie jest nam �rednio potrzebna, dop�ki mamy tylko jeden level,
//ale potem b�dzie potrzebna.

class Game
{
private:
	Player player = *(new Player(SetPlayerPositionMatrix(), SetPlayerPositionVector()));
	Level levelOne = *(new Level());

public:
	Game() {};
	~Game();

	//Nie mamy �adnych zmian perspektywyw grze - chyba �e np. obr�cimy j� po �mierci (�e si� k�adzie)
	//Ale info o �mierci i potrzebie zrelo�dowania systemu i tak odbiera game, wi�c mo�na zrobi� to tutaj.
	mat4 SetPerspective();
	mat4 SetPlayerPositionMatrix();
	vec3 SetPlayerPositionVector();
	mat4 GetPlayerPositionMatrix();
	//Macierzy Modelu ni ma bo to raczej sprawa poszczeg�lnych leveli

	void SetLevelOne();
	void ShowLevelOne();
};

Game::~Game()
{
	delete &levelOne;
	delete &player;
}

mat4 Game::SetPerspective()
{
	float aspect = (float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN);
	mat4 P = perspective(50.0f, aspect, 0.1f, 100.0f);
	return P;
}

mat4 Game::SetPlayerPositionMatrix()
{
	mat4 V = lookAt(
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, -50.0f),
		vec3(0.0f, -1.0f, 0.0f));
	return V;
}

vec3 Game::SetPlayerPositionVector()
{
	return vec3(0.0f, 0.0f, 0.0f);
}

mat4 Game::GetPlayerPositionMatrix()
{
	return player.positionMatrix;
}

void Game::SetLevelOne()
{
	Room room = *(new Room(vec3(0, 0, 0), vec3(100, 100, 100)));

	Construct floor = *(new Construct("floor", Construct::cube, room.roomMatrix, vec3(0, -10, 0), vec3(100, 1, 100)));
	floor.SetCollider(vec3(0, -10, 0), vec3(0, 0, 0), vec3(100, 1, 100));
	room.AddObject(floor);

	Construct floor2 = *(new Construct("floor", Construct::cube, room.roomMatrix, vec3(0, -1, 0), vec3(10, 1, 10)));
	floor2.SetCollider(vec3(0, -1, 0), vec3(0, 0, 0), vec3(10, 1, 10));
	room.AddObject(floor2);

	levelOne.labirynth.AddRoom(room);
}

void Game::ShowLevelOne()
{
	//Tutaj reload przy �mierci, przej�cie do nastepnego poziomu przy wygranej itp.
	levelOne.labirynth.ShowLabirynth(player.positionMatrix);

	player.collider.ChangePositionOfCenter(player.positionVector);
	player.GoFoward();

	if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[0].collider) == false 
		&& player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[1].collider) == false)
	{
		player.FallDown();
	}
}