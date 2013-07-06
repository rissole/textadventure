#include "MapLoader.h"

/**
 * Finds the first node with the specified name from the specified node.
 * This will throw an exception if a node is not found.
 */
xml_node<> * MapLoader::findNodeStrict(const char * name, xml_node<> * fromWhere)
{
	xml_node<> * ret = fromWhere->first_node(name);
	if (ret == NULL)
		throw MapLoadException("MISSING VITAL NODE", name);
	return ret;
}

xml_attribute<> * MapLoader::findAttributeStrict(const char * name, xml_node<> * fromWhere)
{
	xml_attribute<> * ret = fromWhere->first_attribute(name);
	if (ret == NULL)
		throw MapLoadException("MISSING VITAL ATTRIBUTE", name);
	return ret;
}

bool MapLoader::xmlValueToBool(const char * xmlString)
{
	return (strncmp(xmlString, "true", 4) == 0 || strncmp(xmlString, "1", 1) == 0);
}

Object * MapLoader::loadObject(xml_node<> * n)
{
	char * name = findNodeStrict("name", n)->value();
	char * desc = findNodeStrict("desc", n)->value();
	bool bPickUp = false;
	xml_attribute<> * apPickUp = n->first_attribute("pickUp");
	if (apPickUp != NULL)
		bPickUp = xmlValueToBool(apPickUp->value());

	return (new Object(name, desc, bPickUp));
}

/**
 * Loads the room and all associated objects. DOES NOT add the room to the map,
 * but will load any new object definitions to the map.
 */
Room * MapLoader::loadRoom(xml_node<> * n, GameMap * map)
{
	char * name = findNodeStrict("name", n)->value();
	char * intro = findNodeStrict("intro", n)->value();
	bool isStart = false;
	bool isEnd = false;

	// check if this is a start room
	xml_attribute<> * ap = n->first_attribute("start");
	if (ap != NULL)
		isStart = xmlValueToBool(ap->value());

	// check if this is an end room
	ap = n->first_attribute("end");
	if (ap != NULL)
		isEnd = xmlValueToBool(ap->value());

	// create the room
	Room * r = new Room(name, intro, isStart, isEnd);
	
	// Check for object definitions/declarations for this room
	for (xml_node<> * on = n->first_node("object"); on != NULL; on = on->next_sibling("object"))
	{
		xml_attribute<> * apName = on->first_attribute("name");

		// Object already declared, let's check
		if (apName != NULL)
		{
			Object * o = map->getObject(apName->value());
			if (o != NULL)
				r->addObject(o);
			else
				throw MapLoadException("OBJECT NOT DEFINED", apName->value());
			//printf("Added existing object %s for map/room %s/%s\n", o->getName(), map->getName(), r->getName());
		}
		else
		{
			Object * o = loadObject(on);
			map->addObject(o);
			r->addObject(o);
			//printf("Created object %s for map/room %s/%s, description of length %d, canPickUp: %d\n", o->getName(), map->getName(), r->getName(), strlen(o->getDesc()), o->getCanPickUp());
		}
	}
	
	// Get door data
	for (xml_node<> * dn = n->first_node("dir"); dn != NULL; dn = dn->next_sibling("dir"))
	{
		char * sDir = findAttributeStrict("name", dn)->value();
		int dir = GameMap::getDirection(sDir);
		if (dir == -1)
			throw MapLoadException("INVALID DIRECTION NAME", sDir);
		door_name_datas.push_back(door_name_data(r, dir, dn->value()));
	}

	return r;
}

void MapLoader::resolveDoors(GameMap * map)
{
	for (std::list<door_name_data>::iterator itr = door_name_datas.begin(); itr != door_name_datas.end(); ++itr)
	{
		Room * door = map->getRoom((*itr).name);
		if (door == NULL)
			throw MapLoadException("DOOR TO NONEXISTENT ROOM", (*itr).name);
		((*itr).room)->setDoor((*itr).dir, door);
	}
}

/**
 * Creates a new GameMap object at the specified address, and loads
 * data from the specified XML file into it.
 */
void MapLoader::loadMapData(xml_document<> * doc, GameMap ** dest)
{
	// Get basic map info
	xml_node<> * mapn = findNodeStrict("map", doc);
	char * mname = findNodeStrict("name", mapn)->value();
	char * mauth = findNodeStrict("author", mapn)->value();
	char * mintro = findNodeStrict("intro", mapn)->value();
	(*dest) = new GameMap(mname, mauth, mintro);
	GameMap * map = (*dest);
	//printf("Created map %s, by %s with intro of length %d\n", mname, mauth, strlen(mintro));

	// Get global object declarations
	for (xml_node<> * n = mapn->first_node("object"); n != NULL; n = n->next_sibling("object"))
	{
		Object * o = loadObject(n);
		map->addObject(o);
		//printf("Created object %s for map %s, description of length %d, canPickUp: %d\n", o->getName(), map->getName(), strlen(o->getDesc()), o->getCanPickUp());
	}

	// Get room declarations
	int nEndRooms = 0; //will check this > 0 after all rooms interpreted.
	for (xml_node<> * n = mapn->first_node("room"); n != NULL; n = n->next_sibling("room"))
	{
		Room * r = loadRoom(n, map);
		map->addRoom(r);
		if (r->isStart() && map->getStartRoom() != NULL)
			throw MapLoadException("MORE THAN ONE START ROOM", r->getName());
		else if (r->isStart())
			map->setStartRoom(r);
		if (r->isEnd())
			nEndRooms++;
		//printf("Created room %s, intro of length %d, isStart: %d, isEnd: %d\n", r->getName(), strlen(r->getIntro()), r->isStart(), r->isEnd());
	}
	if (nEndRooms == 0)
		throw MapLoadException("NO END ROOMS DEFINED", map->getName());
	if (map->getStartRoom() == NULL)
		throw MapLoadException("NO START ROOM DEFINED", map->getName());

	// Resolve door_name_datas to Room pointers for each room
	resolveDoors(map);
}