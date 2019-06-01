// Daniel Nedrow
// CEG 4350
// Dr. Yong Pei
#pragma once

#include <iostream>

using namespace std;

// The class to represent processes. It includes only trivial functions to get and set the data fields.
class Process
{
private:
	int pid, arrivalTime, burstTime, lastPreemptedTime;
	bool previouslyServiced;
public:
	// Create a default process.
	Process() {
		pid = -1;
	}

	// Create a process with the specified, pid, arrival time and burst time.
	Process(int newPid, int newArrivalTime, int newBurstTime) {
		pid = newPid;
		arrivalTime = newArrivalTime;
		burstTime = newBurstTime;
		previouslyServiced = false; // We indicate the process has not yet received CPU service in preemptive schemes.
	}
	~Process() {}

	// Return value: the process's pid.
	int getPID() {
		return pid;
	}

	// Return value: the process's arrival time.
	int getArrivalTime() {
		return arrivalTime;
	}

	// Return value: the process's burst time.
	int getBurstTime() {
		return burstTime;
	}

	// Reduce the burst time of the process by the specified amount of time (taken in as a parameter).
	void reduceBurstTime(int time) {
		burstTime -= time;
	}

	// Return value: the last time at which the process was preempted.
	int getLastPreemptedTime() {
		return lastPreemptedTime;
	}

	// Note the last time at which a process was preempted (taken in as a parameter).
	void setLastPreemptedTime(int preemptionTime) {
		lastPreemptedTime = preemptionTime;
	}

	// Return value: true if the process has been serviced at least once (for preemptive schemes), false if not.
	bool isPreviouslyServiced() {
		return previouslyServiced;
	}

	// Note that the process has received service at least once (for preemptive schemes).
	void setPreviouslyServiced() {
		previouslyServiced = true;
	}
};