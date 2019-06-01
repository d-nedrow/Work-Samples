// Daniel Nedrow
// CS 3100-01
// Dr. Raymer
// Function headers with time complexity in this .h file.  Comments of important steps within functions in .cpp file.
#pragma once

#include <vector>

using namespace std;

// The class for our AVL tree, with a pointer to the tree root, and an integer to track the size of the tree.  Functions 
// include the ability to insert a node with a given key, value pair; the 4 types of AVL tree rotations to balance the tree; 
// functions which return the size and height of the tree; printing the tree; finding a key; and a range query.
// Space Overhead: 2pn + p + 4 bytes, p = size of pointer, n = number of nodes.  2pn from 2 pointers for each node.
// P is from the root pointer, and 4 bytes is from the int size variable.
class AVLTree
{
	// This is the node class for AVLTree nodes with a key, value pair of integers.  The depth of the left and right
	// subtree is stored in the node, and the difference between these is stored as the balance.  Pointers are given
	// for the left and right child of each node.
	class Node;
private:
	int size;
	Node *root;
public:
	// Create an empty tree.
	AVLTree();

	~AVLTree();

	// Insert a new key/value pair into the tree. Duplicate keys are not allowed. This function should return
	// true if the key/value pair is successfully inserted into the map, and false if the pair could not be 
	// inserted (for example, due to a duplicate key already found in the map).
	// Return value: true if the key/value pair is inserted, false if not.
	// Parameters: the key and value to be inserted.
	// Time Complexity: Theta(n) = log n. The tree is always in a balanced state.
	bool insert(int, int);

	// A recursive helper function to insert a key, value pair.
	// Parameters: The node we will look to create a child for with this key, value; the key and value to be inserted.
	// Time Complexity: Theta(n) = log n. The tree is always in a balanced state.
	void helpInsert(Node*&, int, int);

	// Do an AVL tree single right rotate to balance the tree.
	// Parameters: The problem node which is the deepest out of balance node.
	// Time Complexity: Theta(n) = 1.  
	void singleRightRotate(Node*&);

	// Do an AVL tree single left rotate to balance the tree.
	// Parameters: The problem node which is the deepest out of balance node.
	// Time Complexity: Theta(n) = 1. 
	void singleLeftRotate(Node*&);

	// Do an AVL tree double right rotate to balance the tree.
	// Parameters: The problem node which is the deepest out of balance node.
	// Time Complexity: Theta(n) = 1. 
	void doubleRightRotate(Node*&);

	// Do an AVL tree double left rotate to balance the tree.
	// Parameters: The problem node which is the deepest out of balance node.
	// Time Complexity: Theta(n) = 1. 
	void doubleLeftRotate(Node*&);

	// Return value: the greater of two ints.
	// Parameters: Two ints to compare.
	// Time Complexity: Theta(n) = 1. 
	int max(int, int);

	// Return value: the total number of nodes (key/value pairs) in the AVL tree.
	// Time Complexity: Theta(n) = 1. 
	int getSize();

	// Return value: the height of the AVL tree.
	// Time Complexity: Theta(n) = 1. 
	int getHeight();

	// Print the tree to cout. The print routine should print the AVL tree “sideways” using 
	// indentation to show the structure of the tree.
	// Time Complexity: Theta(n) = n. Every node must be visited to print the entire tree.
	void print();

	// Recursive helper function to print tree.  Uses right-child first, inorder traversal to print tree "sideways."
	// Parameters: The node to recursively print, the "level" of the tree used to create proper indentation.
	// Time Complexity: Theta(n) = n. Every node must be visited to print the entire tree.
	void printHelp(Node*, int);

	// If the given key is found in the AVL tree, this function should return true and place the 
	// corresponding value in value. Otherwise this function should return false (and the value 
	// in value can be anything).
	// Return value: true if the key is found, false otherwise.
	// Parameters: the key to search for, an integer reference to store the associated value (if key found).
	// Time Complexity: Theta(n) = log n. The tree is always in a balanced state.
	bool find(int, int&);

	// Recursive helper funtion to find a key in the tree.
	// Return value: true if the key is found, false if we have exhausted the search with no match.
	// Parameters: The node we are recursively searching from, the key to find, an integer reference to store 
	// associated value.
	// Time Complexity: Theta(n) = log n. The tree is always in a balanced state.
	bool findHelp(Node*, int, int&);

	// This function returns a C++ vector of integers containing all of the key/value pairs in the
	// tree with keys between lowkey and highkey (inclusive). For each key/value pair found, there 
	// will be two values in the vector: first the key, and then the value. If no matching key/value
	// pairs are found, the function should return an empty vector.
	// Return value: the vector containing all the key, value pairs within range.
	// Parameters: the lower and upper bounds (inclusive) of the range query.
	// Time Complexity: Theta(n) = n. If the range encompasses the whole tree, every node is visited.
	vector<int> findRange(int, int);

	// Recursive helper function for the range query.  Fills a vector with key, value pairs where the key
	// is in range.
	// Parameters: The node we are recursively searching from, the lower and upper bounds (inclusive) of the 
	// range query, the vector to be filled with key, values pairs in range.
	// Time Complexity: Theta(n) = n. If the range encompasses the whole tree, every node is visited.
	void findRangeHelp(Node*, int, int, vector<int>&);
};

