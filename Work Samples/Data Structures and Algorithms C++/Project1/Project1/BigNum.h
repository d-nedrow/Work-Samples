#pragma once
#include <string>
#include <iostream>

using namespace std;

// A class for the individual nodes of a linked list to represent infinite-precision integers, or "BigNums".
class DigitNode
{
public:
	int digit; // each node will hold one integer digit of the infinitely long integer number
	DigitNode *next, *previous; // we use a doubly-linked list

	DigitNode() { // create a default node with no pointer to the next or previous element
		digit = 0;
		next = previous = nullptr;
	}
};

// This class implements the infinite-precision integers.  It can create them from a numerical string or digit-by-digit.
// The class contains the functions to implement addition, multiplication, and exponentiation of the numbers.
class BigNum
{
private:
	DigitNode *head, *tail;
public:
	BigNum() { // Create a default BigNum with no nodes.  Useful for building BigNum math results digit by digit.
		head = tail = nullptr;
	}
	
	BigNum(string seedNumber) {  // Create a complete BigNum from a numerical string.
		buildNumFromString(seedNumber);
	}
	
	~BigNum() { // Delete the individual nodes of a BigNum with the destructor.
		DigitNode *currentDigit, *nextDigit;
		currentDigit = head; // start deleting at head
		while (currentDigit) { // while there are nodes to delete, traverse the nodes of this BigNum
			nextDigit = currentDigit->next; // store a pointer to the next node before deleting current node
			delete currentDigit; // delete the current node
			currentDigit = nextDigit; // advance to the next node
		}
	}

	// This function builds a complete BigNum from a numerical string.
	void buildNumFromString(string number) {
		DigitNode *newDigit;
		newDigit = new DigitNode(); // create a node to hold first digit
		unsigned int i = 0; // i = 0 represents the first numerical character of the string
		newDigit->digit = number[i] - 48; // 48 is the asci for '0'.  Convert character to int and store in digit field.
		head = tail = newDigit; // point both the head and the tail of BigNum to this lone first digit
		
		for (i = 1; i < number.length(); i++) { // now examine every remaining character in the string
			newDigit = new DigitNode(); // create a node to hold each of these digits we examine
			newDigit->digit = number[i] - 48; // convert character to int and store in digit field of node
			tail->next = newDigit; // this new node comes after the old tail
			newDigit->previous = tail; // we note the new node's previous field points to the old tail
			tail = newDigit; // finally, the new node will be seen as the new tail
		}
	}

	// This function prints out a BigNum to the screen.
	void print() {
		DigitNode *currentDigit = head; // start from the head of the BigNum
		while (currentDigit) { // continue displaying digits of the BigNum while they exist
			cout << currentDigit->digit; // print the digit to the screen
			currentDigit = currentDigit->next; // advance to the next digit
		}
	}

	// This function adds a single node at the beggining of a BigNum object.  It is useful for building BigNums that are
	// the result of math operations.  The BigNum would be built from the least significant digit to the most.  The digit
	// field of the node will be filled with some integer passed to the function (the result of a math operation).
	void createDigitFromInt(int number) {
		DigitNode *newDigit;
		newDigit = new DigitNode(); // create a new node to place at beggining of BigNum
		newDigit->digit = number; // fill that node's digit field with the integer passed to the function

		if (!tail) { // if there is not yet a tail, this is the very first node we are adding
			tail = head = newDigit; // thus set the head and tail to both point to this new node
		}
		else { // if there is already a tail, we are merely adding a new node to the beggining of the BigNum
			newDigit->next = head; // the old head will come after this new node
			head->previous = newDigit; // the old head's previous field will thus point to the new node
			head = newDigit; // we now call the new node our new head
		}
	}

	// This function adds a second BigNum (taken in as a parameter) to the BigNum that calls the function.  The result
	// of the addition is then stored in a separate BigNum which the function returns a pointer to.
	BigNum* add(BigNum *numTwo) {
		BigNum *result = new BigNum(); // an empty BigNum to hold the result
		DigitNode *currentDigit = tail, *numTwoCurrentDigit = numTwo->tail; // examine from least significant digit
		int currentInt, numTwoCurrentInt, sum;
		int carry = 0; // initially, obviously there is no carry for addition

		while (currentDigit || numTwoCurrentDigit) { // continue addition if EITHER BigNum still has digits to add
			if (currentDigit) { // if the first BigNum still has digits to add
				currentInt = currentDigit->digit; // store that digit in an int variable
				currentDigit = currentDigit->previous; // ready to look at the next most significant digit next time
			}
			else
				currentInt = 0; // if there are no digits left in the first number, we regard the digit as zero for add
			
			if (numTwoCurrentDigit) { // if the second BigNum still has digits to add, the process is as above
				numTwoCurrentInt = numTwoCurrentDigit->digit; 
				numTwoCurrentDigit = numTwoCurrentDigit->previous; 
			}
			else
				numTwoCurrentInt = 0;

			sum = currentInt + numTwoCurrentInt + carry; // the sum of adding these two digits plus any previous carry
			carry = sum / 10; // the carry to send forward to the NEXT addition of digits
			sum = sum % 10; // the one digit of the sum that we would actually "write" as part of the result
			result->createDigitFromInt(sum); // create a new node on the beggining of the BigNum with that result
		}

		if (carry != 0) // after adding every digit of the BigNums, we must still consider if there was carry left over
			result->createDigitFromInt(carry); // if so, simply create one more node to hold the final carry digit
		return result; // return the pointer the the resulting BigNum sum
	}

	// This function multiplies a second BigNum (taken in as a parameter) to the BigNum that calls the function.  The
	// result of the multiplication is then stored in a separate BigNum which the function returns a pointer to.
	BigNum* multiply(BigNum *numTwo) {
		BigNum *result = new BigNum("0"); // we create a BigNum object for the answer "0"
		BigNum *temp1 = new BigNum(); // used to store intermediate results
		BigNum *temp2 = new BigNum(); // used as a swapping variable with "result"
		DigitNode *currentDigit = tail, *numTwoCurrentDigit = numTwo->tail; // start from least significant digits
		int product;
		int carry = 0, count = 0; // initially no carry, count tracks # of zeroes to append to end of intermediate products
		int currentInt = currentDigit->digit; // initially the least significant digit of first number
		int numTwoCurrentInt = numTwoCurrentDigit->digit; // initially the least significant digit of second number

		// Head == tail means a single digit number.  If either number is "0", return 0 as the result immediately.
		if ((currentInt == 0 && head == tail) || (numTwoCurrentInt == 0 && numTwo->head == numTwo->tail)) {
			delete temp1;
			delete temp2;
			return result;
		}
			
		// Note: when I say "intermediate result", I mean the number that comes from multiplying one digit of the "bottom"
		// number in grade-school multiplication with the entire "top" number of grade-school multiplication.  It is 
		// "intermediate" because it still needs to be added to find the final product.
		while (currentDigit) { // while the "bottom" number in grade-school multiplication has digits to multiply
			currentInt = currentDigit->digit; // store that digit in an int var

			while (numTwoCurrentDigit) { // while the "top" number in grade-school multiplication has digits to multiply
				numTwoCurrentInt = numTwoCurrentDigit->digit; // store that digit in an int var
				product = currentInt * numTwoCurrentInt + carry; // the product of the two digits plus any previous carry
				carry = product / 10; // the carry to send forward to the NEXT multiplication of digits
				product = product % 10; // the digit of the product we would actually "write" in intermediate result
				temp1->createDigitFromInt(product); // create a new node at the beggining of intermediate result
				numTwoCurrentDigit = numTwoCurrentDigit->previous; // move focus to next digit to left on "top" number
			}
			if (carry != 0) { // after we examine all digits of the "top" number, we must still consider left-over carry
				temp1->createDigitFromInt(carry); // simply create one more node to hold final carry digit
				carry = 0; // reset carry for next round of multiplying (as we traverse "bottom" number)
			}
			temp2 = result; // hold the sum of previous intermediate results in a temporary variable
			result = temp2->add(temp1); // add current intermediate result to the sum of previous intermediate results
			delete temp1; // delete these temporary BigNums to avoid memory leaks
			delete temp2;
			currentDigit = currentDigit->previous; // move focus to next digit to left for "bottom" number
			if (currentDigit) { // if we still have digits to multiply on the "bottom" number
				numTwoCurrentDigit = numTwo->tail;  // reset focus to rightmost digit for "top" number (as in grade school)
				count++; // track iterations of outer loop to right-fill appropriate zeroes on next intermediate result
				temp1 = new BigNum(); // we need to set up a BigNum to hold the right-filled zeroes
				for (int i = 0; i < count; i++) { // right-fill zeroes on the next intermediate result (as in grade school)
					temp1->createDigitFromInt(0); 
				}
			}
		}
		return result; // return a pointer to the BigNum that is the final product
	}

	// This function simply converts this BigNum object to an int type, and returns the int.  The old BigNum
	// should of course be deleted by a calling function (see exponentiateNums() in "RPNStack.h" for actual deletion).
	int bigNumToInt() {  
		DigitNode *current = tail; // we start our conversion from the least significant digit
		int result = 0;
		int multiplier = 1; // tracks how much to multiply each BigNum digit by (as in grade school place value)

		while (current) { // while there are digits remaining in the BigNum
			result += multiplier * current->digit; // add to the result the place value of the current digit
			multiplier *= 10; // increase the place-value multiplier for the next most significant digit
			current = current->previous; // move our focus to the next most significant digit
		}

		return result; // return the resulting int, which is only valid if BigNum was less than approximately 2 billion
	}
	
	// This function raises a BigNum to the power of a second BigNum (taken in as a parameter).  The result of the
	// exponentiation is then stored in a separate BigNum which the function returns a pointer to.
	BigNum* exponentiate(BigNum *numTwo) {
		int exponent = numTwo->bigNumToInt(); // Exponent into int.  Note BigNum deletion in RPNStack.h calling function.
		BigNum *result = new BigNum("1"); // we should prepare a result of "1" in case the exponent is zero
		for (; exponent > 0; exponent--) { // multiply "1" by the base repeatedly an "exponent" number of times
			BigNum *temp = new BigNum();
			temp = result;                 // hold old result of repeated multiplication
			result = this->multiply(temp); // result now inlcludes another round of multiplication
			delete temp;                   // delete old result to avoid memory leaks
		}

		return result; // return the first number raised to the power of the second
	}

	
};

