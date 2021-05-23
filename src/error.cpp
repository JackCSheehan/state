#include "error.h"

namespace Error {

	// Error thrown if user tries to define their own END state
	void endStateClash(int line) {
		cerr << "[ERROR] Cannot create state named " << END_STATE << " on line " << line << " as it is a reserved state name\n";
		exit(1);
	}

	// Error thrown if a block is not ended before another one is started
	void missingClosingBrace(int line) {
		cerr << "[ERROR] Missing closing brace on line " << line << "\n";
		exit(1);
	}

	// Error thrown if a block is closed without one being opened
	void missingOpeningBrace(int line) {
		cerr << "[ERROR] Missing opening brace on line " << line << "\n";
		exit(1);
	}

	// Error thrown if user tries to run an unknown output action
	void unknownOutputAction(int line) {
		cerr << "[ERROR] Unknown output action on line " << line << "\n";
		exit(1);
	}

	// Error thrown if input action are placed in state blocks
	void incorrectlyPlacedInputAction(int line) {
		cerr << "[ERROR] Incorrectly place input action on line " << line << ". Cannot place input actions in state blocks\n";
		exit(1);
	}

	// Error thrown when actions are missing parts or not correctly formed
	void malformedAction(int line) {
		cerr << "[ERROR] Malformed action on line " << line << "\n";
		exit(1);
	}

	// Error thrown when no inputs are defined
	void noInputs() {
		cerr << "[ERROR] Cannot create a finite-state machine without defined inputs\n";
		exit(1);
	}

	// Error thrown when no states are defined
	void noStates() {
		cerr << "[ERROR] Cannot create a finite-state machine with no states.\n";
		exit(1);
	}
}