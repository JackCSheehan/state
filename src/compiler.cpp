#include "compiler.h"

// Takes strings of source file path
Compiler::Compiler(string f) {
	src.open(f);
}

// Closes source
Compiler::~Compiler() {
	src.close();
}