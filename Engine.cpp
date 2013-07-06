#include "Engine.h"

using namespace std;

unordered_map<string, commandfunc> Engine::commands = unordered_map<string, commandfunc>();
unordered_map<string, string> Engine::commandHelp = unordered_map<string, string>();

CommandContinueType Engine::run(CommandContinueType lastAction)
{
	if (firstrun)
	{
		// Print map info and intro
		printf("WELCOME TO: %s\nA TEXT ADVENTURE BY: %s.\n\n%s\n\n--------\n\n", map->getName(), map->getAuthor(), map->getIntro());
		firstrun = false;
	}

	Room * r = player->getCurrentRoom();

	// Print room name & intro
	if (lastAction == CONTINUE_TYPE_PROMPT)
		printf("%s\n%s\n\n", r->getName(), r->getIntro());

	printf("> ");
	string input;
	getline(cin, input);
	stringstream inputArgs(input);

	string command;
	inputArgs >> command;
	
	return executeCommand(player, command, &inputArgs);
}

CommandContinueType Engine::executeCommand(Player * player, string command, stringstream *args)
{
	if (command == "exit")
		return CONTINUE_TYPE_END;

	unordered_map<string, commandfunc>::iterator itr = commands.find(command);
	if (itr == commands.end())
	{
		cout << "Invalid command entered, type 'help' for a list of commands." << endl << endl;
		return CONTINUE_TYPE_NO_PROMPT;
	}
	return itr->second(player, command, args);
}

void Engine::showHelp(string command)
{
	unordered_map<string, string>::iterator itr = commandHelp.find(command);
	if (itr != commandHelp.end())
	{
		cout << itr->second << endl << endl;
	}
	cout << "Unknown command" << endl << endl;
}

CommandContinueType Engine::helpCommand(Player * player, string command, stringstream *args)
{
	string hcommand;
	if ((*args) >> hcommand)
	{
		unordered_map<string,string>::iterator itr = commandHelp.find(hcommand);
		cout << itr->first << " - " << itr->second << endl;
	}
	else
	{
		cout << "Commands: " << endl << "exit - Exit the game" << endl;
		for (unordered_map<string,string>::iterator itr = commandHelp.begin(); itr != commandHelp.end(); ++itr)
		{
			cout << itr->first << " - " << itr->second << endl;
		}
	}
	cout << endl;
	return CONTINUE_TYPE_NO_PROMPT;
}

CommandContinueType Engine::moveCommand(Player * player, string command, stringstream *args)
{
	string dir;
	if (!(*args >> dir))
	{
		cout << "Invalid arguments specified" << endl;
		showHelp("move");
		return CONTINUE_TYPE_NO_PROMPT;
	}
	
	int d = GameMap::getDirection(dir.c_str());
	if (d == -1)
	{
		cout << "Invalid direction specified" << endl;
		showHelp("move");
		return CONTINUE_TYPE_NO_PROMPT;
	}

	Room * nextRoom = player->getCurrentRoom()->getDoor(d);
	if (nextRoom == NULL)
	{
		cout << "There is nothing " << dir << "." << endl;
		return CONTINUE_TYPE_NO_PROMPT;
	}
	player->move(nextRoom);

	cout << "You move " << dir << "." << endl;
	cout << endl;
	return CONTINUE_TYPE_PROMPT;
}