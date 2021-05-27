#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "compiler.h"
#include "action.h"

using namespace std;

// Contains functions needed to write compiled code
class Writer {
private:
	ofstream f;									// Ofstream to write to compiled file
	map<string, string>* files;					// Pointer to files parsed from source
	map<string, string>* inputs;				// Pointer to inputs parsed from source
	map<string, map<string, string>>* states;	// Pointer to states parsed from source
	map<string, vector<Action>>* outputActions;	// Pointer to output actions parsed from source
	Action* inputAction;						// Pointer to input actions
	string firstState;							// Name of first state parsed

	void writeIncludes();
	void declareAlphabet();
	void declareStates();
	void writeFileDeclarations();
	void writeLogic();
	void writeInputAction();
	void writeOutputAction(Action);
	void writeFileCloses();
public:
	Writer(string, map<string, string>*, map<string, string>*, map<string, map<string, string>>*, map<string, vector<Action>>*, Action*, string);
	~Writer();

	void write();
};

#endif