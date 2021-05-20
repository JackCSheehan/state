#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include <cstdio>
#include "compiler.h"

using namespace std;

// Namespace with functions needed to throw errors
namespace Error {
	void endStateClash();
}

#endif