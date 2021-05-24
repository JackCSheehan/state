#include "writer.h"

// Takes path to target file and pointers to parsed data
Writer::Writer(string p, map<string, string>* f, map<string, string>* i, map<string, map<string, string>>* s, map<string, vector<Action>>* a) {
	this->f.open(p);
	files = f;
	inputs = i;
	states = s;
	outputActions = a;
}

// Closes file writer 
Writer::~Writer() {
	f.close();
}

// Writes imports needed in compiled code
void Writer::writeIncludes() {
	f << "#include<iostream>\n"
	   	 "#include<fstream>\n"
	     "#include<string>\n"
	     "using namespace std;\n";
}

// Declares enum for states in target file
void Writer::declareStates() {
	// Write enum for state
	f << "enum State {\n";
	for (pair<string, map<string, string>> state : *states) {
		f << "\t" << state.first << ",\n";
	}

	// Write built-in END state
	f << "\t" << END_STATE << ",\n";

	f << "};\n";
}

// Writes declarations of files declared by user
void Writer::writeFileDeclarations() {
	// Iterate through files and write each one as an f stream
	for (pair<string, string> file : *files) {
		// Declare current file as fstream object
		f << "\tfstream " << file.first << "(\"" << file.second << "\", fstream::in | fstream::out);\n";
	}
}

// Writes main function and state change logic
void Writer::writeLogic() {
	// Write beginning of main function
	f << "int main() {\n";

	// Write declaration of file objects
	writeFileDeclarations();

	// Declare input variable, starting state, while loop opening, and switch head
	f << "\tchar " IN " [" << INPUT_SIZE << "];\n"
	  << "\tState state = " << states->begin()->first << ";\n"
	  << "\twhile(state != " << END_STATE << ") {\n"
	  << "\t\tswitch(state) {\n";

	// Flag to determine if "if" should be written
	bool writeIf;

	// Write case for each state
	for (pair<string, map<string, string>> state : *states) {
		f << "\t\tcase " << state.first << ":\n";

		writeIf = true;

		// Write if/else for each transition to change state
		for (pair<string, string> trans : state.second) {
			// Write if/else if branch
			if (writeIf) {
				f << "\t\t\tif(";
				writeIf =  false;
			} else {
				f<< "\t\t\telse if(";
			}

			// Write condition to determine which input is currently being analyzed
			f << IN " == \"" << inputs->operator[](trans.first) << "\") {\n";
			
			// Write state transition logic
			f << "\t\t\t\tstate = "<< trans.second << ";\n";

			// Get output actions associated with this state
			vector<Action> currentActions = outputActions->operator[](state.first);

			// Iterate through actions and write them
			for (Action a : currentActions) {
				// Write tab to properly indent this action
				f << "\t\t\t\t";

				writeOutputAction(a);

				f << "\n";
			}

			// Close if statement
			f << "\t\t\t}\n";
		}

		f << "\t\t\tbreak;\n";
	}
	// Close switch and while loop
	f << "\t\t}\n\t}\n";

	// Close files
	writeFileCloses();
}

// Writes the given output action with given args
void Writer::writeOutputAction(Action action) {
	// Get size of IN variable name
	static const int IN_LEN = string(IN_MARKER).size();

	// Get position of first occurrence of the IN marker
	size_t inPos = action.arg.find(IN_MARKER);

	// Vector of positions in line where the in marker is used
	vector<size_t> inReferences;

	// Find all occurrences of the in marker in this action's arg
	while (inPos != string::npos) {
		// Add position of reference to list
		inReferences.push_back(inPos);

		// Find next reference to the in marker
		inPos = action.arg.substr(inPos + IN_LEN).find(IN_MARKER);
	}

	// Replace each reference to in with the name of the in variable
	for (int pos : inReferences) {
		action.arg.replace(pos, IN_LEN, "\" << " IN " << \"");
	}

	// Write statement depending on which output action this line has
	if (action.name == PRINT) {
		f << "cout << \"" << action.arg << "\";";
	} else {
		f << action.identifier << " << \"" << action.arg << "\";";
	}
}

// Writes closing statements for ifstream
void Writer::writeFileCloses() {
	// Iterate through files and write closes for each one
	for (pair<string, string> file : *files) {
		// Declare current file as fstream object
		f << "\t" << file.first << ".close();\n";
	}
}

// Writes closing for main function
void Writer::writeMainClose() {
	f << "\treturn 0;\n}";
}

// Function to drive helper functions to compile to target language
void Writer::write() {
	writeIncludes();
	declareStates();
	writeLogic();
	writeMainClose();
}