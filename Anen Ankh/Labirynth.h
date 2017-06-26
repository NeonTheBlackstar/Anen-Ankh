#pragma once

#include "Room.h"

class Labirynth
{
public:
	vector <Room> rooms;

private:
	int currentRoom;

public:
	Labirynth() {};
	~Labirynth() {};
	void AddRoom(Room room);
	void ShowLabirynth(mat4 playerPosition, Texture * tex = NULL);
};

void Labirynth::AddRoom(Room room)
{
	rooms.push_back(room);
}

void Labirynth::ShowLabirynth(mat4 playerPosition, Texture * tex)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		rooms[i].ShowRoom(playerPosition, tex);
	}
}