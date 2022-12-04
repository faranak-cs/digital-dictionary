#ifndef Dictionary_H
#define Dictionary_H

//libraries are included

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;


//AVL Tree Data Structure


//node AVL
class Node {
public:
	Node* left;
	Node* right;
	string word;
	string partOfSpeech;
	string definition;
	int height;
};

//max function
int max(int first, int second) {
	if (first > second)
		return first;
	else
		return second;
}
//returns the height
int getHeight(Node* temp) {
	if (temp == NULL)
		return 0;
	return temp->height;
}

//This function will create new AVL node and initialize it
Node* createAvlNode(string input_word, string input_partOfSpeech, string input_definition) {
	Node* node = new Node();
	node->left = NULL;
	node->right = NULL;

	node->word = input_word;
	node->partOfSpeech = input_partOfSpeech;
	node->definition = input_definition;
	node->height = 1;
	return node;
}

//left rotation
Node* rotateLeft(Node* node) {

	//Set and do left rotation

	Node* grandchild = node->right->left;
	Node* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;

	//Update height
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	newParent->height = max(getHeight(newParent->left), getHeight(newParent->right)) + 1;

	//Return the new parent
	return newParent;
}

//right rotation
Node* rotateRight(Node* node) {
	//Set and do right rotation
	Node* grandchild = node->left->right;
	Node* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;

	//Update height
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	newParent->height = max(getHeight(newParent->left), getHeight(newParent->right)) + 1;

	//Return the new parent
	return newParent;
}

//calculate balance factor
int balanceFactor(Node* temp) {
	if (temp == NULL)
		return 0;
	return getHeight(temp->left) - getHeight(temp->right);
}

//insert function for AVL
Node* insert(Node* node, string input_word, string input_partOfSpeech, string input_definition) {

	//Perform recursive insertion
	if (node == NULL) {
		Node* temp_node = createAvlNode(input_word, input_partOfSpeech, input_definition);
		return temp_node;
	}
	//goes to left
	if (input_word < node->word)
		node->left = insert(node->left, input_word, input_partOfSpeech, input_definition);

	//goes to right
	else if (input_word > node->word)
		node->right = insert(node->right, input_word, input_partOfSpeech, input_definition);
	else {
		return node;
	}
	//Again calculate the height of the node
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

	//Determine the balance factor to see if rotations are necessary
	int balance_factor = balanceFactor(node);


	/*********Rotation Cases**********/

	//Left Left
	if (balance_factor > 1 && input_word < node->left->word)
		return rotateRight(node);
	//Right Right
	if (balance_factor < -1 && input_word > node->right->word)
		return rotateLeft(node);
	//Left Right
	if (balance_factor > 1 && input_word > node->left->word) {
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}
	//Right Left
	if (balance_factor < -1 && input_word < node->right->word) {
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	//Insertion successful, return node
	return node;
}

//search function for AVL
string findDefinitionAVL(Node* current, string input) {
	if (current == NULL) {
		return "DNE";
	}
	else {
		//base case
		if (current->word == input) {
			return (current->partOfSpeech + " " + current->definition);
		}
		//recursive cases

		else if (current->word > input) {
			return findDefinitionAVL(current->left, input);
		}
		else if (current->word < input) {
			return findDefinitionAVL(current->right, input);
		}
	}
	return "DNE";
}

/*----------------------------------------------------------------------------------*/

//Trie Tree Data Structure



//node Trie
struct Trie {
	//since there are 26 alphabets
	Trie* child[26];
	string meaning;
	bool isWord;
};


//new node
Trie* getNewTrieNode()
{
	Trie* node = new Trie;

	for (int i = 0; i < 26; i++) {
		node->child[i] = NULL;
	}
	node->isWord = false;
	return node;
}

//This insert function will be used by the parser we created to insert string
void insertTrie(Trie*& root, string str, const string& type, const string& def)
{
	if (root == NULL)
		root = getNewTrieNode();

	Trie* temp = root;

	for (int i = 0; str[i] != '\0'; i++) {

		//provide proper indexing from 0-25
		int index = tolower(str[i]) - 'a';

		//handle the error
		if (index > 25 || index < 0)
			continue;

		if (temp->child[index] == NULL) {
			temp->child[index] = getNewTrieNode();
		}
		temp = temp->child[index];
	}


	// store the meaning
	temp->isWord = true;
	temp->meaning = type + " " + def;

}

//search function for Trie
string findDefinitionTrie(Trie* root, const string& word)
{
	// If dictionary is empty
	if (root == nullptr) {
		cout << "root is empty";
		return "DNE";
	}

	Trie* temp = root;


	// Search a word in the Trie
	for (int i = 0; word[i] != '\0'; i++) {

		//provide proper indexing from 0-25
		int index = tolower(word[i]) - 'a';

		//handle the error
		if (index > 25 || index < 0)
			continue;

		if (temp->child[index] == NULL)
			return "DNE";
		temp = temp->child[index];
	}

	//return its meaning
	if (temp->isWord)
		return temp->meaning;
	return "DNE";
}


#endif /* Dictionary_H */