#include "puzzle.h"
#include <iostream>

using namespace std;

Puzzle::Puzzle()
{
	int option = -1;

	cout << "CROSSWORD PUZZLE CREATOR" << endl;
	cout << "=======================================" << endl << endl;

	cout << "INSTRUCTIONS:" << endl;
	cout << "Input '?' anytime to get help" << endl; 
	cout << "Position (LCD / CTRL-Z = stop )" << endl;
	cout << "LCD stands for Line Column and Direction" << endl;
	cout << "Example: CbH stands for line C, column b and horizontal direction" << endl; 
	cout << endl; // TEMPORARIO
	cout << "---------------------------------------" << endl << endl;
	while (option != 0) {
		cout << "MENU" << endl << "1 - Create Puzzle" << endl << "2 - Resume Puzzle" << endl << "0 - Exit" << endl << endl;

		cout << "Option ? "; // opcao
		cin >> option;
		cin.clear(); // Limpa a errorFlag
		cin.ignore(1000, '\n'); // Ignora tudo o que esta para a frente da palavra inicial para evitar erros
		if (option != 0 && option != 1 && option != 2)
			cout << "Not a valid option..." << endl << endl;

		if (option == 0 || option == 1 || option == 2) {
			switch (option) {
			case 1: createPuzzle();
				option = -1;
				break;
			case 2: resumePuzzle();
				option = -1;
				break;
			case 0: cout << "Closing Program..." << endl;
				break;
			}
		}
	}
}

int Puzzle::createPuzzle() {
	string dictionaryFile_name;
	ifstream dicFile;

	// DIRETORIO DO MARANTES: C:\\Users\\gonca\\Documents\\GitHub\\PROG_PROJ2\\synonyms_WINDOWS.txt
	//DIRETORIO DO TIAGO: C:\\Users\\tiago\\Desktop\\PROG_PROJ2\\synonyms_WINDOWS.txt

	cout << "---------------------------------------" << endl << "CREATE PUZZLE" << endl << "---------------------------------------" << endl;
	do {
		cout << "Dictionary file name ? ";
		cin >> dictionaryFile_name;

		dictionaryFileName = dictionaryFile_name; // Needed to save the board

		dicFile.open(dictionaryFile_name); // open input file

		if (!dicFile.is_open()) { // If the input file is not open
			cout << "Couldn't open the file" << endl;
		}
	} while (!dicFile.is_open());
	dicFile.close();

	Dictionary dic(dictionaryFile_name); // It is impossible to pass an ifstream as argument

	int lin, col;
	do {
		cout << "Board size (lines columns) ? ";
		cin >> lin >> col;

		if (cin.fail()) {
			cout << "Wrong input... Please insert two integers separated by space" << endl;
			cin.clear(); // Clears errorFlag
			cin.ignore(1000, '\n'); // Ignora tudo o que esta para a frente da palavra inicial para evitar erros
		}
	} while (cin.fail());

	Board b(lin, col);

	cout << endl;
	b.showBoard();

	puzzleOperations(b, dic);

	return 0;
}

void Puzzle::puzzleOperations(Board b, Dictionary dic) {
	string position, word, pseudoWord;
	vector <string> possibleWords;

	while (true) {
		cout << endl << "Position ('LCD' / CTRL-Z = stop / ? = help)? ";
		cin >> position;
		if (position == "?") {  // Help input
			cout << "=== HELP ===" << endl;
			cout << "Enter coordinates and direction" << endl << "Example: CbH = Coordinates (C,b) horizontal" << endl;
			cout << endl << "Position ('LCD' / CTRL-Z = stop)? ";
			cin >> position;
		}
		if (cin.fail()) // Ends if entered CTRL+Z
			if (cin.eof()) {
				cin.clear();
				break;
			}
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Word (? = help / - = remove word)? ";
		cin >> word;
		cin.clear();
		cin.ignore(1000, '\n');
		if (word == "?") {  // Help input
			cout << "=== HELP ===" << endl;
			cout << "Enter a word to be added to the crossword board (if its not in the dictionary you can try again)" << endl;
			cout << "Enter '-' to delete the word starting in the chosen position" << endl;
			cout << "Possible words are..." << endl;
			pseudoWord = b.getWildcardWord(position);
			possibleWords = dic.wildcard(pseudoWord); // Vector containing all the possible words
			for (int i = 0; i < possibleWords.size(); i++) {
				if (!b.wordRepeated(possibleWords[i]))
					cout << possibleWords[i] << ", ";
			}
			cout << endl << "Word? ";
			cin >> word;
		}
		if (word == "-") // Remove word input
			b.removeWord(position);
		if (dic.isValid(word) && !b.wordRepeated(word)) // Does the word belong in the dictionary?
			b.insertWord(position, word); // Insert word input
		else if (b.wordRepeated(word))
			cout << "Word was already inserted in the board" << endl;
		else if (word != "?" && word != "-") // Only shows error when word is different from 'help' and 'delete'
			cout << "Invalid word" << endl;
		b.showBoard();
		cout << endl;
	}

	int option;

	do {
		cout << endl << "OPTIONS" << endl;
		cout << "1- Save and resume later" << endl << "2- Finish" << endl << "0- Discard" << endl;
		cin >> option;
		if (option != 1 && option != 2 && option != 0)   // Invalid entries 
			cout << "Not a valid option..." << endl << endl;

		if (option == 1) 
			b.saveBoard(dictionaryFileName);
		else if (option == 2) {
			if (b.checkFinalBoard()) { // Check if the board is correct
				b.finalizeBoard(); // Fills the elements that are not letters or '#' with '#'
				b.saveFinalBoard(dictionaryFileName);
			}
			else
				cerr << "Board has invalid words or other errors..." << endl;
		}
	} while (option != 1 && option != 2 && option != 0); 

}

int Puzzle::resumePuzzle() { // Resume a criacao de um puzzle
	string dictionaryFile_name, crosswordsFile_name;
	ifstream dicFile;

	cout << "---------------------------------------" << endl << "RESUME PUZZLE" << endl << "---------------------------------------" << endl;

	cout << "Board file name ? ";
	cin >> crosswordsFile_name;

	do {
		cout << "Board file name ? ";
		cin >> crosswordsFile_name;

		crossFile.open(crosswordsFile_name); // open input file

		if (!crossFile.is_open()) { // If the input file is not open
			cout << "Couldn't open the file" << endl;
		}
	} while (!crossFile.is_open());
	
	getline(crossFile, dictionaryFile_name); // Extracts the name of the dictionary file
	dictionaryFileName = dictionaryFile_name; // Puzzle class variable used to save the board

	Dictionary dic(dictionaryFile_name);
	Board b(0,0);

	b.loadBoard(crosswordsFile_name);
	b.showBoard();
	puzzleOperations(b, dic);

	crossFile.close();
	return 0;
}