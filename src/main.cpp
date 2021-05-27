#include <iostream>
#include "compiler.h"

#define FILE_INDEX 1
#define EXPECTED_INPUT_SIZE 2

int main(int argc, char* argv[]) {
	// Check for correct number of arguments
	if (argc < EXPECTED_INPUT_SIZE) {
		cerr << "Please provide a valid source file path\n";
		return 1;
	}

	// Create
	Compiler c(argv[FILE_INDEX]);
	c.parse();
	c.compile();

	return 0;
}