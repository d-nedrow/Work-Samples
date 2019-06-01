// Daniel Nedrow
// CEG 4350
// Dr. Yong Pei
#pragma once

#include "Process.h"

// Node class for a Queue. Each node holds a process.
class QueueNode {
public:
	Process process;
	QueueNode *next;

	// Default node constructor.
	QueueNode() {
		next = nullptr;
	}

	// Create a node with a specified process.
	QueueNode(Process newProcess) {
		process = newProcess;
		next = nullptr;
	}
};

// This is a basic queue class. The code for the functions is adapted from the textbook for CS 3100: Data Structures & Algorithm
// Analysis in C++ by Clifford A Shaffer. These queues are intended to be used by any of the scheduling algorithm choices.
class ReadyQueue
{
private:
	QueueNode *front, *rear;
	int size;
public:
	// Create a new queue with size zero. The front node having no data is a trick from the textbook mentioned above.
	ReadyQueue() {
		front = rear = new QueueNode();
		size = 0;
	}
	~ReadyQueue() {}

	// Add a node with the specified process onto the rear of the queue.
	// Paramaters: the process to put at the rear of the queue.
	void enqueue(Process newProcess) {
		rear->next = new QueueNode(newProcess);
		rear = rear->next;
		size++;
	}

	// Remove the first node from the queue and return its process.
	// Return value: the first process in the queue.
	Process dequeue() {
		Process theProcess = front->next->process;
		QueueNode* temp = front->next;
		front->next = temp->next;
		if (rear == temp) rear = front;
		delete temp;
		size--;
		return theProcess;
	}

	// Return the length of the queue.
	int length() {
		return size;
	}
};