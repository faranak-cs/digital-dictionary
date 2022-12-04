//libraries are included

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "Dictionary.h"

using namespace std;

//to measure execution time
using namespace chrono;



//Parsing algorithm that read dictionary and add to Trie
Trie* readFiletoTrie(Trie* root) {
	ifstream file;
	file.open("Dictionary.csv", ios::in);

	string line, word, partOfSpeech, definition;
	while (getline(file, line)) {

		//stream of entire line of csv
		stringstream s(line);

		getline(s, word, ',');
		//word without quotations
		word.erase(remove(word.begin(), word.end(), '\"'), word.end());

		getline(s, partOfSpeech, ',');
		//Part of Speech without quotations
		partOfSpeech.erase(remove(partOfSpeech.begin(), partOfSpeech.end(), '\"'), partOfSpeech.end());

		getline(s, definition);
		//definition without quotations
		definition.erase(remove(definition.begin(), definition.end(), '\"'), definition.end());

		//add to Trie Tree
		insertTrie(root, word, partOfSpeech, definition);
	}
	file.close();
	return root;
}

//Parsing algorithm that read dictionary and add to AVL
Node* readFiletoAVL(Node* root) {
	fstream file;
	file.open("Dictionary.csv", ios::in);
	string line, word, partOfSpeech, definition;
	while (getline(file, line)) {
		stringstream s(line);
		getline(s, word, ',');
		word.erase(remove(word.begin(), word.end(), '\"'), word.end());

		getline(s, partOfSpeech, ',');
		partOfSpeech.erase(remove(partOfSpeech.begin(), partOfSpeech.end(), '\"'), partOfSpeech.end());

		getline(s, definition);
		definition.erase(remove(definition.begin(), definition.end(), '\"'), definition.end());

		//add to AVL Tree
		root = insert(root, word, partOfSpeech, definition);
	}
	file.close();
	return root;
}

//compares	execution time between using Trie and AVL
void timeComp(microseconds timeTrie, microseconds timeAVL) {
	float percentAVL = ((timeTrie.count() - timeAVL.count()) / (float)timeTrie.count()) * 100.0;
	float percentTrie = ((timeAVL.count() - timeTrie.count()) / (float)timeAVL.count()) * 100.0;
	if (timeTrie.count() > timeAVL.count()) {
		cout << "The AVL Tree is " << percentAVL << "% faster." << endl;
	}
	else {
		cout << "The Trie is " << percentTrie << "% faster." << endl;
	}
}


//search definition
void definitionFinder(Node* root, Trie* root2, string mode, string input) {
	string def = "DNE";
	microseconds findDefDurationTrie, findDefDurationAVL;

	if (mode == "Trie" || mode == "Comparison") {
		auto start = high_resolution_clock::now();

		//returns definition
		def = findDefinitionTrie(root2, input);

		auto stop = high_resolution_clock::now();
		findDefDurationTrie = duration_cast<microseconds>(stop - start);
	}
	if (mode == "AVL" || mode == "Comparison") {
		auto start2 = high_resolution_clock::now();

		//returns definition
		def = findDefinitionAVL(root, input);

		auto stop2 = high_resolution_clock::now();
		findDefDurationAVL = duration_cast<microseconds>(stop2 - start2);
	}

	//if word does not exist
	if (def == "DNE") {
		cout << "Your input, '" << input << "', was not found." << endl;

		if (mode == "Trie" || mode == "Comparison") {
			cout << "It took " << findDefDurationTrie.count() << " microseconds using Trie mode." << endl;
		}
		if (mode == "AVL" || mode == "Comparison") {
			cout << "It took " << findDefDurationAVL.count() << " microseconds using AVL mode." << endl;
		}
		if (mode == "Comparison") {
			timeComp(findDefDurationTrie, findDefDurationAVL);
		}
	}
	//if word exists
	else {
		cout << def << endl;
		if (mode == "Trie" || mode == "Comparison") {
			cout << "It took " << findDefDurationTrie.count() << " microseconds to find the defintion using Trie mode." << endl;
		}
		if (mode == "AVL" || mode == "Comparison") {
			cout << "It took " << findDefDurationAVL.count() << " microseconds to find the defintion using AVL mode." << endl;
		}
		if (mode == "Comparison") {
			timeComp(findDefDurationTrie, findDefDurationAVL);
		}
	}
}

//start main() 

int main() {
	//pointer to AVL node
	Node* rootAVL = NULL;
	//pointer to Trie node
	Trie* rootTrie = NULL;

	//read file and measure time to load dictionary in Trie Tree 
	auto start = high_resolution_clock::now();
	rootTrie = readFiletoTrie(rootTrie);
	auto stop = high_resolution_clock::now();
	auto TrieDuration = duration_cast<microseconds>(stop - start);
	cout << TrieDuration.count() / 1000000.0 << " seconds to create the Trie dictionary." << endl;

	//read file and measure time to load dictionary in AVL Tree 
	auto start2 = high_resolution_clock::now();
	rootAVL = readFiletoAVL(rootAVL);
	auto stop2 = high_resolution_clock::now();
	auto AVLDuration = duration_cast<microseconds>(stop2 - start2);
	cout << AVLDuration.count() / 1000000.0 << " seconds to create the AVL dictionary." << endl;

	timeComp(TrieDuration, AVLDuration);

	string mode = "Trie";
	string input = "";
	while (input != "@Exit") {
		cout << "\nWelcome to the Digital Dictionary!";
		cout << "\n\tCurrent Mode: " << mode;
		cout << "\n\tType <@help> for information";
		cout << "\n\tType to search for a word or definition:" << endl;
		cin >> input;

		if (input == "@Help" || input == "@help") {
			cout << "\nInstructions:";
			cout << "\n\tType <a word> to check if it is a word and find the definition";
			cout << "\n\t\tNote: You must capitalize the first letter of the searched word.";
			cout << "\n\tType <1> for Trie dictionary mode";
			cout << "\n\tType <2> for AVL dictionary mode";
			cout << "\n\tType <3> for a comparison mode of Trie and AVL Structures";
			cout << "\n\tType <@Exit> to exit the program anytime" << endl;
		}
		//change to Trie mode
		else if (input == "1") {
			if (mode == "Trie") {
				cout << "It is already in Trie mode!" << endl;
			}
			else {
				mode = "Trie";
				cout << "Changing mode to Trie\n";
			}
		}
		//change to AVL mode
		else if (input == "2") {
			if (mode == "AVL") {
				cout << "It is already in AVL mode!" << endl;
			}
			else {
				mode = "AVL";
				cout << "Changing mode to AVL\n";
			}
		}
		//change to comparison mode
		else if (input == "3") {
			if (mode == "Comparison") {
				cout << "It is already in Comparison mode!" << endl;
			}
			else {
				mode = "Comparison";
				cout << "Changing mode to Comparison\n";
			}
		}
		//exit program
		else if (input == "@Exit" || input == "@exit") {
			return 0;
		}
		//finds definition
		else {
			definitionFinder(rootAVL, rootTrie, mode, input);
		}
	}
	return 0;

}//end main()
