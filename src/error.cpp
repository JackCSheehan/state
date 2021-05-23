#include "error.h"

namespace Error {

	// Error thrown if user tries to define their own END state
	void endStateClash(int line) {
		cerr << "Cannot create state named " << END_STATE << " on line " << line << " as it is a reserved state name\n";
		exit(1);
	}

	// Error thrown if a block is not ended before another one is started
	void missingClosingBrace(int line) {
		cerr << "Missing closing brace on line " << line << "\n";
		exit(1);
	}

	// Error thrown if a block is closed without one being opened
	void missingOpeningBrace(int line) {
		cerr << "Missing opening brace on line " << line << "\n";
		exit(1);
	}

	// Error thrown if user tries to run an unknown output action
	void unknownOutputAction(int line) {
		cerr << "Unknown output action on line " << line << "\n";
		exit(1);
	}

	void incorrectlyPlacedInputAction(int line) {
		cerr << "Incorrectly place input action on line " << line << ". Cannot place input actions in state blocks\n";
		exit(1);
	}

	void malformedAction(int line) {
		cerr << "Malformed action on line " << line << "\n";
		exit(1);
	}
}