#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "compiler.h"
#include "error.h"

using namespace std;

// Contains functions needed to write compiled code
class Writer {
private:
	ofstream f;									// Ofstream to write to compiled file
	map<string, string>* inputs;				// Pointer to inputs parsed from source
	map<string, map<string, string>>* states;	// Pointer to states parsed from source

	void writeIncludes();
	void declareAlphabet();
	void declareStates();
	void writeLogic();
public:
	Writer(string, map<string, string>*, map<string, map<string, string>>*);
	~Writer();

	void write();
};

#endif