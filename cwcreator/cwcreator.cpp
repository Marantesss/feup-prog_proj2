#include <iostream>
#include <vector>
#include <string>
#include "board.h"

using namespace std;
int main() {

	Board b1(10, 10);
	b1.showBoard();
	/*string input;
	while (!cin.eof()) {
		cout << "infix: ";
		cin >> input;
		cin.ignore(1000, '\n');
		cin.clear();
		cout << "postfix: " << input << endl;
	}*/
	return 0;
}
