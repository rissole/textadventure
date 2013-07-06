#pragma once
#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Room.h"
#include "Object.h"
#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, Room*> RoomMap;
typedef std::unordered_map<std::string, Object*> ObjectMap;

/**
 * NOT A DATASTRUCTURE, represents a map file.
 */
class GameMap
{
private:
	const char * name;
	const char * author;
	const char * intro;
	RoomMap * rooms;
	ObjectMap * objs;
	Room * startRoom;

public:
	// Gets a direction that matches enum Direction based on the string
	static int getDirection(const char * sDir)
	{
		char * validDirections = "NESW";
		char * validDirectionsL = "nesw";
		char * c = strchr(validDirections, sDir[0]);
		char * cl = strchr(validDirectionsL, sDir[0]);

		// Since strchr returns a pointer to the character you're searching for in that string,
		// we can use pointer arithmetic to find the index of that character in the string.
		// Since validDirections matches the enum Directions, this gives us a valid direction int.
		
		if (c == NULL)
		{
			if (cl == NULL)
				return -1;
			else
				return (cl - validDirectionsL);
		}
		else
		{
			return (c - validDirections);
		}	
	}

	GameMap(char * name, char * author, char * intro)
	{
		this->name = name;
		this->author = author;
		this->intro = intro;
		rooms = new RoomMap();
		objs = new ObjectMap();
		startRoom = NULL;
	}

	const char * getName() { return name; }
	const char * getIntro() { return intro; }
	const char * getAuthor() { return author; }

	void addRoom(Room * r) { rooms->insert(RoomMap::value_type(r->getName(), r)); }
	Room * getRoom(const char * name)
	{
		RoomMap::iterator itr = rooms->find(name);
		if (itr != rooms->end())
			return itr->second;
		else
			return NULL;
	}
	RoomMap::iterator roomsBegin() { return rooms->begin(); }
	RoomMap::iterator roomsEnd() { return rooms->end(); }

	void addObject(Object * o) { objs->insert(ObjectMap::value_type(o->getName(), o)); }
	Object * getObject(const char * name)
	{
		ObjectMap::iterator itr = objs->find(name);
		if (itr != objs->end())
			return itr->second;
		else
			return NULL;
	}
	ObjectMap::iterator objsBegin() { return objs->begin(); }
	ObjectMap::iterator objsEnd() { return objs->end(); }

	void setStartRoom(Room * r) { startRoom = r; }
	Room * getStartRoom() { return startRoom; }

	~GameMap()
	{
		// Clear out all objects and rooms.
		for (ObjectMap::iterator itr = objs->begin(); itr != objs->end(); ++itr)
			delete itr->second;
		delete objs;
		for (RoomMap::iterator itr = rooms->begin(); itr != rooms->end(); ++itr)
			delete itr->second;
		delete rooms;
	}
};

#endif /* GAMEMAP_H */