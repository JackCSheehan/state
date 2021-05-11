#include <cstdio>
#include "compiler.h"

int main() {
	Compiler c("test/test.statelang");
	
	c.parse();
	c.compile();

	return 0;
}