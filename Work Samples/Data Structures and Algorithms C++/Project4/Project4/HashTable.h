// Daniel Nedrow
// CS 3100-01
// Dr. Raymer
// Note: the hash function is not original.  Citation given in function header.

#pragma once

#include <stdlib.h> // these two includes are for random #s
#include <time.h>

#include "Slot.h"

#define MAXHASH 1000

using namespace std;

// Template for a hash table class.  Includes a table of 1000 slots, an array of random indices for
// pseudo-random probing, and a variable to store the current number of records.  There are functions
// to insert, remove, and search records, as well as a hash and probe function.  There is a function which
// returns alpha, the current load factor.  The function randomize is provided to initialize and shuffle
// the array of indices for used for probing.  Finally, << is provided to print the hash table to the screen.
// A citiation for the borrowed hash function is in the function header.
template <class T> class HashTable
{
private:
	int numRecords;
	Slot<T> table[MAXHASH]; // the hash table of size 1000
	int randomPerm[MAXHASH]; // hold the shuffled indices for pseudo-random probing
public:
	// Construct a default hash table and shuffle an array of indices for probing
	HashTable() {
		numRecords = 0;
		randomize(randomPerm); // initialize and shuffle the indices array for probing
	}
	~HashTable() {}

	// Insert a new key/value pair into the table. Duplicate keys are not allowed. 
	// This function should return true if the key/value pair is successfully 
	// inserted into the hash table, and false if the pair could not be inserted 
	// (for example, due to a duplicate key already found in the table). If the 
	// insertion is successful, the number of collisions occuring during the insert
	// operation should be returned in collisions.
	// Parameters: the key to hash, any kind of value to store in the table, a variable
	// to store the number of collisions when probing
	// Return value: true if the key, value pair is inserted, false otherwise
	bool insert(int key, T value, int& collisions) {
		if (numRecords == MAXHASH - 1) { // We will not allow the hash table to become completely full
			cout << endl << "The hash table is too full for insert." << endl;
			return false;
		}

		collisions = 0;
		int home;
		unsigned int pos = home = hash(key); // find the home position of the given key
		for (int i = 1; table[pos].isNormal(); i++) { // probe as needed to find empty slot
			if (key == table[pos].getKey()) // if we have a duplicate key, do not insert
				return false;
			collisions++;
			pos = (home + probe(key, i)) % MAXHASH; // get the next probe position
		}

		Slot<T> temp(key, value); // when we find an unoccupied slot, create a record and insert it
		table[pos] = temp;
		numRecords++;
		return true;
	}

	// If there is a record with the given key in the hash table, it is deleted
	// and the function returns true; otherwise the function returns false.
	// Parameters: the key of the record to delete.
	// Return value: true if the record is deleted, false otherwise.
	bool remove(int key) {
		int home;
		unsigned int pos = home = hash(key); // find the home position of the given key
		for (int i = 1; !table[pos].isEmpty(); i++) { // probe as needed to find this key
			if (key == table[pos].getKey()) { // if we find the key in the table ...
				// If there is a tombstone for this key, it isn't really in the table.
				if (table[pos].isTombstone())
					return false;

				// Otherwise we found it, so remove it now.
				table[pos].kill(); // set slot as tombstone
				numRecords--;
				return true;
			}

			pos = (home + probe(key, i)) % MAXHASH; // get the next probe position
		}

		return false; // if we probed to an empty slot w/o finding key, give up on deleting
	}

	// If a record with the given key is found in the hash table, the function
	// returns true and a copy of the value is returned in value. Otherwise the
	// function returns false.
	// Parameters: the key of the record to find, a variable to hold the value associated with key.
	// Return value: true if the key is found, false if not.
	bool find(int key, T& value) {
		int home;
		unsigned int pos = home = hash(key); // find the home position of the given key

		// Probe as long as we haven't yet found the key and haven't found an empty slot.
		for (int i = 1; key != table[pos].getKey() && !table[pos].isEmpty(); i++) {
			pos = (home + probe(key, i)) % MAXHASH; // get the next probe position
		}

		// If we found the key in an occupied slot, store its value and return true.
		if (key == table[pos].getKey() && table[pos].isNormal()) {
			value = table[pos].getValue();
			return true;
		}
		else { // If the key wasn't found or if it was in a tombstone, we give up.
			return false;
		}
	}

	// Returns the current loading factor, alpha, of the hash table.
	// Return value: alpha.
	float alpha() {
		return (float)numRecords / MAXHASH;
	}

	// This hash function is from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
	// It is the 2nd answer on the page, by Thomas Mueller. I made the following 
	// changes: I accept an int parameter instead of unsigned int.  I also
	// added the first line of the function to convert back to unsigned int for return.
	// Finally, I added % MAXHASH to the return statement.
	// Parameters: the key to hash.
	// Return value: the hash value of the key.
	unsigned int hash(int key) {
		unsigned int x = key + 1; // avoid hashing key zero to zero every time
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		return x % MAXHASH;
	}

	// Pseudo-random probe function.  Although the key isn't used, we maintain the normal
	// signature of a probe function out of convention.
	// Parameters: the key, the number of times we have probed.
	// Return value: the current value of the probe sequence.
	int probe(int key, int i) {
		return randomPerm[i - 1];
	}

	// Takes an array the same size as the hash table, initializes the array to contain the indices
	// themselves, and then randomly shuffles those indices for use in pseudo-random probing.
	// Parameters: an integer array to initialize and shuffle.
	void randomize(int* randomPerm) {
		for (int i = 0; i < MAXHASH; i++) { // initialize array with indices themselves
			randomPerm[i] = i;
		}

		int temp, randomIndex;
		srand((unsigned int)time(NULL)); // random seed based on time
		for (int i = 0; i < MAXHASH; i++) { // randomly shuffle the array
			randomIndex = rand() % MAXHASH;
			temp = randomPerm[i];
			randomPerm[i] = randomPerm[randomIndex];
			randomPerm[randomIndex] = temp;
		}
	}

	// Overload the << operator for printing records.
	// Parameters: the output stream, a hash table.
	// Return value: the output stream (for operator chaining).
	friend ostream& operator<<(ostream& os, const HashTable& me) {
		os << endl << setw(9) << "Slot #" << setw(9) << "Key" << setw(9) << "Value" << endl;

		for (int i = 0; i < MAXHASH; i++) {
			if (me.table[i].isNormal()) {
				os << setw(9) << i << setw(9) << me.table[i].getKey() << setw(9) << me.table[i].getValue() << endl;
			}
		}

		cout << endl;
		return os;
	}
};

