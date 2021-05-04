#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Compiler {
private:
	ifstream src;	// Source code file

public:
	Compiler(string);
	~Compiler();
};


#endif