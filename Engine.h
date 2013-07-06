#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <sstream>
#include <string>
#include "GameMap.h"
#include "Player.h"

enum CommandContinueType
{
	CONTINUE_TYPE_NO_PROMPT,
	CONTINUE_TYPE_PROMPT,
	CONTINUE_TYPE_END,
};

typedef CommandContinueType (*commandfunc)(Player*,std::string,std::stringstream*);

class Engine
{
public:
	Engine(GameMap * map, Player * p)
		: map(map), player(p), firstrun(true)
	{
		commands.insert(std::pair<std::string, commandfunc>("help", &Engine::helpCommand));
		commands.insert(std::pair<std::string, commandfunc>("move", &Engine::moveCommand));

		commandHelp.insert(std::pair<std::string, std::string>("move", "Usage: move <direction>, direction = north/south/east/west"));
	}

	~Engine()
	{
	}

	CommandContinueType run(CommandContinueType lastAction);
	CommandContinueType executeCommand(Player * player, std::string command, std::stringstream *args);

	static void showHelp(std::string command);
	static CommandContinueType moveCommand(Player * player ,std::string command, std::stringstream *args);
	static CommandContinueType helpCommand(Player * player ,std::string command, std::stringstream *args);

private:
	GameMap * map;
	Player * player;
	bool firstrun;
	static std::unordered_map<std::string, commandfunc> commands;
	static std::unordered_map<std::string, std::string> commandHelp;
};

#endif /* ENGINE_H */