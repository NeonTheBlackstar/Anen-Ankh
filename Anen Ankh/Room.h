#pragma once

#include "Construct.h"
#include <vector>

class Room
{
public:
	mat4 roomMatrix;
	vector <Construct> objects;

private:
	vec3 position;
	vec3 size;
	vector <int> neighboringRooms;

public:
	Room(vec3 position, vec3 size);
	~Room() {};
	void AddObject(Construct object);
	void ShowRoom(mat4 playerPosition, Texture * tex = NULL);
};

Room::Room(vec3 position, vec3 size)
{
	roomMatrix = mat4(1);
	roomMatrix = translate(roomMatrix, position);
	this->position = position;
	this->size = size;
}

void Room::AddObject(Construct object)
{
	objects.push_back(object);
}

void Room::ShowRoom(mat4 playerPosition, Texture * tex)
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].DrawSolid(playerPosition, tex);
	}
}