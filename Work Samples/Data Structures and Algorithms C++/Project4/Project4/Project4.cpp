// Project4.cpp : Defines the entry point for the console application.
// Daniel Nedrow
// CS 3100-01
// Dr. Raymer

#include "stdafx.h"
#include "HashTable.h"
#include "Student.h"

using namespace std;

// function headers given below main()
void insert(HashTable<Student>&);
void remove(HashTable<Student>&);
void search(HashTable<Student>&);

// Driver to allow for insert, delete, and search of student records in hash table.
int main()
{
	HashTable<Student> table;
	bool done = false;
	char choice;

	while (!done) { // continue prompting user to insert, delete, search, until they quit
		cout << "Would you like to (I)nsert or (D)elete a record, or (S)earch " <<
			"for a record, or (Q)uit?" << endl << "Enter action:  ";
		cin >> choice;
		switch (choice) { // respond to user's choice of action to insert, delete, search, quit
		case 'I':
			insert(table);
			break;
		case 'D':
			remove(table);
			break;
		case 'S':
			search(table);
			break;
		case 'Q':
			done = true; // loop exit
			cout << "Exiting." << endl;
			break;
		default:
			cout << "Please input a valid choice." << endl << endl;
		}
	}

	system("pause");
	return 0;
}

// Allow user to input fields of Student record to insert into hash table, then insert.
// Parameter: the hash table we will insert the Student record into.
void insert(HashTable<Student>& table) {
	Student student;
	string lastName, firstName, year;
	int uid, collisions;
	bool inserted;

	// grab user input and set as fields of Student object
	cout << "Inserting a new record." << endl << "Last name:  ";
	cin >> lastName;
	cout << "First name:  ";
	cin >> firstName;
	cout << "UID:  ";
	cin >> uid;
	cout << "Year:  ";
	cin >> year;
	student.setFirstName(firstName);
	student.setLastName(lastName);
	student.setUID(uid);
	student.setYear(year);

	inserted = table.insert(uid, student, collisions); // see if insert worked

	if (inserted) // let user know if insert worked
		cout << "Record inserted." << endl << endl;
	else
		cout << "Unable to insert. Check for duplicate key." << endl << endl;
}

// Allow user to input uid of Student they want removed from hash table, and remove.
// Parameter: the hash table we will delete the Student record from.
void remove(HashTable<Student>& table) {
	int uid;
	bool removed;
	cout << "Enter UID of student to remove:  "; // get key from user
	cin >> uid;

	removed = table.remove(uid); // see if deletion worked

	if (removed) // let user know if deletion worked
		cout << "Record deleted." << endl << endl;
	else
		cout << "There was no such record to delete." << endl << endl;
}

// Allow user to input uid of Student to find in the hash table, and find it.
// Parameter: the hash table to search.
void search(HashTable<Student>& table) {
	int uid;
	Student student;
	bool found;

	cout << "Enter UID to search for:  "; // get key from user
	cin >> uid;

	found = table.find(uid, student); // see if record was found

	if (found) { // let user know if record was found
		cout << "Searching... record found" << endl;
		cout << student; // print the record using Student's output operator
		cout << endl;
	}
	else {
		cout << "Searching... record not found" << endl << endl;
	}
}