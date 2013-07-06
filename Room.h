#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "Object.h"
#include <list>

enum Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NUM_DIRECTIONS
};

class Room
{
private:
	const char * name;
	const char * intro;
	Room * doors[NUM_DIRECTIONS]; // these are the validated rooms themselves based on the names above
	bool _isStart;
	bool _isEnd;

	// References to objects that are stored in this room.
	std::list<Object*> * objs;

public:
	Room(char * name, char * intro, bool isStart, bool isEnd)
	{
		this->name = name;
		this->intro = intro;
		this->_isStart = isStart;
		this->_isEnd = isEnd;
		for (int i = 0; i < NUM_DIRECTIONS; ++i)
		{
			doors[i] = NULL;
		}
		objs = new std::list<Object*>();
	}

	void addObject(Object * o) { objs->push_back(o); }

	const char * getName() { return name; }
	const char * getIntro() { return intro; }
	bool isStart() { return _isStart; }
	bool isEnd() { return _isEnd; }

	// 0 <= d <= NUM_DIRECTIONS
	void setDoor(int d, Room * r) { doors[d] = r; }
	Room * getDoor(int d) { return doors[d]; }

	~Room()
	{
		// We don't delete each object - just the list. Leave deleting the objects to GameMap.
		delete objs;
	}
};

#endif /* ROOM_H */