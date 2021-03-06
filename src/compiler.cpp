#include "compiler.h"

// Takes strings of source file path
Compiler::Compiler(string p) {
	// Initialization
	src.open(p);

	// Check that source exists
	if (!src) Error::sourceOpenError(p);

	compiledName = p.substr(0, p.find_last_of(".")) + ".cpp";
	lineCount = 1;
	attatchAction = false;
	inputActionParsed = false;

	// Create regex objects for searching
	fileRegex = regex(FILE_TYPE GENERAL_ACTION);
	inputRegex = regex(INPUT_TYPE GENERAL_ACTION);
	stateRegex = regex(STATE_TYPE STATE_VALUE);
	scanRegex = regex(SCAN CONSOLE_ACTION);
	readRegex = regex(READ GENERAL_ACTION);
	writeRegex = regex(WRITE GENERAL_ACTION);
	printRegex = regex(PRINT CONSOLE_ACTION);
}

// Closes source
Compiler::~Compiler() {
	src.close();
}

// Trims whitespace from either end of given string
string Compiler::trim(string str) {
	// Base case; check for empty strings
	if (str.empty()) return "";

	// Indexes of string with starting and ending whitespace omitted
	int start = 0;
	int end = str.length() - 1;
	int len;

	// Find start of non-whitespace
	while (start < str.length() && isspace(str.at(start))) ++start;

	// Find end of non-whitespace
	while (end > 0 && isspace(str.at(end))) --end;

	// Calculate length of trimmed string
	len = (end + 1) - start;

	return str.substr(start, len);
}

// Splits given str at given delim. Puts split strings into given string vector
void Compiler::split(string str, char delim, vector<string> &vect) {
	// Current split being created
	string currentSplit = "";

	// Clear vector before pushing to it
	vect.clear();

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
	// Check if id is valid and doesn't clash with certain reserved words
	if (regex_match(id, regex(VALID_IDENTIFIER)) &&
		id != IN &&
		id != STATE &&
		id != INPUT_TYPE &&
		id != STATE_TYPE &&
		id != FILE_TYPE &&
		id != PRINT &&
		id != WRITE &&
		id != SCAN &&
		id != READ) {
		return true;
	}
	return false;
}

// Takes the given string representing a delimiter and returns the first char delimiter it parses from it as a string
string Compiler::strDelimToChar(string delim) {
	string trimmedDelim = trim(delim);
	string parsedDelim;

	// If no delim given, throw error
	if (trimmedDelim.empty()) Error::invalidDelimiter(lineCount, trimmedDelim);
	if (trimmedDelim.substr(0, 1) == "\\") {
		// Use either en escaped backslash or escape character depending on length of delim
		if (trimmedDelim.length() < 2) parsedDelim = "\\\\";
		else parsedDelim = "\\" + string(1, trimmedDelim[1]);
	} else {
		parsedDelim = trimmedDelim[0];
	}

	return parsedDelim;
}

// Parses input and file declarations since they are so similarly formed
void Compiler::parseInputAndFileDeclarations(string line) {
	string trimmedLine = trim(line);

	// Will hold regex matches
	smatch matches;

	// Points to which regex to be used to parse this line
	regex* parsingRegex;

	// Points to which map to fill with parsed data
	map<string, string>* targetMap;

	// Determine which regex to use based on which type of statement this is
	if (trimmedLine.rfind(INPUT_TYPE, 0) != string::npos) {
		parsingRegex = &inputRegex;
		targetMap = &inputs;
	} 
	else if (trimmedLine.rfind(FILE_TYPE, 0) != string::npos){
		parsingRegex = &fileRegex;
		targetMap = &files;
	} else {
		Error::malformedAction(lineCount);
	}

	// Search line with regex
	if (!regex_search(trimmedLine, matches, *parsingRegex)) Error::malformedAction(lineCount);

	// Parse action identifier and the action arg from the regex
	string actionIdentifier = matches.str(1);
	string actionArg = matches.str(2);

	// Check that action id is a valid identifier
	if (!isValidIdentifier(actionIdentifier)) Error::invalidIdentifier(lineCount, actionIdentifier);

	// Adds parsed data to target map
	targetMap->operator[](actionIdentifier) = actionArg;
}

// Parses state definition from given line
void Compiler::parseState(string line) {
	// Will hold regex matches
	smatch matches;

	// Search line with regex
	if (!regex_search(line, matches, stateRegex)) Error::malformedAction(lineCount);

	// Parse input name, raw transition mapping string, and actions
	string stateName = trim(matches.str(1));

	// Check that state name is a valid identifier
	if (!isValidIdentifier(stateName)) Error::invalidIdentifier(lineCount, stateName);

	// Throw error if user tries to define their own end state
	if (stateName == END_STATE) Error::endStateClash(lineCount);

	mostRecentState = stateName;

	// If first state hasn't been assigned yet, assign in
	if (firstState.empty()) firstState = stateName;

	string rawTransitionMap = matches.str(2);

	// If the transition map is blank, put a blank string mapping in the states map to indicate sink state
	if (rawTransitionMap.empty()) {
		map<string, string> blank;
		blank[""] = "";
		states[stateName] = blank;
		return;
	}

	// Create string vector to hold split transition map
	vector<string> transitionMapSplit;

	// Split transition map at commas
	split(rawTransitionMap, TRANSITION_SEPARATOR, transitionMapSplit);

	// Maps input names to state names; represents transitions
	map<string, string> transitionMap;

	// Current transition being parsed in loop
	vector<string> transition;

	// Parse transition mappings from raw transition map
	for (int i = 0; i < transitionMapSplit.size(); ++i) {
		// Parse transition and add it to the map
		split(transitionMapSplit[i], MAPPING_SEPARATOR, transition);

		// Don't add this transition if it transitions to current state
		if (stateName == trim(transition[1])) continue;

		// Add transition to map if it hasn't already been added
		transitionMap[trim(transition[0])] = trim(transition[1]);
	}

	// Add this state and its corresponding transitions to the states map
	states[stateName] = transitionMap;
}

// Parsed input actions such as SCAN and READ
void Compiler::parseInputAction(string line) {
	string trimmedLine = trim(line);

	// Check for duplicate input actions
	if (inputActionParsed) Error::multipleInputActions(lineCount);

	// Action that will contain parsed input action data
	Action action;

	// If this action is a SCAN statement
	if (trimmedLine.rfind(SCAN, 0) != string::npos) {
		// Holds parsed parts of SCAN statement
		smatch scanParts;

		// Search line with regex. Throws error if it does not match
		if (!regex_search(trimmedLine, scanParts, scanRegex)) Error::malformedAction(lineCount);

		// Create action container for SCAN action being parsed
		action = Action(SCAN, strDelimToChar(scanParts.str(1)));

	// If the action is a READ statement
	} else if (trimmedLine.rfind(READ, 0) != string::npos) {
		smatch readParts;

		// Search line with regex. Throws error if it does not match
		if (!regex_search(trimmedLine, readParts, readRegex)) Error::malformedAction(lineCount);

		// Create action container for READ action being parsed
		action = Action(READ, readParts.str(1), strDelimToChar(readParts.str(2)));
	
	// If this statement is not a valid input action
	}

	inputAction = action;
	inputActionParsed = true;
}

// Parses built-in output actions
void Compiler::parseOutputAction(string line) {
	string trimmedLine = trim(line);

	// If line is only brace, do not try to parse
	if (trimmedLine == BLOCK_START || trimmedLine == BLOCK_END) return;

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
	Writer w(compiledName, &files, &inputs, &states, &outputActions, &inputAction, firstState);
	w.write();
}

// Checks parsed data for errors
void Compiler::checkForParseErrors() {
	// Check if any required data is not included
	if (inputs.size() == 0) Error::noInputs();

	if (states.size() == 0) Error::noStates();

	// Check if there is no input action
	if (inputAction.name.empty()) Error::noInputActions();

	// If input action is WRITE and the file name hasn't been declared, throw error
	if (inputAction.name == READ && !files.count(inputAction.identifier))
		Error::referencingUndeclaredFile(inputAction.identifier);

	// Check WRITE actions to make sure they reference valid files
	for (pair<string, vector<Action>> outputAction : outputActions) {
		// Iterate through output actions and check for WRITEs
		for (Action a : outputAction.second) {
			// Throw error if WRITE file wasn't declared
			if (a.name == WRITE && !files.count(a.identifier))
				Error::referencingUndeclaredFile(a.identifier);
		}
	}

	// Check that all inputs and states referenced in transitions were declared
	for (pair<string, map<string, string>> state : states) {
		// Iterate through each transition
		for (pair<string, string> trans : state.second) {
			// If transition input does not exist in inputs, throw error
			if (!trans.first.empty() && !inputs.count(trans.first))
				Error::referencingUndeclaredInput(trans.first);

			// If transition target state does not exist in states, throw error
			if (!trans.second.empty() &&!states.count(trans.second))
				Error::referencingUndeclaredState(trans.second);
		}
	}
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
		if (trim(line).rfind(COMMENT, 0) != string::npos);
		else if (line.find(INPUT_TYPE) != string::npos || line.find(FILE_TYPE) != string::npos) parseInputAndFileDeclarations(line);
		else if (line.find(STATE_TYPE) != string::npos) parseState(line);
		else if (line.find(SCAN) != string::npos || line.find(READ) != string::npos) parseInputAction(line);
		else if (attatchAction) parseOutputAction(line);
		else {
			if (trim(line) != "") Error::unknownStatement(lineCount, line);
		}

		// Check if current line has a closing block character
		if (line.find(BLOCK_END) != string::npos) {
			// Throw error if user is trying to close a block without closing the last
			if (!attatchAction) Error::missingOpeningBrace(lineCount);

			// Set attach action to false if this is a valid block end
			else attatchAction = false;
		}

		++lineCount;
	}
	checkForParseErrors();
}