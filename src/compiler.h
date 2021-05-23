#ifndef COMPILER_H
#define COMPILER_H

// Keywords
#define INPUT_TYPE "INPUT"
#define STATE_TYPE "STATE"
#define TRANSITION_SEPARATOR ','
#define MAPPING_SEPARATOR ':'
#define ACTION_SEPARATOR '\n'
#define MAP_OPEN "["
#define MAP_CLOSE "]"
#define BLOCK_START "{"
#define BLOCK_END "}"
#define END_STATE "END"

// Output actions
#define PRINT "print"
#define WRITE "write"

// Input actions
#define SCAN "scan"
#define READ "read"

#define INPUT_SIZE 100

// Regex strings
#define INPUT_VALUE "\\s+(\\w*)\\s+\"(.*?)\""
#define STATE_VALUE "\\s+(\\w*)\\s+\\[(.*?)\\]"
#define ACTION_ARG "\\s*\\((.*)\\)"

// i/o Destinations
#define TO "to"
#define FROM "from"

#include <cstdio>
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <map>
#include "writer.h"
#include "error.h"
#include "action.h"

using namespace std;

// Class containing code needed to parse source and write compiled code
class Compiler {
private:
	string compiledName;						// Name of compiled file
	ifstream src;								// Source code file
	map<string, string> inputs;					// Maps input name to input string
	map<string, map<string, string>> states;	// Maps states to transitions: state names -> (input -> another state name)
	map<string, vector<Action>> outputActions;	// Maps state name to a list of output actions

	int lineCount;								// Current line being parsed by compiler
	bool attatchAction;							// Flag to track if actions are being parsed
	string mostRecentState;						// The most recent state parsed from source

	regex inputRegex;							// Regex to parse input
	regex stateRegex;							// Regex to parse state
	regex outputActionRegex;					// Regex to parse parts of output actions
	regex inputActionRegex;						// Regex to parse parts of input actions

	static string trim(string);
	static void split(string, char, vector<string>&);

	void parseInput(string);
	void parseState(string);
	void parseOutputAction(string);

public:
	Compiler(string);
	~Compiler();

	void parse();
	void compile();
};

#endif