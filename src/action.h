#ifndef ACTION_H
#define ACTION_H

#include <string>

using namespace std;

// Class used for storing parsed action info
class Action {
public:
	string name;		// Name of action (e.g. print)
	string identifier;	// Optional identifier that some actions have
	string arg;			// Action argument (e.g. "Hello World")

	Action() { name = ""; identifier = ""; arg = ""; }
	Action(string n, string a) { name = n; identifier =  ""; arg = a; }
	Action(string n, string i, string a) { name = n; identifier =  i; arg = a; }
	~Action(){}
};

#endif