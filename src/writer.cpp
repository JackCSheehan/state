#include "writer.h"

// Takes path to target file and pointers to parsed data
Writer::Writer(string p, map<string, string>* i, map<string, map<string, string>>* s, map<string, vector<Action>>* a) {
	f.open(p);
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
	f << "#include<cstdio>\n"
	  << "#include<string>\n"
	  << "using namespace std;\n";
}

// Declares enum for states in target file
void Writer::declareStates() {
	// Write enum for state
	f << "enum State {\n";
	for (auto& state : *states) {
		f << "\t" << state.first << ",\n";
	}

	// Write built-in END state
	f << "\t" << END_STATE << ",\n";

	f << "};\n";
}

// Writes main function and state change logic
void Writer::writeLogic() {
	// Write beginning of main function
	f << "int main() {\n";

	// Declare input variable, starting state, while loop opening, and switch head
	f << "\tchar in[" << INPUT_SIZE << "];\n"
	  << "\tState state = " << states->begin()->first << ";\n"
	  << "\twhile(state != " << END_STATE << ") {\n"
	  << "\t\tswitch(state) {\n";

	// Flag to determine if "if" should be written
	bool writeIf;

	// Write case for each state
	for (auto& state : *states) {
		f << "\t\tcase " << state.first << ":\n";

		writeIf = true;

		// Write if/else for each transition to change state
		for (auto& trans : state.second) {
			// Write if/else if branch
			if (writeIf) {
				f << "\t\t\tif(";
				writeIf =  false;
			} else {
				f<< "\t\t\telse if(";
			}

			// Write condition to determine which input is currently being analyzed
			f << "in == \"" << inputs->operator[](trans.first) << "\") {\n";
			
			// Write state transition logic
			f << "\t\t\tstate = "<< trans.second << ";\n";

			// Get output actions associated with this state
			vector<Action> currentActions = outputActions->operator[](state.first);

			// Iterate through actions and write them
			for (Action a : currentActions) {
				// Write tab to properly indent this action
				f << "\t\t\t";

				if (a.name == PRINT) writePrint(a.arg);
				else if (a.name == WRITE) writeWrite(a.arg);

				f << "\n";
			}

			// Close if statement
			f << "\t\t\t}\n";

		}

		f << "\t\t\tbreak;\n";
	}
	// Close switch, while loop, and main
	f << "\t\t}\n\t}\n\treturn 0;\n}";
}

// Writes a print action with given args. State's print action is equivalent to C's printf
void Writer::writePrint(string args) {
	f << "printf(" << args << ");\n";
}

// Writes a write action with given args. W
void Writer::writeWrite(string args) {
	f << "//fprintf(\n";
}

// Function to drive helper functions to compile to target language
void Writer::write() {
	writeIncludes();
	declareStates();
	writeLogic();
}