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

#include <cstdio>
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <map>

using namespace std;

// Class containing code needed to parse source and write compiled code
class Compiler {
private:
	ifstream src;								// Source code file
	map<string, string> inputs;					// Maps input name to input string
	map<string, map<string, string>> states;	// States and input -> state mappings

	static string trim(string);
	static void split(string, char, string*);

public:
	Compiler(string);
	~Compiler();

	void parseInput(string);
	void parseState(string);
	void parseStatement(string);
	void parse();
};


#endif