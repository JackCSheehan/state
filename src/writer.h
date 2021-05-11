#ifndef WRITER_H
#define WRITER_H

#define INPUT_SIZE 100

#include <fstream>
#include <iostream>
#include <string>
#include <map>

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