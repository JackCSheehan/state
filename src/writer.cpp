#include "writer.h"

// Takes path to target file and pointers to inputs and states. Also takes size of input C string array
Writer::Writer(string p, map<string, string>* i, map<string, map<string, string>>* s) {
	f.open(p);
	inputs = i;
	states = s;
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
	f << "};\n";
}

// Writes main function and state change logic
void Writer::writeLogic() {
	// Write beginning of main function
	f << "int main() {\n";

	// Declare input variable, starting state, while loop opening, and switch head
	f << "\tchar in[" << INPUT_SIZE << "];\n"
	  << "\tState state = " << states->begin()->first << ";\n"
	  << "\twhile(1) {\n"
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

			// Write state transition logic
			f << "in == \"" << inputs->operator[](trans.first) << "\") state = " << trans.second << ";\n";
		}
	}
	// Close switch, while loop, and main
	f << "\t\t}\n\t}\n}";
}

// Function to drive helper functions to compile to target language
void Writer::write() {
	writeIncludes();
	declareStates();
	writeLogic();
}