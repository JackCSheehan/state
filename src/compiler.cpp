#include "compiler.h"
// Takes strings of source file path
Compiler::Compiler(string p) {
	src.open(p);
	compiledName = p.substr(0, p.find_last_of(".")) + ".cpp";
}

// Closes source
Compiler::~Compiler() {
}

// Trims whitespace from either end of given string
string Compiler::trim(string str) {
	// Indexes of string with starting and ending whitespace omitted
	int start = 0;
	int end = str.size() - 1;

	// Find start of non-whitespace
	while (start < str.size() && isspace(str.at(start))) ++start;

	// Find end of non-whitespace
	while (end > 0 && isspace(str.at(end))) --end;

	return str.substr(start, end + 1);
}

// Splits given str at given delim. Puts split strings into given string array
void Compiler::split(string str, char delim, string* arr) {
	// Current split being created
	string currentSplit = "";

	// Current index being written to in string array
	int i = 0;

	// Iterate through each char in given str
	for (char const &c : str) {
		// Once delim reached, push split into split string
		if (c == delim) {
			arr[i] = currentSplit;
			currentSplit = "";
			++i;
		// If not delim, add char to current split
		} else {
			currentSplit += c;
		}
	}

	// Append the remaining string if not blank
	arr[i] = currentSplit;
}

// Parses input definition from given line
void Compiler::parseInput(string line) {
	// Regex for parsing input definitions
	regex parseInputRegex(INPUT_TYPE "\\s+(\\w*)\\s+\"(.*?)\"");

	// Will hold regex matches
	smatch matches;

	// Search line with regex
	if (!regex_search(line, matches, parseInputRegex)) return;

	// Parse input name and the input string from the regex
	string inputName = matches.str(1);
	string inputString = matches.str(2);

	// Adds parsed input data to inputs map
	inputs[inputName] = inputString;
}

// Parses state definition from given line
void Compiler::parseState(string line) {
	// Regex for parsing state definitions
	regex parseStateRegex(STATE_TYPE "\\s+(\\w*)\\s+\\[(.*?)\\]");

	// Will hold regex matches
	smatch matches;

	// Search line with regex
	if (!regex_search(line, matches, parseStateRegex)) return;

	// Parse input name and raw transition mapping string
	string stateName = matches.str(1);
	string rawTransitionMap = matches.str(2);

	int transitionMapSplitSize = count(rawTransitionMap.begin(), rawTransitionMap.end(), TRANSITION_SEPARATOR) + 1;

	// Create string array to hold split transition map
	string* transitionMapSplit = new string[transitionMapSplitSize];

	// Split transition map at commas
	split(rawTransitionMap, TRANSITION_SEPARATOR, transitionMapSplit);

	// Maps input names to state names; represents transitions
	map<string, string> transitionMap;

	// Current transition being parsed in loop
	string transition[2];

	// Parse transition mappings from raw transition map
	for (int i = 0; i < transitionMapSplitSize; ++i) {
		// Check that split won't cause out of bounds access
		if (count(transitionMapSplit[i].begin(), transitionMapSplit[i].end(), MAPPING_SEPARATOR) != 1) {
			return;
		}

		// Parse transition and add it to the map
		split(transitionMapSplit[i], MAPPING_SEPARATOR, transition);
		transitionMap[trim(transition[0])] = trim(transition[1]);
	}

	// Add this state and its corresponding transitions to the states map
	states[stateName] = transitionMap;

	delete[] transitionMapSplit;
}

// Parses other statements, such as built-in functions
void Compiler::parseStatement(string line) {

}

// Compile parsed data to a compiled file
void Compiler::compile() {
	// Ofstream to write to compiled file
	ofstream f(compiledName);

	// Write enum for input alphabet
	f << "enum InputAlphabet {\n";
	for (auto& input : inputs) {
		f << "\t" << input.first << " = \"" << input.second << "\",\n";
	}
	f << "};\n";

	// Write enum for state
	f << "enum State {\n";
	for (auto& state : states) {
		f << "\t" << state.first << ",\n";
	}
	f << "};\n";

	// Write main
	f << "int main() {\n" << "\treturn 0;\n}";

	f.close();
}

// Parses source files
void Compiler::parse() {
	// Current line in source
	string line;

	// Iterate through lines in source
	while (getline(src, line)) {
		// Check what the line is doing. Call appropriate function to parse it
		if (line.find(INPUT_TYPE) != string::npos) parseInput(line);
		else if (line.find(STATE_TYPE) != string::npos) parseState(line);
		else parseStatement(line);
	}

	src.close();

	// After inputs and states parsed, compile
	compile();
}