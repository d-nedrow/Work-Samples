#pragma once
#include <iostream>
#include <string>

#define ARRAYSIZE 100

using namespace std;

// This class provides a stack on which to put BigNum operands.  It also provides the functions to determine which
// operations need to be performed, and to call functions from the BigNum class to execute those operations.
class RPNStack
{
private:
	BigNum *stackOfNums[ARRAYSIZE]; // the stack will contain pointers to BigNum objects
	int topIndex, error;
	
public:
	RPNStack() { // create a default stack
		topIndex = -1; // we use index = -1 to mean the stack is empty, nothing at index 0
		error = 0; // we will track whether the number of operands/operators is consistent
	}
	~RPNStack(); // we have no object-type members in this class, so we provide no destructor here

	// This function adds a BigNum pointer to the top of the stack.
	void push(BigNum *num) {
		stackOfNums[++topIndex] = num; // pre-increment to avoid writing to occupied or invalid space
	}

	// This function removes and returns the top BigNum pointer on the stack.
	BigNum* pull() {
		return stackOfNums[topIndex--]; // post-decrement to grab top before shrinking stack
	}

	// This function tests if the stack is empty, returns 1 for empty, 0 for non-empty.
	int empty() {
		return (topIndex == -1);
	}

	// This function pulls the top two BigNums off the stack.  If they exist, it adds them.  If not, it marks an error 
	// state and leaves the function.  It pushes the result of the add back onto the stack. 
	void addNums() {
		BigNum *numOne, *numTwo;
		
		if (!empty()) { // test if there are any numbers left on the stack
			numTwo = pull(); // we assign the top number as "numTwo" as a convention
		}
		else {
			error = 1;
			return; // no operands on the stack means we have an error and should leave
		}

		if (!empty()) { // repeat the test to get another number
			numOne = pull();
		}
		else {
			error = 1;
			return;
		}
			
		push(numOne->add(numTwo)); // add the numbers together and push the result back onto the stack
		delete numOne; // delete the operands themselves to avoid a memory leak
		delete numTwo;
	}

	// This function pulls the top two BigNums off the stack.  If they exist, it multiplies them.  If not, it marks an 
	// error state and leaves the function.  It pushes the result of the multiplication back onto the stack.
	void multiplyNums() {
		BigNum *numOne, *numTwo;

		if (!empty()) { // test if there are any numbers left on stack
			numTwo = pull(); // top number is "numTwo" as a convention
		}
		else {
			error = 1;
			return; // no operands on stack means we have an error and should leave
		}

		if (!empty()) { // repeat the test to get another number
			numOne = pull();
		}
		else {
			error = 1;
			return;
		}

		push(numOne->multiply(numTwo)); // multiply the numbers and push the result back on the stack
		delete numOne; // delete the operands to avoid a memory leak
		delete numTwo;
	}

	// This function pulls the top two BigNums off the stack.  If they exist, it exponentiates them.  If not, it marks an 
	// error state and leaves the function.  It pushes the result of the exponentiation back onto the stack.
	void exponentiateNums() {
		BigNum *numOne, *numTwo;

		if (!empty()) { // test if there any numbers left on the stack
			numTwo = pull(); // importantly, top number is the exponent
		}
		else {
			error = 1;
			return; // no operands on the stack means we have an error and should leave
		}

		if (!empty()) { // repeat the test to get another number
			numOne = pull(); // the bottom number is the base
		}
		else {
			error = 1;
			return;
		}

		push(numOne->exponentiate(numTwo)); // raise the bottom number to the top number, push result onto stack.
		delete numOne; // delete the operands to avoid a memory leak
		delete numTwo;
	}

	// This function eliminates all operands from the stack, and deletes them to avoid a memory leak.  The function
	// is useful if we had an error and want to purge the remaining stack, or if we are done with an expression.
	void clear() {
		for (; topIndex >= 0; topIndex--) // delete every BigNum on the stack, get to empty state of topIndex = -1
			delete stackOfNums[topIndex];
	}
	
	// This function takes in the input expression as a vector, and examines each item in the input.  If we encounter a
	// number, we push it onto the stack.  If we encounter an operator, we call the function to handle the operation.
	// If we encounter an error related to an inconsistent number or operands and operators, we print the error to the 
	// screen, and reset the error state.  Finally, we print the result of the expression to the screen and clear the
	// stack.
	void processInput(vector<string> tokens) {
		for (unsigned int i = 0; i < tokens.size(); i++) { // examine each item in the input and process accordingly
			if (error == 1) // if another function returned an error state, stop input processing immediately
				break;
			else if (tokens[i] == "+") // add, multiply, or exponentiate if we encounter such operators
				addNums();
			else if (tokens[i] == "*")
				multiplyNums();
			else if (tokens[i] == "^")
				exponentiateNums();
			else                       // otherwise assume we encountered a number and push it onto the stack
				push(new BigNum (tokens[i])); // these anonymous objects will be deleted after operations on them
		}

		if (error == 1 || topIndex != 0) { // error is either reported, or in the form of operands still left on stack
			cout << "Expression Error" << endl;
			error = 0;                     // reset error state for next expression
		}
		else {                             // if we were left with a single result number on the stack and nothing else
			stackOfNums[0]->print();       // print the result to the screen
			cout << endl;
		}

		clear();                          // in any case, completely remove and delete all BigNums on the stack
	}
};

