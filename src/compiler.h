#ifndef COMPILER_H
#define COMPILER_H

// Keywords
#define INPUT_TYPE "INPUT"
#define STATE_TYPE "STATE"
#define TRANSITION_SEPARATOR ','
#define MAPPING_SEPARATOR ':'
#define MAP_OPEN "["
#define MAP_CLOSE "]"
#define BODY_START "{"
#define BODY_END "}"
#define END_STATE "END"


#define INPUT_SIZE 100

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

using namespace std;

// Class containing code needed to parse source and write compiled code
class Compiler {
private:
	string compiledName;						// Name of compiled file
	ifstream src;								// Source code file
	map<string, string> inputs;					// Maps input name to input string
	map<string, map<string, string>> states;	// Maps states to transitions: state names -> (input -> another state name)

	static string trim(string);
	static void split(string, char, string*);

	void parseInput(string);
	void parseState(string);
	void parseStatement(string);

public:
	Compiler(string);
	~Compiler();

	void parse();
	void compile();
};

#endif