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

	// Error thrown if input action are placed in state blocks
	void incorrectlyPlacedInputAction(int line) {
		cerr << ERROR_MESSAGE " Incorrectly place input action on line " << line << ". Cannot place input actions in state blocks\n";
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
		cerr << ERROR_MESSAGE " File " << path << " declared at line " << line << " does not exist\n";
		exit(1);
	}
}