#ifndef ACTION_H
#define ACTION_H

#include <string>

using namespace std;

// Class used for storing parsed action info
class Action {
public:
	string name;	// Name of action (e.g. print)
	string arg;		// Action argument (e.g. "Hello World")

	Action(string n, string a) { name = n; arg = a; }
	~Action(){}
};

#endif