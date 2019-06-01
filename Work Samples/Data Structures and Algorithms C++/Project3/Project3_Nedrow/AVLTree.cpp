#include "stdafx.h"
#include "AVLTree.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class AVLTree::Node {
public:
	Node *leftChild, *rightChild;
	int key, value, leftDepth, rightDepth, balance;

	// Create a node with given key, value pair.
	Node(int newKey, int newValue) {
		key = newKey;
		value = newValue;
		leftDepth = rightDepth = balance = 0;
		leftChild = rightChild = nullptr;
	}
};

AVLTree::AVLTree()
{
	size = 0;
	root = nullptr;
}


AVLTree::~AVLTree()
{
}

bool AVLTree::insert(int key, int value) {
	int dummyValue; // the find function requires an int to store the node's value in; we will ignore
	if (find(key, dummyValue)) // if the given key is in the tree, we won't insert it again
		return false;

	helpInsert(root, key, value);
	size++;
	return true;
}

void AVLTree::helpInsert(Node*& where, int key, int value) {
	if (!where) { // when we recursively arrive at the nullptr where our new node goes, create the node
		where = new Node(key, value);
		return;
	}

	if (key < where->key) { // if the key is smaller than the current node's key, insert as left child
		helpInsert(where->leftChild, key, value);
		where->leftDepth = max(where->leftChild->leftDepth, where->leftChild->rightDepth) + 1; // update left depth
	}
	else { // otherwise insert as right child
		helpInsert(where->rightChild, key, value);
		where->rightDepth = max(where->rightChild->leftDepth, where->rightChild->rightDepth) + 1; // update right depth
	}

	where->balance = where->leftDepth - where->rightDepth; // update balance

	if (where->balance > 1) { // check to see if we need any rotates (among the four options) due to imbalance
		if (where->leftChild->balance > 0) {
			singleRightRotate(where);
		}
		else {
			doubleRightRotate(where);
		}
	}
	else if (where->balance < -1) {
		if (where->rightChild->balance < 0) {
			singleLeftRotate(where);
		}
		else {
			doubleLeftRotate(where);
		}
	}
}

void AVLTree::singleRightRotate(Node*& problem) {
	Node* hook = problem->leftChild;
	Node* temp = hook->rightChild; // store part of the tree that would need reattached
	hook->rightChild = problem; // swing problem node down to be right child of hook
	problem = hook; // pull the hook up 
	problem->rightChild->leftChild = temp; // reattach cut-out part
	
	if (problem->rightChild->leftChild) { // update the left depth of old problem node; it may have gained a subtree
		problem->rightChild->leftDepth =
			max(problem->rightChild->leftChild->leftDepth, problem->rightChild->leftChild->rightDepth) + 1;
	}
	else { // if no subtree, left depth is zero
		problem->rightChild->leftDepth = 0;
	}
	
	problem->rightDepth = max(problem->rightChild->leftDepth, problem->rightChild->rightDepth) + 1; // old hook's right depth

	problem->balance = problem->leftDepth - problem->rightDepth; // new balance for old hook and old problem
	problem->rightChild->balance = problem->rightChild->leftDepth - problem->rightChild->rightDepth;

}

void AVLTree::singleLeftRotate(Node*& problem) {
	Node* hook = problem->rightChild;
	Node* temp = hook->leftChild; // store part of the tree that would need reattached
	hook->leftChild = problem; // swing problem node down to be left child of hook
	problem = hook; // pull the hook up
	problem->leftChild->rightChild = temp; // reattach cut-out part
	
	if (problem->leftChild->rightChild) { // update the right depth of old problem node; it may have gained a subtree
		problem->leftChild->rightDepth =
			max(problem->leftChild->rightChild->leftDepth, problem->leftChild->rightChild->rightDepth) + 1;
	}
	else { // if no subtree, right depth is zero
		problem->leftChild->rightDepth = 0;
	}
	
	problem->leftDepth = max(problem->leftChild->leftDepth, problem->leftChild->rightDepth) + 1; // old hook's left depth

	problem->balance = problem->leftDepth - problem->rightDepth; // new balance for old hook and old problem
	problem->leftChild->balance = problem->leftChild->leftDepth - problem->leftChild->rightDepth;
}

void AVLTree::doubleRightRotate(Node*& problem) {
	singleLeftRotate(problem->leftChild);
	singleRightRotate(problem);
}

void AVLTree::doubleLeftRotate(Node*& problem) {
	singleRightRotate(problem->rightChild);
	singleLeftRotate(problem);
}

int AVLTree::max(int num1, int num2) {
	if (num1 >= num2)
		return num1;
	return num2;
}

int AVLTree::getSize() {
	return size;
}

int AVLTree::getHeight() {
	if (!root)
		return 0;
	else
		return max(root->leftDepth, root->rightDepth) + 1;
}

void AVLTree::print() {
	printHelp(root, 0);
}

void AVLTree::printHelp(Node* node, int level) {
	if (!node) {
		return;
	}

	string indent = "";
	for (int i = 0; i < level; i++) // create proper indent based on level of tree
		indent += "        ";
	level++;

	printHelp(node->rightChild, level);

	cout << indent << node->key << ", " << node->value << endl << endl;

	printHelp(node->leftChild, level);
}

bool AVLTree::find(int key, int& value) {
	return findHelp(root, key, value);
}

bool AVLTree::findHelp(Node* node, int key, int& value) {
	if (!node) { // we've exhausted the search
		value = -1;
		return false;
	}

	if (key < node->key) { // if the key is smaller than this node's key, only search left
		return findHelp(node->leftChild, key, value);
	}
	else if (key > node->key) { // if the key is bigger than this node's key, only search right
		return findHelp(node->rightChild, key, value);
	}
	else { // otherwise we found the key
		value = node->value;
		return true;
	}

}

vector<int> AVLTree::findRange(int lowkey, int highkey) {
	vector<int> results;
	findRangeHelp(root, lowkey, highkey, results);
	return results;
}

void AVLTree::findRangeHelp(Node* node, int lowkey, int highkey, vector<int>& results) {
	if (!node) // we've exhausted the search along this path
		return;

	if (highkey < node->key) { // if the node's key is too big for the range, only search left
		findRangeHelp(node->leftChild, lowkey, highkey, results);
	}
	else if (lowkey > node->key) { // if the node's key is too small for the range, only search right
		findRangeHelp(node->rightChild, lowkey, highkey, results);
	}
	else { // If the node's key is in range, add the key, value pair and continue searching in both directions.
		results.push_back(node->key);
		results.push_back(node->value);
		findRangeHelp(node->leftChild, lowkey, highkey, results);
		findRangeHelp(node->rightChild, lowkey, highkey, results);
	}

}