#include <stdio.h>
#include <stdlib.h>
#include "MapLoader.h"
#include "Engine.h"

using namespace rapidxml;

bool readFile(char ** dest, const char * fname)
{
	FILE * f;
	fopen_s(&f, fname, "rb");
	if (f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*dest = new char[size+1];
	if ((*dest) == NULL)
		return false;
	fread(*dest, size, 1, f);
	(*dest)[size] = '\0';
	fclose(f);
	return true;
}

int main(int argc, char ** argv)
{
	char * srcText;
	char * mapname = "yaschas_massif.xml";
	if (!readFile(&srcText, mapname))
	{
		fprintf(stderr, "Error loading file or allocating file buffer.\n");
		return 1;
	}
	xml_document<> * doc = new xml_document<>();
	try
	{
		doc->parse<parse_trim_whitespace>(srcText);
	}
	catch (parse_error p)
	{
		fprintf(stderr, "Error parsing XML document: \"%s\", character %d\n", p.what(), p.where<char>()-srcText);
		delete doc;
		delete[] srcText;
		return 1;
	}

	MapLoader * loader = new MapLoader(doc);
	GameMap * map;
	try
	{
		loader->loadMapData(doc, &map);
	}
	catch (MapLoader::MapLoadException e)
	{
		fprintf(stderr, "Error loading map: %s (%s)", e.what(), e.who());
	}
	printf("Successfully loaded map %s.\n\n", mapname);

	Player * p = new Player(map, map->getStartRoom());
	Engine * engine = new Engine(map, p);
	CommandContinueType lastAction = CONTINUE_TYPE_PROMPT;

	while (lastAction != CONTINUE_TYPE_END)
	{
		lastAction = engine->run(lastAction);
	}

	doc->clear();
	delete p;
	delete engine;
	delete doc;
	delete[] srcText;
	char t;
	printf("END OF THE MAP!\n");
	scanf_s("%c", &t);
	return 0;
}