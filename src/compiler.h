#ifndef COMPILER_H
#define COMPILER_H

// Keywords
#define INPUT_TYPE "INPUT"
#define STATE_TYPE "STATE"
#define FILE_TYPE "FILE"
#define TRANSITION_SEPARATOR ','
#define MAPPING_SEPARATOR ':'
#define MAP_OPEN "["
#define MAP_CLOSE "]"
#define BLOCK_START "{"
#define BLOCK_END "}"
#define END_STATE "END"
#define IN "IN"
#define IN_MARKER "$in"
#define COMMENT "//"

// Output actions
#define PRINT "PRINT"
#define WRITE "WRITE"

// Input actions
#define SCAN "SCAN"
#define READ "READ"

#define INPUT_SIZE 100

// Regex strings
#define GENERAL_ACTION "\\s+(\\w*)\\s+\"(.*?)\""	// Regex for parsing general 3-part actions
#define STATE_VALUE "\\s+(\\w*)\\s+\\[(.*?)\\]"		// Regex for parsing the names and transitions of states
#define CONSOLE_ACTION	"\\s*\"(.*)\""				// Regex for parsing PRINT and SCAN actions
#define VALID_IDENTIFIER "^[a-zA-Z_]\\w*$"			// Regex for checking for a valid identifier name

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
	map<string, string> files;					// Maps file names to file  paths
	map<string, string> inputs;					// Maps input name to input string
	map<string, map<string, string>> states;	// Maps states to transitions: state names -> (input -> another state name)
	Action inputAction;							// Holds action object representing input action
	map<string, vector<Action>> outputActions;	// Maps state name to a list of output actions

	int lineCount;								// Current line being parsed by compiler
	bool attatchAction;							// Flag to track if actions are being parsed
	bool inputActionParsed;						// Flag to track if an input action has already been parsed
	string mostRecentState;						// The most recent state parsed from source

	regex fileRegex;							// Regex to parse file declarations
	regex inputRegex;							// Regex to parse input declarations
	regex stateRegex;							// Regex to parse state declarations
	regex scanRegex;							// Regex to parse SCAN actions
	regex readRegex;							// Regex to parse READ actions
	regex writeRegex;							// Regex to parse WRITE statements
	regex printRegex;							// Regex to parse PRINT statements

	static string trim(string);
	static void split(string, char, vector<string>&);
	static bool isValidIdentifier(string);
	static bool exists(string);

	string strDelimToChar(string);

	void parseInputAndFileDeclarations(string);
	void parseState(string);
	void parseInputAction(string);
	void parseOutputAction(string);

public:
	Compiler(string);
	~Compiler();

	void parse();
	void compile();
};

#endif