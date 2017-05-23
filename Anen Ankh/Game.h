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
	Level levelOne = *(new Level());

public:
	//Player player = *(new Player(SetPlayerPositionMatrix(), SetPlayerPositionVector()));
	Player player = *(new Player(SetPlayerPositionMatrix(), SetPlayerPositionVector(), SetPlayerViewVector(), SetPlayerUpVector()));
	bool canGoLeft = true;
	bool canGoRight = true;
	bool canGoFoward = true;
	bool canGoBack = true;

public:
	Game() {};
	~Game();

	//Nie mamy ¿adnych zmian perspektywyw grze - chyba ¿e np. obrócimy j¹ po œmierci (¿e siê k³adzie)
	//Ale info o œmierci i potrzebie zrelo³dowania systemu i tak odbiera game, wiêc mo¿na zrobiæ to tutaj.
	mat4 SetPerspective();
	mat4 SetPlayerPositionMatrix();
	vec3 SetPlayerPositionVector();
	mat4 GetPlayerPositionMatrix();
	vec3 SetPlayerViewVector();
	vec3 SetPlayerUpVector();
	//Macierzy Modelu ni ma bo to raczej sprawa poszczególnych leveli

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
	mat4 P = perspective(50.0f*3.14f / 180.0f, aspect, 0.1f, 1000.0f);
	return P;
}

mat4 Game::SetPlayerPositionMatrix()
{
	mat4 V = lookAt(
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, -1000.0f),
		vec3(0.0f, -1.0f, 0.0f));
	return V;
}

vec3 Game::SetPlayerPositionVector()
{
	return vec3(0.0f, 0.0f, 0.0f);
}

vec3 Game::SetPlayerViewVector()
{
	return vec3(0.0f, 0.0f, -10.0f);
}

vec3 Game::SetPlayerUpVector()
{
	return vec3(0.0f, -1.0f, 0.0f);
}

mat4 Game::GetPlayerPositionMatrix()
{
	return player.positionMatrix;
}

void Game::SetLevelOne()
{
	Room room = *(new Room(vec3(0, 0, 0), vec3(25, 50, 25)));

	Construct floor = *(new Construct("floor", Construct::cube, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(0, 50, 0), vec3(25, 0.1f, 25)));
	floor.SetCubeCollider(vec3(0, 50, 0), vec3(0, 0, 0), vec3(50, 0.1f, 50));
	room.AddObject(floor);

	/*Construct roof = *(new Construct("floor", Construct::cube, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(0, 50, 0), vec3(50, 0.1f, 100)));
	roof.SetCubeCollider(vec3(0, -10, 0), vec3(0, 0, 0), vec3(50, 1, 100));
	room.AddObject(roof);*/

	Construct wallFront = *(new Construct("floor", Construct::cube, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(0, 0, -25), vec3(25, 50, 0.1f)));
	wallFront.SetCubeCollider(vec3(0, 0, -20), vec3(0, 0, 0), vec3(25, 100, 1));
	room.AddObject(wallFront);

	Construct wallBack = *(new Construct("floor", Construct::cube, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(0, 0, 25), vec3(25, 50, 0.1f)));
	wallBack.SetCubeCollider(vec3(0, 0, 20), vec3(0, 0, 0), vec3(25, 100, 1));
	room.AddObject(wallBack);

	Construct wallLeft = *(new Construct("floor", Construct::cube, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(-25, 0, 0), vec3(0.1f, 50, 25)));
	wallLeft.SetCubeCollider(vec3(-20, 0, 0), vec3(0, 0, 0), vec3(1, 100, 25));
	room.AddObject(wallLeft);

	Construct wallRight = *(new Construct("floor", Construct::cube, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(25, 0, 0), vec3(0.1f, 50, 25)));
	wallRight.SetCubeCollider(vec3(20, 0, 0), vec3(0, 0, 0), vec3(1, 100, 25));
	room.AddObject(wallRight);

	Construct stairs1 = *(new Construct("stairs", Construct::stairs, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(15, 33, 0), vec3(10, 17, 15)));
	stairs1.SetCubeCollider(vec3(15, 50, 0), vec3(0, 0, 0), vec3(20, 100, 30));
	stairs1.SetLineCollider(-7.5, 7.5f, 3.2f, -14.2f);
	room.AddObject(stairs1);

	Construct stairs2 = *(new Construct("stairs", Construct::stairs, room.roomMatrix, *(new Texture("Texture\bricks.png")), vec3(0, 0, -10), vec3(5, 10, 17), vec3(0, 1, 0), 90));
	//stairs.SetLineCollider(0, 50, 1, -50);
	room.AddObject(stairs2);

	levelOne.labirynth.AddRoom(room);
}

void Game::ShowLevelOne()
{
	//Tutaj reload przy œmierci, przejœcie do nastepnego poziomu przy wygranej itp.
	
	levelOne.labirynth.ShowLabirynth(player.positionMatrix);

	if (player.noClip == false)
	{

		player.collider.ChangePositionOfCenter(player.positionVector);

		if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[0].cubeCollider) == false
			)/*&& (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[5].cubeCollider) == false
				&& levelOne.labirynth.rooms[0].objects[5].lineCollider.DetectCollision(player.collider.position.z, player.collider.position.y) == true))*/
		{

			player.FallDown();
			player.SetCollider();
		}

		if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[1].cubeCollider))
			canGoFoward = false;
		else
			canGoFoward = true;

		if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[2].cubeCollider))
			canGoBack = false;
		else
			canGoBack = true;

		if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[3].cubeCollider))
			canGoLeft = false;
		else
			canGoLeft = true;

		if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[4].cubeCollider))
			canGoRight = false;
		else
			canGoRight = true;

		if (player.collider.DetectCollision(levelOne.labirynth.rooms[0].objects[5].cubeCollider))
		{
			cout << "YES" << endl;
			while (levelOne.labirynth.rooms[0].objects[5].lineCollider.DetectCollision(player.collider.position.z, player.collider.position.y) == true)
			{
				player.GoUp();
				player.SetCollider();
			}
		}

	}
}