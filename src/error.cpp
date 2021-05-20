#include "error.h"

namespace Error {

	// Error thrown if user tries to define their own END state
	void endStateClash() {
		cerr << "Cannot create state named " << END_STATE << " as it is a reserved state name\n";
		exit(1);
	}
}