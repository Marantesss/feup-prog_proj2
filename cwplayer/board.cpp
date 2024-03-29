#include "board.h"
#include "colors.h"
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>

Board::Board(int lines, int columns)
{
	numColumns = columns;
	numLines = lines;
	
	board.resize(numLines); // Resizes the vector 'board' to the user-entered number of lines
	for (int i = 0; i < numLines; i++)
		board[i].resize(numColumns);  //Resizes the vectors in the vector 'board' to the user - entered number of columns


	for (int i = 0; i < numLines; i++) // Fills the vector board with "."
		for (int j = 0; j < numColumns; j++)
			board.at(i).at(j) = '.';
}	

int Board::getColumns() const
{
	return numColumns;
}

int Board::getLines() const
{
	return numLines;
}

map<string, string> Board::getPosition_words() // devolve o vetor position_words
{
	return position_words;
}

void Board::showBoard() 
{
	setcolor(12); // First line of letters is lightred
	for (int i = 0; i < numColumns + 1; i++)  // Prints the upper line with the column identifiers
		cout << lowerLetters.at(i) << "  ";   // stored in the vector lowerletters

	for (int i = 0; i < numLines; i++)
	{
		cout << endl;
		setcolor(12); // First column of letters is lightred
		cout << upperLetters.at(i) << "  ";  // Prints the identifier for each line
		for (int j = 0; j < numColumns; j++) {
			if (board.at(i).at(j) == '#') { // Checks if the char being printed is '#'
				setcolor(7, 0); // Change the color of the letters 7=LIGHTGRAY and background 0=BLACK
				cout << board.at(i).at(j);
				setcolor(0, 12);
				cout << "  "; // Prints each position of the board stored in board
			}
			else {
				setcolor(0, 12); // Change the color of the letters 4=RED and background 12=LIGTHRED
				cout << board.at(i).at(j) << "  "; // Prints each position of the board stored in board
			}
		}
	}
	cout << endl;
	setcolor(7, 0); // Change back to normal (15=LIGHTGRAY letters, 0=BLACK background)
}

void Board::insertWord(string position, string word)
{
	int line, column;
	line = getIndex(toupper(position[0]));
	column = getIndex(toupper(position[1]));
	char orientation = toupper(position[2]);
	transform(word.begin(), word.end(), word.begin(), ::toupper); // Changes the word to uppercase

	if (wordFitsSpace(line, column, orientation, word)) {
		if (!(wordMatchesSpace(line, column, orientation, word)))
			cerr << "The word does not match the space" << endl; // displays error message if the word does not match the space

		if (orientation == 'V' && wordMatchesSpace(line, column, orientation, word)) { // insert VERTICAL words, if the word matches the space
			//if (line != 0)
				//board.at(line - 1).at(column) = '#'; // If the word is not being inserted in the beginning of a column places a '#' in the position before
			for (int i = 0; i < word.length(); i++) {
				board.at(line).at(column) = word[i]; // inserts the word by changing the vector board
				line++;
			};
			//if (line < getLines()) 
				//board.at(line).at(column) = '#'; // If the word does not end in the last position of a column places a '#' in the next position
			position_wordsPLAYER.insert(pair<string, string>(position, word)); // Adds a pair of (position - word) to a vector
		}

		if (orientation == 'H' && wordMatchesSpace(line, column, orientation, word)) { // insert HORIZONTAL words, if the word matches the space
			//if (column != 0)
				//board.at(line).at(column - 1) = '#'; // If the word is not being inserted in the beginning of a  places a '#' in the position before
			for (int i = 0; i < word.length(); i++) {
				board.at(line).at(column) = word[i]; // inserts the word by changing the vector board
				column++;
			};
			//if (column < getColumns())
				//board.at(line).at(column) = '#'; // If the word does not end in the last position of a line places a '#' in the next position
			position_wordsPLAYER.insert(pair<string, string>(position, word)); // Adds a pair of (position - word) to a vector
		}
	}
	else
		cerr << "The word does not fit the space" << endl; // displays error message if the word does not fit the space

	
}

int Board::getIndex(char letter) // Gets the index of a given letter of line or column 
{
	return (int)letter - 65;
}

bool Board::wordFitsSpace(int const &line, int const &column, char const &orientation, string const &word) 
// Checks if the word fits the column/line starting from the user entered position until the end of the column/line
{
	if (orientation == 'V')
	{
		if (getLines() - line >= word.length()) 
			return true;
		else return false;
	}
	if (orientation == 'H')
	{
		if (getColumns() - column >= word.length())
			return true;
		else return false;
	}	
}

bool Board::wordMatchesSpace(int const &line, int const &column, char const &orientation, string const &word)
// Cheks if the word matches with the already in-board letters
{
	vector<pair<int, int>> temporaryNonRemovableLetters;
	int movingBoardVariable;
	if (orientation == 'V')  
	{
		movingBoardVariable = line;
		if (line != 0)
			if (board.at(movingBoardVariable - 1).at(column) != '.' && board.at(movingBoardVariable - 1).at(column) != '#') 
				return false; // If the char before is not '.' or '#' returns false
		if (movingBoardVariable + word.length() < getLines())
			if (board.at(movingBoardVariable + word.length()).at(column) != '.' && board.at(movingBoardVariable + word.length()).at(column) != '#')
				return false; // If the char after is not '.' or '#' returns false

		for (int i = 0; i < word.length(); i++)
		{
			if (board.at(movingBoardVariable).at(column) != '.')  
			{
				if (board.at(movingBoardVariable).at(column) != toupper(word[i])) // If the word letter is different from the board letter returns false
					return false;
				pair<int, int> dontRemove = make_pair(movingBoardVariable, column); // if the Crossover letters match add the coordinate to a vector of pairs
				temporaryNonRemovableLetters.push_back(dontRemove);
			}
			movingBoardVariable++;
		}
		nonRemovableLetters.insert(nonRemovableLetters.end(), temporaryNonRemovableLetters.begin(), temporaryNonRemovableLetters.end()); // If the path leaves the loop FOR adds the (temporary vector of) crossover letters to a vector
		return true;
	}
	
	if (orientation == 'H')
	{
		movingBoardVariable = column;
		if (column != 0)
			if (board.at(line).at(movingBoardVariable - 1) != '.' && board.at(line).at(movingBoardVariable - 1) != '#')
				return false;
		if (movingBoardVariable + word.length() < getLines())
			if (board.at(line).at(movingBoardVariable + word.length()) != '.' && board.at(line).at(movingBoardVariable + word.length()) != '#')
				return false;

		for (int i = 0; i < word.length(); i++)
		{
			if (board.at(line).at(movingBoardVariable) != '.')
			{
				if (board.at(line).at(movingBoardVariable) != toupper (word[i]))
					return false;
				pair<int, int> dontRemove = make_pair(line, movingBoardVariable);
				temporaryNonRemovableLetters.push_back(dontRemove);
			}
			movingBoardVariable++;
		}
		nonRemovableLetters.insert(nonRemovableLetters.end(), temporaryNonRemovableLetters.begin(), temporaryNonRemovableLetters.end());
		return true;
	}
}


void Board::removeWord(string position)
{
	int line, column;
	line = getIndex(toupper(position[0]));
	column = getIndex(toupper(position[1]));
	char orientation = toupper(position[2]);

	if (orientation == 'V') // VERTICAL words
	{
		//if (line != 0) // If the word stars in the middle of the line changes the '#' to a '.'
		//	board.at(line - 1).at(column) = '.';
		
		while (board.at(line).at(column) != '#' && line < getLines()) 
		{
			if (!isInNonRemovable(line, column)) // If the letter is NOT a crossover letter changes the letter to a '.'
				board.at(line).at(column) = '.';
			line++;
			if (line == getLines()) // Stop the loop condition of trying to access beyond the length of the vector(error) 
				break;
		}
		//if (line < getLines()) // If the word ends in the middle of the line changes the '#' to a '.'
		//	board.at(line).at(column) = '.';
	}

	if (orientation == 'H') // HORIZONTAL words
	{
		//if (column != 0) // If the word stars in the middle of the column changes the '#' to a '.'
		//	board.at(line).at(column-1) = '.';

		while (board.at(line).at(column) != '#' && column < getColumns())
		{
			if (!isInNonRemovable(line, column)) // If the letter is NOT a crossover letter changes the letter to a '.'
				board.at(line).at(column) = '.';
			column++;
			if (column == getColumns()) // Stop the loop condition of trying to access beyond the length of the vector(error) 
				break;
		}
		//if (column < getColumns()) // If the word stars in the middle of the column changes the '#' to a '.'
		//	board.at(line).at(column) = '.';
	}
	map<string, string>::const_iterator index = position_wordsPLAYER.find(position); 
	position_wordsPLAYER.erase(index);
}

bool Board::isInNonRemovable(int line, int column)
{
	for (int i = 0; i < nonRemovableLetters.size(); i++) // Checks if the pair is in the nonRemovableLetters 
		if (nonRemovableLetters.at(i).first == line && nonRemovableLetters.at(i).second == column)
		{
			nonRemovableLetters.erase(nonRemovableLetters.begin() + i); // If so removes that pair because the letter is no longer a crossover letter
			return true;
		}
	return false;
}

void Board::loadBoard(string fileName)
{
	ifstream boardFile;
	string line;

	boardFile.open(fileName);  // File to load 
	if (!boardFile.is_open())
		cout << "Couldn't open the file";

	// Dictionary file name is loaded in the outside of the class Board (in the class Puzzle)
	getline(boardFile, line);
	boardFile.seekg(line.length() + 4); // Moves the pointer to the line before of the beggining of the board
	
	getline(boardFile, line);

	while (!boardFile.eof()) // Loads the letters to the vector board
	{
		board.push_back(loadBoardLine(line)); // Loads a line of the board
		getline(boardFile, line);
		if (line == "\n" || line == "") // Stops when the board ends
			break;
	}
	
	while (!boardFile.eof()) // Loads the positions and the words to a vector of pairs
	{
		string position, word;
		getline(boardFile, line);
		
		position = line.substr(0, 3); // Position are the first 3 letters
		word = line.substr(4); // Words are the remaining letters

		position_words.insert(make_pair(position, word)); // Makes a pair of (position-word)
	}
}

vector<char> Board::loadBoardLine(string &line)
{
	vector<char> boardLine;
	for (int i = 0; i < line.length(); i++) // Puts the letters or '.' in a vector that represents a line of the board
		if (line[i] != ' ' && line[i] != '\n')
			boardLine.push_back(line[i]);
	
	numColumns = boardLine.size(); // Refreshes the number of lines and columns
	numLines++;

	return boardLine; // Returns a line of the board
}

void Board::emptyGrid()
{
	for (int i = 0; i < numLines; i++)
	{
		for (int j = 0; j < numColumns; j++)
			if (board.at(i).at(j) != '#')
				board.at(i).at(j) = '.';
	}
}

map<string, string> Board::getBoardWords() {
	return position_words;
}

bool isValidPosition(Board b, string position)
{
	for (auto p : b.getBoardWords())
		if (position == p.first)
			return true;
	return false;
}

bool Board::isBoardFull() {
	for (int i = 0; i < numLines; i++)
		for (int j = 0; j < numColumns; j++)
			if (board.at(i).at(j) == '.')
				return false;
	return true;
}

bool Board::checkSolution()
{
	map<string, string>::const_iterator  vectorPlayer;
	bool result = true;

	for (auto p : position_words)
	{
		vectorPlayer = position_wordsPLAYER.find(p.first);
		if (vectorPlayer == position_wordsPLAYER.end()) {
			result = false;
			setcolor(4);
			cout << p.first << "  -------";
			setcolor(2);
			cout << " (" << p.second << ")" << endl;
		}
		else if (p.second == vectorPlayer->second)
		{
			setcolor(2);
			cout << p.first << "  " << vectorPlayer->second << endl;
		}
		else
		{
			result = false;
			setcolor(4);
			cout << p.first << "  " << vectorPlayer->second;
			setcolor(2);
			cout << " (" << p.second << ")" << endl;
		}
	}
	setcolor(7, 0);
	return result;
}