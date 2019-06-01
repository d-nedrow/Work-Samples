#pragma once
#include <string>
#include <iostream>
#include <fstream>

#define TREENODEPTR TreeNode*
#define TREENULLPTR NULL

using namespace std;

// This is a class for the nodes of an organizational tree.  They hold two strings and pointers to the
// parent, leftmost child, and right sibling.
class TreeNode {
public:
	string title;
	string name;
	TreeNode *parent, *leftmostChild, *rightSibling;

	// Create a TreeNode with the given title and name.  All pointers are set to NULL.
	TreeNode(string title, string name) {
		this->title = title;
		this->name = name;
		parent = leftmostChild = rightSibling = TREENULLPTR;
	}
};

// The class which represents an organizational tree.  It holds a size and a pointer to its root, along with
// the methods to manipulate the tree.
// SPACE REQUIREMENTS for a tree of n employees: each employee node has 3 pointers.  The tree itself has a pointer
// to the root and an int for the size.  We ignore the data (title and name).  So for n employees, you have
// 3pn + p(for the root pointer) + 4 bytes(for the size variable).  Assuming 4 byte pointers,
// 3pn + p + 4 = (12n + 8) bytes for n employees. 
class OrgTree
{
private:
	int size;
	TREENODEPTR root;
	
public:
	// Create a default OrgTree with size zero and root pointer NULL.
	OrgTree() {
		size = 0;
		root = TREENULLPTR;
	}
	~OrgTree() {
		deleteNodes(root);
	}

	// A method (called by the destructor) to recursively delete the nodes of the OrgTree with postorder traversal.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We must visit every node to delete it.
	void deleteNodes(TREENODEPTR node) {
		if (node == TREENULLPTR)
			return;

		deleteNodes(node->leftmostChild);
		deleteNodes(node->rightSibling);
		delete node;
	}

	// Creates a root node for the tree. If the tree already has a root, the entire tree becomes a subtree 
	// of the new root.
	// Parameters: the title and name of the new root employee.
	// ASYMPTOTIC RUN TIME: Theta(n) = 1.  We create a single node, and at most assign it a child node.
	void addRoot(string title, string name) {
		TREENODEPTR newRoot = new TreeNode(title, name);
		size++;

		if (root) // if there is already a root, then place the old root as the child of the new root
			newRoot->leftmostChild = root;
		root = newRoot;
		
	}

	// Returns the number of employees in the tree.
	// Return value: the number of nodes (employees) in the OrgTree.
	// ASYMPTOTIC RUN TIME: Theta(n) = 1.
	unsigned int getSize() {
		return size;
	}

	// Returns a pointer to the root node in the tree.
	// Return value: the root pointer of the OrgTree.
	// ASYMPTOTIC RUN TIME: Theta(n) = 1.
	TREENODEPTR getRoot() {
		return root;
	}

	// Returns a pointer to the leftmost child of the node passed to the function. If the node has no children, 
	// returns TREENULLPTR.
	// Parameters: the node whose leftmost child we are looking for.
	// Return Value: the leftmost child of the given node.
	// ASYMPTOTIC RUN TIME: Theta(n) = 1.
	TREENODEPTR leftmostChild(TREENODEPTR node) {
		return node->leftmostChild;
	}

	// Returns a pointer to the right sibling of the node passed to the function. If the node has no right sibling, 
	// returns TREENULLPTR.
	// Parameters: the node whose right sibling we are looking for.
	// Return value: the right sibling of the given node.
	// ASYMPTOTIC RUN TIME: Theta(n) = 1.
	TREENODEPTR rightSibling(TREENODEPTR node) {
		return node->rightSibling;
	}

	// Hire an employee. The employee should be added to the tree as the last child of the node pointed 
	// to by TREENODEPTR.
	// Parameters: a pointer to the boss of the new employee, the new employee's title, and their name.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  In a fat tree, we would have to traverse (n - 1) siblings to add node.
	void hire(TREENODEPTR parent, string newTitle, string newName) {
		TREENODEPTR newHire = new TreeNode(newTitle, newName);
		newHire->parent = parent;
		TREENODEPTR temp;
		if (!parent->leftmostChild) // if the parent node is childless, add the new node as its child
			parent->leftmostChild = newHire;
		else { // otherwise we need to iterate over all the parent's children and hire as the last child
			temp = parent->leftmostChild;
			while (temp->rightSibling)
				temp = temp->rightSibling;
			temp->rightSibling = newHire;
		}
		size++;
		
	}

	// Print out the subtree starting at the node pointed to by subTreeRoot.
	// Parameters: the root of the subtree to be printed.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  Calls a helper function that visits every node in the subtree.
	void printSubTree(TREENODEPTR subTreeRoot) {
		string indent = "";
		printHelp(subTreeRoot, indent);
	}

	// Recursive helper function for printing out an OrgTree
	// Parameters: the root noot of the subtree to be printed, the current level of indentation to be displayed
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We must visit every node in the subtree to print it.
	void printHelp(TREENODEPTR node, string indent) {
		cout << indent << node->title << ": " << node->name << endl;
		if (node->leftmostChild) // only increase indent if we are about to go a level deeper in the tree
			indent += "        ";

		for (TREENODEPTR temp = node->leftmostChild; temp != NULL; temp = temp->rightSibling) // algorithm from textbook
			printHelp(temp, indent);
	}

	// Returns a TREENODEPTR to the node listing the given title (exact string match with the title string in a 
	// TreeNode object). If the title is not found, the function should return TREENULLPTR.
	// Parameters: the title of the node we are looking for.
	// Return value: the node with the specified value, or NULL if not found.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  May have to traverse every node in tree to find match.
	TREENODEPTR find(string title) {
		return findHelp(title, root);
	}

	// Recursive helper function for finding a node with a given title.
	// Parameters: the title of the node to find, the root node of the subtree we are currently looking in.
	// Return value: the node with the specified value, or NULL if not found.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  May have to traverse every node in the subtree to find match.
	TREENODEPTR findHelp(string title, TREENODEPTR node) {
		if (node->title == title) // return the node if it is a match
			return node;

		TREENODEPTR result = TREENULLPTR;
		// I adapted the print algorithm from the textbook to traverse every node of the subtree.
		for (TREENODEPTR temp = node->leftmostChild; temp != NULL; temp = temp->rightSibling) {
			result = findHelp(title, temp);
			if (result != NULL) // We don't want to return NULL results unless we have exhausted our search.
				return result; // We DO want to return and break the loop if we found a match.
		}

		return result; // We may have to return a NULL result if match not found.
	}

	// This is a function I added to return the left sibling of a node.  It is for convenience when firing a node.
	// Parameters: the node whose left sibling we seek.
	// Return value: the left sibling of the given node.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  In a fat tree, we may have to iterate over nearly n siblings.
	TREENODEPTR leftSibling(TREENODEPTR node) {
		// if we are looking at a node that can't have a left sibling, return NULL
		if (node == root || node == node->parent->leftmostChild)
			return TREENULLPTR;
		
		TREENODEPTR temp = node->parent->leftmostChild; 
		// iterate over siblings until we find one whose right sibling is the input parameter
		while (temp->rightSibling != node)
			temp = temp->rightSibling;
		return temp;
	}

	// This function I added returns the last child of a node.  It is for convenience when firing a node.
	// Parameters: the node whose last child we seek.
	// Return value: the last child of the given node.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  In a fat tree, we may have to iterate over nearly n siblings.
	TREENODEPTR lastChild(TREENODEPTR node) {
		if (!node->leftmostChild) // if the node has no children, return NULL
			return TREENULLPTR;

		TREENODEPTR temp = node->leftmostChild;
		// iterate over all the children until we find the last one
		while (temp->rightSibling != NULL)
			temp = temp->rightSibling;
		return temp;
	}

	// Fire the employee who's title matches the argument to this function. If the title is found the employee's node 
	// in the tree is deleted and the function returns true. All employees of the fired employee now work directly for 
	// the fired employee's boss (e.g. all children of the deleted node become children of the deleted node's parent). 
	// If no match is found the function returns false. For simplicity, you can assume that titles in the company are 
	// unique. Note that you cannot fire the root node. If the formerTitle matches the root node, the function should 
	// return false.
	// Parameters: the title of the employee to fire.
	// Return value: true if we found the employee to fire, false if we did not.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We call multiple function in Theta(n) = n, but we do not have loops in this function.
	bool fire(string formerTitle) {
		if (root->title == formerTitle || !find(formerTitle)) // we can't fire the root or a non-existent node
			return false;

		TREENODEPTR fired = find(formerTitle);
		TREENODEPTR theParent = fired->parent;
		
		// if the fired node was the leftmost child of its parent, we will need to assign someone else to that spot
		if (fired == theParent->leftmostChild) {
			// if the fired node had children, we will bring them up to be the LEFTMOST children of the fired's parent 
			if (fired->leftmostChild) {
				theParent->leftmostChild = fired->leftmostChild;
				lastChild(fired)->rightSibling = fired->rightSibling; // May be NULL.  Connect fired's last child to fired's siblings.
			}
			else { // if the fired node had no children, simply make fired's right sibling the parent's left child. 
				theParent->leftmostChild = fired->rightSibling; // May be NULL.
			}
		}		
		else { // if the fired node was not the leftmost child, we will need to connect its left and right siblings
			// if the fired node had children, we will bring them up IN PLACE (between the left and right siblings of fired)
			if (fired->leftmostChild) {
				leftSibling(fired)->rightSibling = fired->leftmostChild; // Connect fired's left sibling to fired's left child.
				lastChild(fired)->rightSibling = fired->rightSibling; // May be NULL.  Connect fired's last child to fired's right sibling.
			}
			else { // if the fired node had no children, simply connect fired's left sibling to fired's right sibling.
				leftSibling(fired)->rightSibling = fired->rightSibling; // May be NULL.
			}
		}

		delete fired;
		size--;
		return true;
	}

	// Reads an organization tree from a file. The file will contain one tree node per line, except for 
	// lines containing only ')' characters, which mark the ends of subtrees. If the file is found and 
	// read successfully, this function should return true. If the file is not found or the file is 
	// improperly formatted, the function should return false.
	// Parameters: the file name we want to read.
	// Return value: true if the file was opened, false if it was not.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We will hire the n nodes that are represented in the input file.
	bool read(string filename) {
		string title, name, junk;
		ifstream inFile(filename);
		if (!inFile.is_open()) // if the file can't open, return false
			return false;

		// Parse the input file to get the comma separated title and name of the root node.
		getline(inFile, title, ',');
		getline(inFile, junk, ' '); // it is convenient to discard a space which would otherwise be part of the name
		getline(inFile, name, '\n');
		addRoot(title, name); // create the root node of the tree
		while (inFile.peek() != ')') // we will send the root to the helper function to recurse over all of its subtrees
			readHelp(inFile, getRoot());
		inFile.close();
		return true;
	}

	// Recursive helper function to read the input file.  Gets an input file and a node whose subtree we will read.
	// Parameters: the input file and the node whose subtree we will read from that file.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We will hire the n nodes that are represented in the input file.
	void readHelp(ifstream &inFile, TREENODEPTR node) {
		string newTitle, newName, junk;
		
		// Parse the input file to get the comma separated title and name of the current node.
		getline(inFile, newTitle, ',');
		getline(inFile, junk, ' '); // it is convenient to discard a space which would otherwise be part of the name
		getline(inFile, newName, '\n');
		hire(node, newTitle, newName); // hire a new node as the last child of the input parameter node
		while (inFile.peek() != ')') { // as long as the subtree is not ended, hire another node as the child of the node we just hired
			readHelp(inFile, find(newTitle));
		}
		getline(inFile, junk); // Read the ‘)’ char in order to discard it
	}

	// Write out the OrgTree to a file, using the same file format described in the read() function above.
	// Parameters: the name of the file to write to.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We will write as many nodes as there are in the tree.
	void write(string filename) {
		ofstream outFile(filename);
		writeHelp(outFile, root);
		outFile.close();
	}

	// A recursive helper function to write the subtree of a given node to a file.
	// Parameters: the file to write to, the node whose subtree we are to write.
	// ASYMPTOTIC RUN TIME: Theta(n) = n.  We will write as many nodes as there are in the tree.
	void writeHelp(ofstream &outFile, TREENODEPTR node) {
		// We use a standard traversal algorithm to write all of the nodes of the tree.
		if (node == TREENULLPTR)
			return;

		outFile << node->title << ", " << node->name << endl; // the nodes are written in a preorder traversal
		writeHelp(outFile, node->leftmostChild);
		outFile << ')' << endl; // the subtrees are terminated in an "inorder" traversal (after we see there are no further levels of children)
		writeHelp(outFile, node->rightSibling);
	}
};

