#include <cstdio>
#include "compiler.h"

int main() {
	Compiler c("main.statelang");
	
	c.parse();

	return 0;
}