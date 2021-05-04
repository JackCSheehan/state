#include <cstdio>
#include "compiler.h"

int main() {
	Compiler c("test/test.statelang");
	
	c.parse();

	return 0;
}