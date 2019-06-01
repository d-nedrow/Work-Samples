// Daniel Nedrow
// CEG 4350
// Dr. Yong Pei
#pragma once

#include "Process.h"
#include <fstream>
#include <string>

// This class stores and sorts the entire list of processes from the input file. It is considered that the processes
// in the bank have not yet arrived into the ready queue. The queue will only draw processes from this bank at the 
// appropriate arrival times.
class ProcessBank
{
private:
	Process *processBank;
	int size, capacity, currentProcess;
public:
	// Create a new process bank with size 0, and an initial capacity of 10.
	ProcessBank() {
		size = currentProcess = 0;
		capacity = 10;
		processBank = new Process[capacity];
	}
	~ProcessBank() {}

	// Double the capacity of the process bank (if there are more processes to read from the input file).
	void moreCapacity() {
		capacity *= 2;
		Process *processBankTemp = new Process[capacity];
		for (int i = 0; i < size; i++) {
			processBankTemp[i] = processBank[i];
		}

		delete[] processBank;
		processBank = processBankTemp;
	}

	// Read the processes from the input file and store in the process bank.
	// Paramater: the name of the file to read from.
	// Return value: true if the file could be opened, false if not.
	bool read(string filename) {
		int pid, arrivalTime, burstTime;
		ifstream inFile(filename);
		
		if (!inFile.is_open()) // if the file can't open, return false
			return false;
		
		// ignore any non-numeric table headers at the start of the input file
		while (!(inFile >> pid && inFile >> arrivalTime && inFile >> burstTime)) {
			inFile.clear();
			inFile.ignore();
		}

		Process temp1(pid, arrivalTime, burstTime); // store first process in bank
		processBank[size++] = temp1;

		// store all remaining processes in bank
		while (inFile >> pid && inFile >> arrivalTime && inFile >> burstTime) {
			if (size == capacity) moreCapacity();
			Process temp2(pid, arrivalTime, burstTime);
			processBank[size++] = temp2;
		}

		inFile.close();
		sortByArrivalTime(); // in the event that the processes were not sorted by arrival time, sort them now
		return true; // indicate successful reading of file
	}

	// Sort the processes in the bank by arrival time.
	void sortByArrivalTime() {
		int minArrivalTime, indexOfMinArrival;
		Process temp;
		
		// selection sort algorithm for sorting the array by arrival time
		for (int i = 0; i < size; i++) {
			indexOfMinArrival = i;
			minArrivalTime = processBank[indexOfMinArrival].getArrivalTime();
			for (int j = i + 1; j < size; j++) {
				if (processBank[j].getArrivalTime() < minArrivalTime) {
					minArrivalTime = processBank[j].getArrivalTime();
					indexOfMinArrival = j;
				}
			}

			temp = processBank[i];
			processBank[i] = processBank[indexOfMinArrival];
			processBank[indexOfMinArrival] = temp;
		}
	}

	// Return value: the next process in the bank.
	Process getNextProcessFromBank() {
		return processBank[currentProcess++];
	}

	// Return value: the arrival time of the next process in the bank.
	int getNextArrivalTime() {
		return processBank[currentProcess].getArrivalTime();
	}

	// Return value: the size of the process bank.
	// Note: size means the number of processes read from file. After file read, it never changes.
	int getSize() {
		return size;
	}
};