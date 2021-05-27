#include <cstdio>
#include "compiler.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Please provide a valid source file path\n";
		return 1;
	}

	Compiler c(argv[1]);
	
	c.parse();
	c.compile();

	return 0;
}