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
	void ShowLabirynth(mat4 playerPosition);
};

void Labirynth::AddRoom(Room room)
{
	rooms.push_back(room);
}

void Labirynth::ShowLabirynth(mat4 playerPosition)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		rooms[i].ShowRoom(playerPosition);
	}
}