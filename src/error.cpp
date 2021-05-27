#include "error.h"

namespace Error {

	// Error thrown if user tries to define their own END state
	void endStateClash(int line) {
		cerr << ERROR_MESSAGE " Cannot create state named " << END_STATE << " on line " << line << " as it is a reserved state name\n";
		exit(1);
	}

	// Error thrown if a block is not ended before another one is started
	void missingClosingBrace(int line) {
		cerr << ERROR_MESSAGE " Missing closing brace on line " << line << "\n";
		exit(1);
	}

	// Error thrown if a block is closed without one being opened
	void missingOpeningBrace(int line) {
		cerr << ERROR_MESSAGE " Missing opening brace on line " << line << "\n";
		exit(1);
	}

	// Error thrown if user tries to run an unknown output action
	void unknownOutputAction(int line) {
		cerr << ERROR_MESSAGE " Unknown output action on line " << line << "\n";
		exit(1);
	}

	// Error thrown if an input action is unknown
	void unknownInputAction(int line) {
		cerr << ERROR_MESSAGE " Unknown input action on line " << line << "\n";
		exit(1);
	}

	// Error thrown when actions are missing parts or not correctly formed
	void malformedAction(int line) {
		cerr << ERROR_MESSAGE " Malformed action on line " << line << "\n";
		exit(1);
	}

	// Error thrown when no inputs are defined
	void noInputs() {
		cerr << ERROR_MESSAGE " Cannot create a finite-state machine without defined inputs\n";
		exit(1);
	}

	// Error thrown when no states are defined
	void noStates() {
		cerr << ERROR_MESSAGE " Cannot create a finite-state machine with no states\n";
		exit(1);
	}

	// Error thrown if an identifier is not valid
	void invalidIdentifier(int line, string id) {
		cerr << ERROR_MESSAGE " Invalid identifier '" << id << "' at line " << line << "\n";
		exit(1);
	}

	// Error thrown if a file does not exist
	void fileNotFound(int line, string path) {
		cerr << ERROR_MESSAGE " File '" << path << "'' declared at line " << line << " does not exist\n";
		exit(1);
	}

	// Error thrown if a user tries to define more than one input action
	void multipleInputActions(int line) {
		cerr << ERROR_MESSAGE " Only one input action is allowed per finite-state machine. A second input action was defined on line " << line << "\n";
		exit(1);
	}

	// Error thrown if given input delimiter 
	void invalidDelimiter(int line, string delim) {
		cerr << ERROR_MESSAGE " Invalid delimiter '" << delim << "' at line " << line << ". Delimiters must not be more than one character";
		exit(1);
	}

	// Error thrown if user tries to read or write from a file that was not declared
	void referencingUndeclaredFile(string identifier) {
		cerr << ERROR_MESSAGE " Attempting file I/O on undeclared file '" << identifier << "'\n";
		exit(1);
	}

	// Error thrown if user references undeclared input
	void referencingUndeclaredInput(string input) {
		cerr << ERROR_MESSAGE " Referencing undeclared input '" << input << "'\n";
		exit(1);
	}

	// Error thrown if user references undeclared state
	void referencingUndeclaredState(string state) {
		cerr << ERROR_MESSAGE " Referencing undeclared state '" << state << "'\n";
		exit(1);
	}

	// Error thrown if user does not define an input action
	void noInputActions() {
		cerr << ERROR_MESSAGE " At least one input action must be defined\n";
		exit(1);
	}

	// Error thrown if source file could not be opened
	void sourceOpenError(string path) {
		cerr << ERROR_MESSAGE " Source file at path '" << path << "' could not be opened. Does the file exist in the given path?\n";
		exit(1);
	}
}