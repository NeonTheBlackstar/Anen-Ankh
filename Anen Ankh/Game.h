#pragma once

#include "Level.h"
#include "Construct.h"
#include "Player.h"
#include "Room.h"

//Klasa bêd¹ca klas¹ organizuj¹c¹ grê. Odpowiada za wczytanie najpierw menu, 
//potem przechodzenie miêdzy poziomami, za³adowanie ponownie poziomu po œmierci... 
//Tego typu rzeczy. Na razie jest nam œrednio potrzebna, dopóki mamy tylko jeden level,
//ale potem bêdzie potrzebna.

class Game
{
private:
	//Player player = *(new Player(SetPlayerPositionMatrix(), SetPlayerPositionVector()));
	Player player = *(new Player(vec3(0.0f, 0.0f, -50.0f)));
	Level levelOne = *(new Level());

public:
	Game() {};
	~Game();

	//Nie mamy ¿adnych zmian perspektywyw grze - chyba ¿e np. obrócimy j¹ po œmierci (¿e siê k³adzie)
	//Ale info o œmierci i potrzebie zrelo³dowania systemu i tak odbiera game, wiêc mo¿na zrobiæ to tutaj.
	mat4 SetPerspective();
	mat4 SetPlayerPositionMatrix();
	vec3 SetPlayerPositionVector();
	mat4 GetPlayerPositionMatrix();
	// Player's vectors
	vec3 GetPlayerPositionVector();
	vec3 GetPlayerViewVector();
	//
	Player * GetPlayer();

	//Macierzy Modelu ni ma bo to raczej sprawa poszczególnych leveli

	void SetLevelOne();
	void ShowLevelOne();
};

Game::~Game()
{
	delete &levelOne;
	delete &player;
}

Player * Game::GetPlayer()
{
	return &player;
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

vec3 Game::GetPlayerPositionVector()
{
	return player.position;
}

vec3 Game::GetPlayerViewVector()
{
	return player.viewDirection;
}

mat4 Game::GetPlayerPositionMatrix()
{
	//return player.positionMatrix;
	return player.getWorldToViewMatrix();
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
	//Tutaj reload przy œmierci, przejœcie do nastepnego poziomu przy wygranej itp.
	//levelOne.labirynth.ShowLabirynth(player.positionMatrix);
	levelOne.labirynth.ShowLabirynth(player.getWorldToViewMatrix());

	//player.collider.ChangePositionOfCenter(player.positionVector);
	player.collider.ChangePositionOfCenter(player.position);
	//player.GoFoward();

	/*if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[0].collider) == false 
		&& player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[1].collider) == false)
	{
		player.FallDown();
	}*/
}