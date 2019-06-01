// Daniel Nedrow
// CEG 4350
// Dr. Yong Pei
#pragma once

#include "ProcessBank.h"
#include "ReadyQueue.h"
#include <iomanip>

// The class to implement FCFS, RR, and MLFQ process scheduling algorithms. The same data fields and constructor
// are available no matter what algorithm is chosen, but certain data fields will be unused for the simpler
// scheduling algorithms. It is in the choice of 3 function calls that the 3 scheduling choices are distinguished.
class CPUScheduler
{
private:
	int time, waitingTime, responseTime, turnaroundTime, quantum0, quantum1, totalProcesses, remainingProcesses;
	ProcessBank processBank;
	ReadyQueue queue0, queue1, queue2;
public:
	// Create a CPU scheduler with the specified process bank to draw processes from.
	CPUScheduler(ProcessBank newProcessBank) {
		processBank = newProcessBank;
		time = waitingTime = responseTime = turnaroundTime = totalProcesses = 0;
		quantum0 = 8; // the quantum for RR or queue 0 in MLFQ
		quantum1 = 16; // the quantum for queue 1 in MLFQ
	}
	~CPUScheduler() {}

	// Add to the ready queue all processes from the bank that are entitled to have arrived by the current time.
	void addNewlyArrivedProcesses() {
		// As long as there are still processes in the bank and their arrival time is due, add them to queue
		while (time >= processBank.getNextArrivalTime() && remainingProcesses > 0) {
			queue0.enqueue(processBank.getNextProcessFromBank()); // we can add new arrivals to queue 0 regardless of scheduling scheme
			totalProcesses++; // total processes is incremented only upon first arrival, and is used for statistical averages
			remainingProcesses--; // tracks remaining processes in process bank
		}
	}

	// Display the average waiting time, average response time, and average turnaround time for all processes.
	// Note: this data is tracked for all processes; we don't bother storing it for individual processes as this
	// information is not required. Integrating the data into the Process class would easily allow individual tracking.
	void displayStats(){
		cout << "=======================================" << endl;
		cout << setw(26) << "Average waiting time: " << setw(7) << (double)waitingTime / totalProcesses << " ms" << endl;
		cout << setw(26) << "Average response time: " << setw(7) << (double)responseTime / totalProcesses << " ms" << endl;
		cout << setw(26) << "Average turnaround time: " << setw(7) << (double)turnaroundTime / totalProcesses << " ms" << endl;
		cout << "=======================================" << endl;
	}

	// Schedule processes in first-come, first serve scheme.
	void scheduleAsFCFS() {
		remainingProcesses = processBank.getSize(); // track remaining processes to draw from bank
		
		// continue getting and scheduling processes from the bank until it is empty
		while (remainingProcesses > 0) {
			// advance time in the unlikely event the queue is empty but there are future processes still to arrive from bank
			if (time < processBank.getNextArrivalTime()) 
				time = processBank.getNextArrivalTime();

			addNewlyArrivedProcesses(); // any processes that would have arrived by now are added to queue

			// while there are processes in the ready queue, schedule them
			while (queue0.length() > 0) {
				Process temp = queue0.dequeue();
				responseTime += time - temp.getArrivalTime(); // track combined response time for all processes
				cout << right << setw(3) << temp.getPID() << left << setw(16) << " starts running" << " at " << right << setw(3) << time << " ms" << endl;
				time += temp.getBurstTime(); // in FCFS, time always advances until process finished
				turnaroundTime += time - temp.getArrivalTime(); // track combined turnaround time for all processes
				cout << right << setw(3) << temp.getPID() << left << setw(16) << " has finished" << " at " << right << setw(3) << time << " ms" << endl;
			}
		}

		waitingTime = responseTime; // always true for non-preemptive scheduling
		displayStats(); // display stats for all processes
	}

	// Schedule processes in round robin scheme.
	void scheduleAsRoundRobin() {
		remainingProcesses = processBank.getSize(); // track remaining processes to draw from bank
		
		// continue getting and scheduling processes from the bank until it is empty
		while (remainingProcesses > 0) {
			// advance time in the unlikely event the queue is empty but there are future processes still to arrive from bank
			if (time < processBank.getNextArrivalTime()) 
				time = processBank.getNextArrivalTime();
			
			addNewlyArrivedProcesses(); // any processes that would have arrived by now are added to queue

			// while there are processes in the ready queue, schedule them
			while (queue0.length() > 0) {
				Process temp = queue0.dequeue();

				// if the process is newly arrived, calculate response and waiting times and display "starts running" message
				if (!temp.isPreviouslyServiced()) {
					responseTime += time - temp.getArrivalTime();
					waitingTime += time - temp.getArrivalTime();
					temp.setPreviouslyServiced(); // note that the process is no longer newly arrived (in case it goes back on queue)
					cout << right << setw(3) << temp.getPID() << left << setw(18) << " starts running" << " at " << right << setw(3) << time << " ms" << endl;
				}
				else { // if the process has seen prior CPU service, update waiting time and display "continues running" message
					waitingTime += time - temp.getLastPreemptedTime(); // wait since last preemption only to avoid double counting
					cout << right << setw(3) << temp.getPID() << left << setw(18) << " continues running" << " at " << right << setw(3) << time << " ms" << endl;
				}

				// if the process can completely finish within 8 ms, calculate turnaround time and display "has finished" message
				if (temp.getBurstTime() <= quantum0) {
					time += temp.getBurstTime(); // time advances only to point where process is finished
					addNewlyArrivedProcesses(); // in preemptive scheme, we must add new arrivals while this process was executing
					turnaroundTime += time - temp.getArrivalTime(); // track combined turnaround time for all processes
					cout << right << setw(3) << temp.getPID() << left << setw(18) << " has finished" << " at " << right << setw(3) << time << " ms" << endl;
				}
				else { // if the quantum will expire before process completion, display "is preempted" message
					time += quantum0; // time advances only by 8 ms quantum
					addNewlyArrivedProcesses(); // in preemptive scheme, we must add new arrivals while this process was executing
					temp.setLastPreemptedTime(time); // note when this process was preempted
					temp.reduceBurstTime(quantum0); // reduce this process's remaining burst time by 8 ms quantum
					cout << right << setw(3) << temp.getPID() << left << setw(18) << " is preempted" << " at " << right << setw(3) << time << " ms" << endl;
					queue0.enqueue(temp); // put this process at the rear of ready queue
				}
			}
		}

		displayStats(); // display stats for all processes
	}

	// Schedule processes in multi-level feedback queue scheme.
	void scheduleAsMLFQ() {
		remainingProcesses = processBank.getSize(); // the number of processes yet to arrive from bank
		
		// continue getting and scheduling processes from bank until it is empty
		while (remainingProcesses > 0) {
			// advance time in the unlikely event all queues are empty but there are future processes still to arrive from bank
			if (time < processBank.getNextArrivalTime()) 
				time = processBank.getNextArrivalTime(); 
			
			addNewlyArrivedProcesses(); // any processes that would have arrived by now are added to queue0
			scheduleQueue0MLFQ(); // Schedule queue 0.
			scheduleQueue1MLFQ(); // Schedule queue 1. Note: queue 1 scheduler will handle preemption by queue 0.
			scheduleQueue2MLFQ(); // Schedule queue 2. Note: queue 2 scheduler will handle preemption by higher queues.
		}

		displayStats(); // display stats for all processes
	}

	// Schedule Queue 0 of the multi-level feedback queue as a round robin queue with time quantum of 8 ms.
	void scheduleQueue0MLFQ() {
		// while there are still processes in queue 0, schedule them
		while (queue0.length() > 0) {
			Process temp = queue0.dequeue();

			// Queue 0 processes are always newly arrived, so we can update response and waiting times, display "starts running" message
			// We don't need to track "previously serviced" as in round robin because queue level is deterministic of previous service.
			responseTime += time - temp.getArrivalTime();
			waitingTime += time - temp.getArrivalTime();
			cout << right << setw(3) << temp.getPID() << left << setw(26) << " starts running" << " at " << right << setw(3) << time << " ms" << endl;
			
			// if the process can completely finish within 8 ms, calculate turnaround time and display "has finished" message
			if (temp.getBurstTime() <= quantum0) {
				time += temp.getBurstTime(); // time advances only to point where process if finished
				addNewlyArrivedProcesses(); // add processes that arrived while this one was executing
				turnaroundTime += time - temp.getArrivalTime();
				cout << right << setw(3) << temp.getPID() << left << setw(26) << " has finished" << " at " << right << setw(3) << time << " ms" << endl;
			}
			else { // if the quantum will expire before process completion, display "is preempted" message
				time += quantum0; // time advances only by 8 ms quantum
				addNewlyArrivedProcesses(); // add processes that arrived while this one was executing
				temp.setLastPreemptedTime(time); // track when this process was preempted
				temp.reduceBurstTime(quantum0); // reduce remaining burst time of this process by 8 ms quantum
				cout << right << setw(3) << temp.getPID() << left << setw(26) << " is preempted into Queue 1" << " at " << right << setw(3) << time << " ms" << endl;
				queue1.enqueue(temp); // demote this process to queue 1 for further scheduling
			}
		}
	}

	// Schedule Queue 1 of the multi-level feedback queue as a round robin queue (with 16 ms quantum) that is preempted by Queue 0.
	void scheduleQueue1MLFQ() {
		// while there are still processes in queue 1, schedule them
		while (queue1.length() > 0) {
			Process temp = queue1.dequeue();

			// Queue 1 processes are always previously serviced, so we update waiting time and display "continues running" message
			waitingTime += time - temp.getLastPreemptedTime();
			cout << right << setw(3) << temp.getPID() << left << setw(26) << " continues running" << " at " << right << setw(3) << time << " ms" << endl;
			
			// if this process can run to completion OR quantum expiration WITHOUT preemption by new arrival in Queue 0
			if (remainingProcesses == 0 || time + min(temp.getBurstTime(), quantum1) <= processBank.getNextArrivalTime()) {
				// if the process can complete within 16 ms quantum, update turnaround time and display "has finished" message
				if (temp.getBurstTime() <= quantum1) {
					time += temp.getBurstTime(); // time advances to point where process is finished
					turnaroundTime += time - temp.getArrivalTime(); // track total turnaround time
					cout << right << setw(3) << temp.getPID() << left << setw(26) << " has finished" << " at " << right << setw(3) << time << " ms" << endl;
				}
				else { // if the 16 ms quantum expires, display "is preempted into Queue 2" message
					time += quantum1; // time advances by 16 ms quantum
					temp.setLastPreemptedTime(time); // note when process was preempted
					temp.reduceBurstTime(quantum1); // reduce process's burst time by 16 ms quantum
					cout << right << setw(3) << temp.getPID() << left << setw(26) << " is preempted into Queue 2" << " at " << right << setw(3) << time << " ms" << endl;
					queue2.enqueue(temp); // demote this process to queue 2 for further scheduling
				}
			}
			else { // if process is going to be preempted by newly arrived process before it reaches completion or quantum expiration
				temp.reduceBurstTime(processBank.getNextArrivalTime() - time); // process's burst time is reduced by whatever service it got
				time = processBank.getNextArrivalTime(); // time will advance only to the new process's arrival time
				temp.setLastPreemptedTime(time); // note when process was preempted
				cout << right << setw(3) << temp.getPID() << left << setw(26) << " is preempted into Queue 1" << " at " << right << setw(3) << time << " ms" << endl;
				queue1.enqueue(temp); // when preempted by higher-priority process, this process goes to rear of CURRENT queue
			}
			
			addNewlyArrivedProcesses(); // we must now add and schedule Queue 0 processes before continuing with Queue 1
			scheduleQueue0MLFQ(); 
		}
	}

	// Schedule Queue 2 of the multi-level feedback queue as a first-come, first-serve queue that is preempted by higher queues.
	void scheduleQueue2MLFQ() {
		// while there are still processes in queue 2, schedule them
		while (queue2.length() > 0) {
			Process temp = queue2.dequeue();
			
			// Queue 2 processes are always previously serviced, so we update waiting time and display "continues running" message
			waitingTime += time - temp.getLastPreemptedTime();
			cout << right << setw(3) << temp.getPID() << left << setw(26) << " continues running" << " at " << right << setw(3) << time << " ms" << endl;
			
			// If there are no more higher-level processes or if this process can finish before one arrives, display "finished" message.
			// Queue 2 is a first come, first serve queue with no concept of time quantums.
			if (remainingProcesses == 0 || time + temp.getBurstTime() <= processBank.getNextArrivalTime()) {
				time += temp.getBurstTime(); // advance time to where this process is finished
				turnaroundTime += time - temp.getArrivalTime(); // track total turnaround time
				cout << right << setw(3) << temp.getPID() << left << setw(26) << " has finished" << " at " << right << setw(3) << time << " ms" << endl;
			}
			else { // if the process is preempted before completion by higher queue, display "is preempted" message
				temp.reduceBurstTime(processBank.getNextArrivalTime() - time); // process's burst time is reduced by whatever service it got
				time = processBank.getNextArrivalTime(); // time will advance only to the new process's arrival time
				temp.setLastPreemptedTime(time); // note when process was preempted
				cout << right << setw(3) << temp.getPID() << left << setw(26) << " is preempted into Queue 2" << " at " << right << setw(3) << time << " ms" << endl;
				queue2.enqueue(temp); // when preempted by higher-priority process, this process goes to rear of CURRENT queue
			}
			
			addNewlyArrivedProcesses(); // we must now add and schedule Queue 0 processes before continuing with Queue 2
			scheduleQueue0MLFQ();
			scheduleQueue1MLFQ(); // we must also schedule Queue 1 processes before continuing with Queue 2
		}
	}

	// Return value: the minimum of two integers (taken in as parameters).
	int min(int num1, int num2) {
		if (num1 <= num2)
			return num1;
		return num2;
	}
};