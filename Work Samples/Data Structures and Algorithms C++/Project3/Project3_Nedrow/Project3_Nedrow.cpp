// Project3_Nedrow.cpp : Defines the entry point for the console application.
// Daniel Nedrow
// CS 3100-01
// Dr. Raymer

#include "stdafx.h"
#include "AVLTree.h"

#include <iostream>

using namespace std;

int main()
{
	AVLTree tree;
	cout << "size: " << tree.getSize() << endl;
	cout << "height: " << tree.getHeight() << endl;

	int key = 0, value;
	while (key != -1) { // enter a key of -1 to terminate input loop
		cout << "input key, value (key of -1 will terminate input): ";
		cin >> key >> value;
		
		if (key != -1) {
			tree.insert(key, value);
			cout << endl;
			tree.print();
		}
		
	}

	cout << "size: " << tree.getSize() << endl;
	cout << "height: " << tree.getHeight() << endl;

	int valueSearch;
	bool found = tree.find(60, valueSearch);
	cout << "Did we find the value? " << found << ". If so, it was " << valueSearch << endl;

	vector<int> results = tree.findRange(30, 70);
	cout << endl << "The following key, value pairs were in range: " << endl;
	for (unsigned int i = 0; i < results.size(); i++) {
		if (i % 2 == 0) {
			cout << results[i] << ", ";
		}
		else {
			cout << results[i] << endl;
		}
	}
	cout << endl;

	system("Pause");
	return 0;
}

