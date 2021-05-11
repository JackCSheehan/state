#include "writer.h"

// Takes path to target file and pointers to inputs and states
Writer::Writer(string p, map<string, string>* i, map<string, map<string, string>>* s) {
	f.open(p);
	inputs = i;
	states = s;
}

// Closes file writer
Writer::~Writer() {
	f.close();
}

// Declares enum for alphabet in target file
void Writer::declareAlphabet() {
	// Write enum for input alphabet
	f << "enum InputAlphabet {\n";
	for (auto& input : *inputs) {
		f << "\t" << input.first << " = \"" << input.second << "\",\n";
	}
	f << "};\n";
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
}

// Function to drive helper functions to compile to target language
void Writer::write() {
	declareAlphabet();
	declareStates();
	writeLogic();
}