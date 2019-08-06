#include <iostream>	// cout
using namespace std;
// Header file
#include "sudoku2.h"

int main(){
	// Different Sudoku examples here
	// Sudoku my_sudoku("sudokus/test_sudoku.txt");
	// Sudoku my_sudoku("sudokus/easy_sudoku.txt");
	// Sudoku my_sudoku("sudokus/medium_sudoku.txt");
	Sudoku my_sudoku("sudokus/hard_sudoku.txt");
	// Sudoku my_sudoku("sudokus/bad_sudoku.txt");

	// Public functions
	// my_sudoku.set_path("Path goes here");
	cout << "File path: " << my_sudoku.get_path() << endl;
	// my_sudoku.display_sudoku();
	return 0;
}
