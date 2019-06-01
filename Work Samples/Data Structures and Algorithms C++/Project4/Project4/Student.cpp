#include "stdafx.h"
#include "Student.h"


Student::Student()
{
	lastName = "Holder";
	firstName = "Place";
	major = "Undecided";
	year = "Freshman";
	uid = 0;
}


Student::~Student()
{
}

string Student::getLastName() {
	return lastName;
}

void Student::setLastName(string newLastName) {
	lastName = newLastName;
}

string Student::getFirstName() {
	return firstName;
}

void Student::setFirstName(string newFirstName) {
	firstName = newFirstName;
}

string Student::getMajor() {
	return major;
}

void Student::setMajor(string newMajor) {
	major = newMajor;
}

string Student::getYear() {
	return year;
}

void Student::setYear(string newYear) {
	year = newYear;
}

int Student::getUID() {
	return uid;
}

void Student::setUID(int newUID) {
	uid = newUID;
}
