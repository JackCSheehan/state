#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include <cstdio>
#include "compiler.h"

using namespace std;

// Namespace with functions needed to throw errors
namespace Error {
	void endStateClash(int);
	void missingClosingBrace(int);
	void missingOpeningBrace(int);
	void unknownOutputAction(int);
	void incorrectlyPlacedInputAction(int);
	void malformedAction(int);
	void noInputs();
	void noStates();
}

#endif