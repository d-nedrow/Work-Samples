// Daniel Nedrow
// CS 3100-01
// Dr. Raymer

#pragma once

#include <string>

using namespace std;

// Class for Student objects to be stored in the database.  Includes basic fields and their getters and setters,
// as well as a << operator for printing Student objects to the screen.
class Student
{
private:
	string lastName;
	string firstName;
	string major;
	string year;
	int uid;
public:
	// Construct a default student object
	Student();

	~Student();

	// Return value: the student's last name.
	string getLastName();

	// Sets the student's last name (which is taken in as a parameter).
	void setLastName(string);

	// Return value: the student's first name.
	string getFirstName();

	// Sets the student's first name (which is taken in as a parameter).
	void setFirstName(string);

	// Return value: the student's major.  Note: this function isn't used because the example input/output
	// doesn't show it.  However, major was a requested field in the specifications.
	string getMajor();

	// Sets the student's major.  Note: this function isn't used because the example input/output
	// doesn't show it.  However, major was a requested field in the specifications.
	void setMajor(string);

	// Return value: the student's year.
	string getYear();

	// Sets the student's year (which is taken in as a parameter).
	void setYear(string);

	// Return value: the student's UID.
	int getUID();

	// Sets the student's UID (which is taken in as a parameter).
	void setUID(int);

	// << operator for printing a Student object to the screen
	// Parameters: the output stream, a student object
	// Return value: the output stream (for operating chaining)
	friend ostream& operator<<(ostream& os, const Student& me) {
		os << "----------------------------" << endl;
		os << "Last name:  " << me.lastName << endl;
		os << "First name:  " << me.firstName << endl;
		os << "UID:  " << me.uid << endl;
		os << "Year:  " << me.year << endl;
		os << "----------------------------" << endl;
		return os;
	}
};

