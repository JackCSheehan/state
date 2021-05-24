#include "compiler.h"

// Takes strings of source file path
Compiler::Compiler(string p) {
	// Initialization
	src.open(p);
	compiledName = p.substr(0, p.find_last_of(".")) + ".cpp";
	lineCount = 1;
	attatchAction = false;

	// Create regex objects for searching
	fileRegex = regex(FILE_TYPE GENERAL_ACTION);
	inputRegex = regex(INPUT_TYPE GENERAL_ACTION);
	stateRegex = regex(STATE_TYPE STATE_VALUE);
	writeRegex = regex(WRITE GENERAL_ACTION);
	printRegex = regex(PRINT CONSOLE_ACTION);
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

// Returns true if the given identifier is valid
bool Compiler::isValidIdentifier(string id) {
	if (regex_match(id, regex(VALID_IDENTIFIER))) return true;
	return false;
}

// Parses input actions (e.g. FILE and INPUT statements which have 3 parts: type, name, arg)
void Compiler::parseInputAction(string line) {
	string trimmedLine = trim(line);

	// Will hold regex matches
	smatch matches;

	// Points to which regex to be used to parse this line
	regex* parsingRegex;

	// Points to which map to fill with parsed data
	map<string, string>* targetMap;

	// Flag to track whether line contains file declaration
	bool isFile = false;

	// Determine which regex to use based on which type of statement this is
	if (trimmedLine.rfind(INPUT_TYPE, 0) != string::npos) {
		parsingRegex = &inputRegex;
		targetMap = &inputs;
	} 
	else if (trimmedLine.rfind(FILE_TYPE, 0) != string::npos){
		parsingRegex = &fileRegex;
		targetMap = &files;
		isFile = true;
	} else {
		Error::malformedAction(lineCount);
	}

	// Search line with regex
	if (!regex_search(trimmedLine, matches, *parsingRegex)) return;

	// Parse action name and the action arg from the regex
	string actionName = matches.str(1);
	string actionArg = matches.str(2);

	// Check that action name is a valid identifier
	if (!isValidIdentifier(actionName)) Error::invalidIdentifier(lineCount, actionName);

	//if (isFile && !exists(actionArg)) Error::fileNotFound(lineCount, actionArg);

	// Adds parsed data to target map
	targetMap->operator[](actionName) = actionArg;
}

// Parses state definition from given line
void Compiler::parseState(string line) {
	// Will hold regex matches
	smatch matches;

	// Search line with regex
	if (!regex_search(line, matches, stateRegex)) return;

	// Parse input name, raw transition mapping string, and actions
	string stateName = matches.str(1);

	// Check that state name is a valid identifier
	if (!isValidIdentifier(stateName)) Error::invalidIdentifier(lineCount, stateName);

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

	// Action that will contain parsed output action data
	Action action;

	// If this action is a PRINT statement
	if (trimmedLine.rfind(PRINT, 0) != string::npos) {
		// Holds parsed parts of PRINT statement
		smatch printParts;

		// Search line with regex. Throws error if it does not match
		if (!regex_search(trimmedLine, printParts, printRegex)) Error::malformedAction(lineCount);

		// Create action container for PRINT action being parsed
		action = Action(PRINT, printParts.str(1));

	// If this action is a WRITE statement
	} else  if (trimmedLine.rfind(WRITE, 0) != string::npos) {
		// Holds parsed parts of WRITE statement
		smatch writeParts;

		// Search line with regex. Throws error if it does not match
		if (!regex_search(trimmedLine, writeParts, writeRegex)) Error::malformedAction(lineCount);

		// Create action container for PRINT action being parsed
		action = Action(WRITE, writeParts.str(1), writeParts.str(2));
	
	// If this statement is not a valid output action
	} else  {
		Error::unknownOutputAction(lineCount);
	}

	// Add current action to the most recently parsed state
	outputActions[mostRecentState].push_back(action);
}

// Compile parsed data to a compiled file
void Compiler::compile() {
	Writer w(compiledName, &files, &inputs, &states, &outputActions);
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
		if (line.find(INPUT_TYPE) != string::npos || line.find(FILE_TYPE) != string::npos) parseInputAction(line);
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

	// Check if any required data is not included
	if (inputs.size() == 0) {
		Error::noInputs();
	}

	if (states.size() == 0) {
		Error::noStates();
	}
}