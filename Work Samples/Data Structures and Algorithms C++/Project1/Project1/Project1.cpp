// Project1.cpp : Defines the entry point for the console application.
// Daniel Nedrow
// CS 3100-01
// Dr. Raymer

#include "stdafx.h"
#include "Split.h"
#include "BigNum.h"
#include "RPNStack.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> getInput();
string trimLeadingZeroes(string);

int main()
{
	RPNStack *stackNums = new RPNStack(); // create the stack to hold operands

	cout << "You may begin input at any time.  The program will continue until you close it." << endl;
	
	while (true) { // continue allowing input until program is closed
		vector<string> tokens = getInput(); // call function to allow user to input an expression
		for (unsigned int i = 0; i < tokens.size(); i++) { // examine every operand of expression
			tokens[i] = trimLeadingZeroes(tokens[i]); // trim the leading zeroes of each operand
			cout << tokens[i] << " "; // echo the input without leading zeroes, extra spaces, or linefeeds
		}
		cout << endl;
		stackNums->processInput(tokens); // determine which math operations to perform on the stack of input
		cout << endl;
	}

	system("pause");
	return 0;
}

// This function gets an entire input expression from the user, stores it in a vector, and returns the vector.
vector<string> getInput() {
	string input = "", nextLine;
	vector<string> tokens;
	
	do { // continue reading the input expression until we encounter a blank line
		getline(cin, nextLine); // read a line of input
		input += nextLine + " "; // append the line to the input expression
	} while (!nextLine.empty());

	split(input, tokens); // split each space-separated part of the input into entries in a vector

	return tokens; // return the vector containing the input expression
}

// This function removes leading zeroes from a single numerical string, and returns the formatted string.
string trimLeadingZeroes(string number) {
	unsigned int position = 0, done = 0;
	
	while (position < number.length() - 1 && !done) { // trim zeroes until one digit is left, or we find non-zero
		if (number[position] == '0') // if we are still finding leading zeroes, advance our position in the string
			position++;
		else // if we find any non-zero, mark the loop as done
			done = 1;
	}

	return number.substr(position); // return a substring which discards the leading zeroes
}