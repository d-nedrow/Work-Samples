// CEG4350_Project.cpp : Defines the entry point for the console application.
// Daniel Nedrow
// CEG 4350
// Dr. Yong Pei

#include "stdafx.h"
#include "CPUScheduler.h"

// Driver for CPU Scheduling Simulation. Allows scheduling as FCFS, RR, or MLFQ.
int main(int argc, char *argv[])
{
	ProcessBank processBank; // we will store not-yet-arrived processes from input file here
	
	// if the user enters the file name and scheduling choice on the command line, bypass interactive input
	if (argc == 3) {
		// if the file name specified can't be read, let the user know
		if (!processBank.read(argv[1])) 
			cout << "Could not open specified input file." << endl;
		else { // if we read the file into our process bank, then select scheduling algorithm
			CPUScheduler scheduler(processBank); // our scheduler will use the sorted list of processes
			
			cout << "  Selected Scheduling algorithm: " << argv[2] << endl;
			
			// select appropriate scheduling algorithm, or inform user of illegal choice
			if ((string)argv[2] == "FCFS") 
				scheduler.scheduleAsFCFS();
			else if ((string)argv[2] == "RR") 
				scheduler.scheduleAsRoundRobin();
			else if ((string)argv[2] == "MLFQ") 
				scheduler.scheduleAsMLFQ();
			else 
				cout << "Usage: Enter input file name followed by FCFS or RR or MLFQ" << endl;
		}
	}
	else { // if the user didn't use the command line, prompt for file name and scheduling choice
		string inputFile, scheduleChoice;
		cout << "Please enter name of input file: ";
		cin >> inputFile;
		
		// same logic as command line if block above
		if (!processBank.read(inputFile))
			cout << "Could not open specified input file." << endl;
		else {
			CPUScheduler scheduler(processBank);
			
			cout << "Please enter FCFS or RR or MLFQ: ";
			cin >> scheduleChoice;
			
			cout << "  Selected Scheduling algorithm: " << scheduleChoice << endl;

			if (scheduleChoice == "FCFS")
				scheduler.scheduleAsFCFS();
			else if (scheduleChoice == "RR")
				scheduler.scheduleAsRoundRobin();
			else if (scheduleChoice == "MLFQ")
				scheduler.scheduleAsMLFQ();
			else
				cout << "Usage: enter FCFS or RR or MLFQ." << endl;
		}
	}
	
	system("pause");
	return 0;
}