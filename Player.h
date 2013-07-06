#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameMap.h"
#include "Room.h"
#include "Object.h"

class Player
{
public:
	Player(GameMap * _map, Room * initialRoom) 
		: map(_map), currentRoom(initialRoom)
	{
	}

	Room * getCurrentRoom() { return currentRoom; }
	GameMap * getMap() { return map; }
	void move(Room * newRoom) { currentRoom = newRoom; }

private:
	GameMap * map;
	Room * currentRoom;
	//const char * name;
	//inventory todo
};

#endif /* PLAYER_H */