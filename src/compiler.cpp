#include "compiler.h"

// Takes strings of source file path
Compiler::Compiler(string f) {
	src.open(f);
}

// Closes source
Compiler::~Compiler() {
	src.close();
}

// Parses input definition from given line
void Compiler::parseInput(string line) {
	// Create regex for parsing input definitions
	regex parseInputRegex(INPUT_TYPE "\\s+(\\w*)\\s+\"(.*?)\"");

	// Will hold regex matches
	smatch matches;

	// Research line for above regex to parse input
	if (!regex_search(line, matches, parseInputRegex)) return;

	// Parse input name and the input string from the regex
	string inputName = matches.str(1);
	string inputString = matches.str(2);

	// Adds parsed input data to inputs map
	inputs[inputName] = inputString;
}

// Parses state definition from given line
void Compiler::parseState(string line) {

}

// Parses other statements, such as built-in functions
void Compiler::parseStatement(string line) {

}

// Parses source files
void Compiler::parse() {
	// Current line in source
	string line;

	// Iterate through lines in source
	while (getline(src, line)) {
		printf("%s\n", line.c_str());

		// Check what the line is doing. Call appropriate function to parse it
		if (line.find(INPUT_TYPE) != string::npos) parseInput(line);
		else if (line.find(STATE_TYPE) != string::npos) parseState(line);
		else parseStatement(line);
	}
}