# State
A simple programming language used for creating finite-state machines.

### About
State is designed to easily create finite-state machines. Its workflow involves defining inputs, defining states, then defining transition conditions. The State compiler compiles source code to a single C++ file which can then be compiled to create an executable.

### Setting Up
To compile from source, download the source code from the latest release. Navigate to the source directory in a terminal and run a compiler on the source files. The exact compile command will depend on which C++ compiler you're using.

Alternatively, if you're on Windows, you can download an `.exe` from the releases tab directly.

To compile your State source file run the following command:
`$ statec.exe path/to/file.statelang`

The State compiler implemented in this repository does not enforce a particular file extension, but the convention is to use `.statelang`.

### Language Overview by Example
Create a `.statelang` file. We'll start by implementing the famous [turnstile finite-state machine](https://en.wikipedia.org/wiki/Finite-state_machine#Example:_coin-operated_turnstile). Declare possible inputs using the `INPUT` keyword: 
```
INPUT push "push"
INPUT coin "coin"
```
The first argument after the `INPUT` keyword is the name of the input as you will refer to it in the code. The value in quotes is a specific input which your finite-state machine can take. Inputs received which are not defined as part of the machine's language will be ignored.

Declare states by using the `STATE` keyword:
```
STATE locked [coin: unlocked]
STATE unlocked [push: locked]
```
The first state defined in the file is the state that the finite-state machine will start at.

States also need transitions, which are defined in brackets (`[` and `]`). Inside the brackets, a comma-separated list of mappings are defined. These mappings tell the compiler which state to switch to when the given input is encountered. Any inputs not included in the transition definition will simply become loops back to the state itself.

State allows users to define an input source using the `SCAN` and `READ` keywords. In this example, we'll get input from the console using `SCAN` and we'll make the delimiter a newline character:
```
SCAN "\n"
```
Input delimiters can only be one character long unless you are defining an escape character, such as `\n` or `\t`. Delimiters that are longer than one character will simply have every character but the first ignored.

Alternatively, developers can read data from files using the `READ` keyword. A file must be declared prior to being used for input using the `FILE` keyword:
```
FILE input "input.txt"
```
The first argument is an identifier name and the second argument is the path to the input file. Input can be read from a file using the `READ` keyword along with the file's identifier:
```
READ input "\n"
```
Only one input action (`READ` and `SCAN`) can be defined in one `.statelang` file.

State also allows developers to attach output actions to each state. Output actions are run once a state is transitioned to. Use braces to attach output actions to a state:
```
STATE locked [coin: unlocked] {
	PRINT "Locked!!\n"
}
STATE unlocked [push: locked] {
	PRINT "Unlocked!\n"
}
```
Use `PRINT` to print directly to the console. `WRITE` could also have been used to write to a declared file:
```
STATE locked [coin: unlocked] {
	WRITE output "Unlocked!\n"
}
STATE unlocked [push: locked] {
	PRINT output "Locked!\n"
}
```

Finally, use `//` to write a comment:
```
// This is a comment!
```

### Keyword and Action Reference
Keyword/Action|Description
-|-
`INPUT`|Used to define a valid input to the finite-state machine. Input value must always been in quotes.
`STATE`|Used to define a valid state in the finite-state machine. Transitions can be defined using square brackets in the form `[input : new state, input : new state...]`. Sink states can be indicated using a blank set of square brackets (`[]`). State declarations can have bodies of code between braces (`{` and `}`). Output actions (`PRINT` and `WRITE`) are only allowed inside the bodies of states. State body actions are run as soon as the state is switched to, making them ideal for indicating when an accept state has been reached. The starting state's body will be run as soon as the program starts. State bodies are optional.
`FILE`|Used to declare a file for input or output. The file will be created if it does not exist. **Caution:** A file can be both written to and read from in the same program. This may lead to confusing results. Be aware!
`PRINT`|Prints the given value in quotes to the console.
`WRITE`|Writes the given value in quotes to the given file.
`SCAN`|Reads input from the console separated by the given delimiter.
`//`|Creates a comment. Comments can be on the same line as other statements, but a line starting with a comment symbol will be entirely ignored.
`$in`|Used when the current output needs to be printed to the console or written to a file. Simply use `$in` in the quotes of a `PRINT` or `WRITE` to display the most recent input. e.g. `PRINT "The input is: $in"`.
`END`|`END` is a built-in state. The program will end when either the `END` state has been reached or the file being read from no longer has any data left to read. The State compiler will throw an error if the user attempts to manually define the `END` state.

### Notes
This project was intended to allow me to practice writing a simple compiler in preparation for a potential future, much larger compiler project. State is not designed to be used for complex applications and instead was simply a fun project that challenged my skills in ways that they have not been challenged before.

A note on terminology. While it can be argued that the code presented here is actually a *transpiler* rather than a *compiler,* I have chosen the term compiler due to the great difference in abstraction between C++ and State source code.
