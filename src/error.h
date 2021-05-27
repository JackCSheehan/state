#ifndef ERROR_H
#define ERROR_H

#define ERROR_MESSAGE "[ERROR]"

#include <iostream>
#include <string>
#include "compiler.h"

using namespace std;

// Namespace with functions needed to throw errors
namespace Error {
	void endStateClash(int);
	void missingClosingBrace(int);
	void missingOpeningBrace(int);
	void unknownOutputAction(int);
	void malformedAction(int);
	void noInputs();
	void noStates();
	void invalidIdentifier(int, string);
	void multipleInputActions(int);
	void invalidDelimiter(int, string);
	void referencingUndeclaredFile(string);
	void referencingUndeclaredInput(string);
	void referencingUndeclaredState(string);
	void noInputActions();
	void sourceOpenError(string);
	void unknownStatement(int, string);
}

#endif