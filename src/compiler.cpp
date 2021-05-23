#include "compiler.h"

// Takes strings of source file path
Compiler::Compiler(string p) {
	// Initialization
	src.open(p);
	compiledName = p.substr(0, p.find_last_of(".")) + ".cpp";
	lineCount = 1;
	attatchAction = false;

	// Create regex objects for searching
	inputRegex = regex(INPUT_TYPE INPUT_VALUE);
	stateRegex = regex(STATE_TYPE STATE_VALUE);
	outputActionRegex = regex("(" PRINT "|" WRITE ")" ACTION_ARG);
}

// Closes source
Compiler::~Compiler() {
	src.close();
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

// Splits given str at given delim. Puts split strings into given string vector
void Compiler::split(string str, char delim, vector<string> &vect) {
	// Current split being created
	string currentSplit = "";

	// Iterate through each char in given str
	for (char const &c : str) {
		// Once delim reached, push split into split string
		if (c == delim) {
			vect.push_back(currentSplit);
			currentSplit = "";
		// If not delim, add char to current split
		} else {
			currentSplit += c;
		}
	}

	// Append the remaining string
	vect.push_back(currentSplit);
}

// Parses input definition from given line
void Compiler::parseInput(string line) {
	// Will hold regex matches
	smatch matches;

	// Search line with regex
	if (!regex_search(line, matches, inputRegex)) return;

	// Parse input name and the input string from the regex
	string inputName = matches.str(1);
	string inputString = matches.str(2);

	// Adds parsed input data to inputs map
	inputs[inputName] = inputString;
}

// Parses state definition from given line
void Compiler::parseState(string line) {
	// Will hold regex matches
	smatch matches;

	// Search line with regex
	if (!regex_search(line, matches, stateRegex)) return;

	// Parse input name, raw transition mapping string, and actions
	string stateName = matches.str(1);

	mostRecentState = stateName;

	// Throw error if user tries to define their own end state
	if (stateName == END_STATE) Error::endStateClash(lineCount);

	string rawTransitionMap = matches.str(2);
	
	int transitionMapSplitSize = count(rawTransitionMap.begin(), rawTransitionMap.end(), TRANSITION_SEPARATOR) + 1;

	// Create string vector to hold split transition map
	vector<string> transitionMapSplit;

	// Split transition map at commas
	split(rawTransitionMap, TRANSITION_SEPARATOR, transitionMapSplit);

	// Maps input names to state names; represents transitions
	map<string, string> transitionMap;

	// Current transition being parsed in loop
	vector<string> transition;

	// Parse transition mappings from raw transition map
	for (int i = 0; i < transitionMapSplitSize; ++i) {
		// Parse transition and add it to the map
		split(transitionMapSplit[i], MAPPING_SEPARATOR, transition);
		transitionMap[trim(transition[0])] = trim(transition[1]);
	}

	// Add this state and its corresponding transitions to the states map
	states[stateName] = transitionMap;
}

// Parses built-in output actions
void Compiler::parseOutputAction(string line) {
	string trimmedLine = trim(line);

	// If line is only brace, do not try to parse
	if (trimmedLine == BLOCK_START || trimmedLine == BLOCK_END) return;

	// Throw error if line starts with input actions
	if (trimmedLine.rfind(SCAN, 0) != string::npos ||
		trimmedLine.rfind(READ, 0) !=  string::npos) {
		Error::incorrectlyPlacedInputAction(lineCount);
	}

	// Throw error if line does not start with any built-in output actions
	if (trimmedLine.rfind(PRINT, 0) == string::npos &&
		trimmedLine.rfind(WRITE, 0) == string::npos) {
		Error::unknownOutputAction(lineCount);
	}

	// Holds parsed parts of action statement
	smatch actionParts;

	// Search line with regex. Throws error if it does not match
	if (!regex_search(line, actionParts, outputActionRegex)) Error::malformedAction(lineCount);

	// Create action container for action being parsed
	Action action(actionParts.str(1), actionParts.str(2));

	// Add current action to the most recently parsed state
	outputActions[mostRecentState].push_back(action);
}

// Compile parsed data to a compiled file
void Compiler::compile() {
	Writer w(compiledName, &inputs, &states, &outputActions);
	w.write();
}

// Parses source files
void Compiler::parse() {
	// Current line in source
	string line;

	// Iterate through lines in source
	while (getline(src, line)) {
		// Check if current line has an opening block character
		if (line.find(BLOCK_START) != string::npos) {
			// Throw error if user is trying to open a block without closing the last
			if (attatchAction) Error::missingClosingBrace(lineCount);

			// Set attach action to true if this is a valid block start
			else attatchAction = true;
		}	

		// Check what the line is doing. Call appropriate function to parse it
		if (line.find(INPUT_TYPE) != string::npos) parseInput(line);
		else if (line.find(STATE_TYPE) != string::npos) parseState(line);
		else if (attatchAction) parseOutputAction(line);

		// Check if current line has a closing block character
		if (line.find(BLOCK_END) != string::npos) {
			// Throw error if user is trying to close a block without closing the last
			if (!attatchAction) Error::missingOpeningBrace(lineCount);

			// Set attach action to false if this is a valid block end
			else attatchAction = false;
		}

		++lineCount;
	}
}