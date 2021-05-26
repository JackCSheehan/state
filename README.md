# State
A simple programming language used for creating finite-state machines.

### About
State is designed entirely to create finite-state machines. Its workflow involves defining inputs, defining states, and defining transition conditions. The logic is handled entirely by the compiler.

### Setting Up
*coming soon*

### Getting Started
The State compiler implemented in this repository does not enforce a particular file extension, but the convention is to use `.statelang` files.

### Example: Turnstile
Create a `.statelang` file. We'll start by implementing the famous [turnstile finite-state machine](https://en.wikipedia.org/wiki/Finite-state_machine#Example:_coin-operated_turnstile). Declare possible inputs using the `INPUT` keyword: 
```
INPUT push "push"
INPUT coin "coin"
```
The first argument after the `INPUT` keyword is the name of the input as you will refer to it in the code. The value in quotes is a specific input which your finite-state machine can take.

Declare states by using the `STATE` keyword:
```
STATE locked [coin: unlocked]
STATE unlocked [push: locked]
```
States also need transitions, which are defined in brackets (`[` and `]`). Inside the brackets, a comma-separated list of mappings are defined. These mappings tell State which state to switch to when the given input is encountered. Any inputs not included in the transition definition will simply become loops back to the current state.

Finally, State allows users to define an input source using the `SCAN` and `READ` keywords. In this example, we'll get input from the console using `SCAN` and we'll make the delimiter a newline character:
```
SCAN "\n"
```
Input delimiters can only be one character long unless you are defining an escape character, such as `\n` or `\t`.

Alternatively, developers can read data from files using the `READ` keyword. A file must be declared prior to being used for input using the `FILE` keyword:
```
FILE input "input.txt"
```
The first argument is an identifier name and the second argument is the path to the input file. Input can be read from a file using the `READ` keyword along with the file's identifier:
```
READ input "\n"
```

*more coming soon*