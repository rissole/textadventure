#pragma once
#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "rapidxml\rapidxml.hpp"
#include "GameMap.h"

using namespace rapidxml;

class MapLoader
{
private:
	xml_document<> * doc;

	struct door_name_data
	{
		door_name_data(Room * _room, int _dir, const char * _name) : room(_room), dir(_dir), name(_name) {};
		Room * room;
		int dir;
		const char * name;
	};
	std::list<door_name_data> door_name_datas;

public:
	class MapLoadException : public std::exception
	{
	private: 
		const char * m_excepttype;
		const char * m_tag;
	public: 
		MapLoadException(const char * excepttype, const char * tag) : m_excepttype(excepttype), m_tag(tag) {}

		const char * what()
		{
			return m_excepttype;
		}

		const char * who()
		{
			return m_tag;
		}
	};

	MapLoader(xml_document<> * doc)
		: doc(doc)
	{
	}

	~MapLoader()
	{
	}

	static xml_node<> * findNodeStrict(const char * name, xml_node<> * fromWhere);
	static xml_attribute<> * findAttributeStrict(const char * name, xml_node<> * fromWhere);
	static bool xmlValueToBool(const char * xmlString);

	Object * loadObject(xml_node<> * n);
	Room * loadRoom(xml_node<> * n, GameMap * map);
	void resolveDoors(GameMap * map);
	void loadMapData(xml_document<> * doc, GameMap ** dest);
};

#endif /* MAPLOADER_H */