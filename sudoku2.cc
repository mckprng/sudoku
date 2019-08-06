#include <iostream>	// cout
#include <vector>	// vector
#include <string>	// string
#include <fstream>	// ifstream
// Header file
#include "sudoku2.h"

// Constructors
Sudoku::Sudoku(): file_path("No path specified.") {}

Sudoku::Sudoku(std::string path): file_path(path) {
	solve(); // if sudoku is initialized with path, then solve
}

// Display and solve loaded sudoku
int Sudoku::solve(){
    read_sudoku();
	std::cout << "Original sudoku:" << std::endl << std::endl;
	display_sudoku(original_sudoku);
	logic_solve(original_sudoku); // logic_solve() runs brute_solve if guesswork has to be done
    // brute_solve(original_sudoku); // brute_solve() can be run without logic_solve()
}

// Read the input file
int Sudoku::read_sudoku(void){
	std::ifstream ifs_file(file_path); // load file to memory
	if (ifs_file.is_open()){ // if file is successfully open
		std::string sudoku_line;
		for (int row=0;row<9;row++){ // go through all 9 rows
			getline(ifs_file, sudoku_line); // get full row as string
			for (int col=0;col<9;col++) // go through all 9 numbers
				original_sudoku[row][col] = sudoku_line[col] - 48; // ASCII numbers go from 0=48
		}
	}
	return 0;
}

// Display sudoku array s
void Sudoku::display_sudoku(int s[9][9]){
	for (int row = 0; row < 9; row++){
		for (int col = 0; col < 9; col++){
			std::cout << s[row][col];
			if ((col+1)%3 == 0) std::cout << " ";
		}
		std::cout << std::endl;
		if ((row+1)%3 == 0) std::cout << std::endl;
	}
	std::cout << "-----------" <<std::endl << std::endl;
}

// Horizontal check for number num at coordinates row, col
int Sudoku::hcheck(int s[9][9], int row, int col, int num){
	for (int x=0;x<9;x++)
		if (s[row][x] == num) return 1;
	return 0;
}

// Vertical check for number num at coordinates row, col
int Sudoku::vcheck(int s[9][9], int row, int col, int num){
	for (int x=0;x<9;x++)
		if (s[x][col] == num) return 1;
	return 0;
}

// Box check for number num at coordinates row, col
int Sudoku::bcheck(int s[9][9], int row, int col, int num){
	for (int x=0;x<3;x++)
		for (int y=0;y<3;y++)
			if (s[x+(row-row%3)][y+(col-col%3)] == num) return 1;
    return 0;
}

// Check horizontal, vertical and box for number num at coordinates row, col
int Sudoku::all_check(int s[9][9], int row, int col, int num){
	if (hcheck(s, row ,col, num)||vcheck(s, row ,col, num)||bcheck(s, row ,col, num)) return 1;
	else return 0;
}

// 
int Sudoku::next_check(int s[9][9], int row, int col, std::vector<int> missing_in_ij){
	// determine other horizontals in same box
	int hone, htwo;
	if (row%3 < 1)		hone = 1,	htwo = 2;
	else if (row%3 > 1)	hone = -1,	htwo = -2;
	else 				hone = 1,	htwo = -1;

	// Determine other verticals in same box
	int vone, vtwo;
	if (col%3 < 1)		vone = 1,	vtwo = 2;
	else if (col%3 > 1)	vone = -1,	vtwo = -2;
	else				vone = 1,	vtwo = -1;

	for (auto num: missing_in_ij){
		// if vertical others are not 0
		if ((s[row+hone][col] && s[row+htwo][col]) 
		// and if num is in other verticals,
		&& (vcheck(s, row, col+vone, num) && vcheck(s, row, col+vtwo, num)))
			return num;
		// if horizontal others are not 0
		else if ((s[row][col+vone] && s[row][col+vtwo])
        // and if num is in other horizontals,
		&& (hcheck(s, row+hone, col, num) && hcheck(s, row+htwo, col, num)))
			return num;
		// If only missing number can only fit in this square
		else if ((vcheck(s, row, col+vone, num) && vcheck(s, row, col+vtwo, num))
		&& (hcheck(s, row+hone, col, num) && hcheck(s, row+htwo, col, num)))
			return num;
		// Missing a few more unique candidate checks here
	}
	return 0;
}

// Return amount of missing numbers (0's)
int Sudoku::missing_numbers(int s[9][9]){
	int count = 0;
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			if (s[col][row] == 0) count++;
	return count;
}

// Solve sudoku until guesswork is required
int Sudoku::logic_solve(int s[9][9]){
	int count = 0; // To see when the function has gone through all the numbers without any changes
	int missing_nums = missing_numbers(s);
	while (missing_nums > 1 && count < 81){
		for (int row = 0; row < 9; row++){  
			for (int col = 0; col < 9; col++){
				count++;
				if (!s[row][col]){
					std::vector<int> missing_in_ij;
					for (int num=1;num<10;num++)
						if (!all_check(s, row ,col, num))
							missing_in_ij.push_back(num);
					if (missing_in_ij.size() == 1){
						s[row][col] = missing_in_ij[0];
						missing_nums--;
						count = 0; // reset count
					}else{
						int testn = next_check(s, row, col, missing_in_ij);
						if (testn){
							s[row][col] = testn;
							missing_nums--;
							count = 0; // reset count
						}
					}
				}
			}
		}
	}
	if(count > 81){
		std::cout << "All numbers has been gone through without any changes." << std::endl << std::endl;
        display_sudoku(s);
		std::cout << "Starting to brute-force sudoku..." << std::endl << std::endl;
		if (!brute_solve(s)) // if the recursion returns negative
            std::cout << "No solution was found." << std::endl << std::endl;
	}else{
		std::cout << "Sudoku done!" << std::endl << std::endl;
		display_sudoku(s);
		return 0;
	}
}

// Go through sudoku s, until a 0 is found
int Sudoku::find_zero(int s[9][9], int &row, int &col){ // row and col references to other functions equivalent
    for (row = 0; row < 9; row++)
		for (col = 0; col < 9; col++)
			if (!s[row][col]) return 1; // When 0 is found
    return 0;
}

// Start guessing using recursion, this can be used alone without logic_solve()
// Inspiration:
// https://www.geeksforgeeks.org/sudoku-backtracking-7/
int Sudoku::brute_solve(int s[9][9]){
    int row, col; // row and col for find_zero()
    // find a 0 in sudoku s, if no 0's then the sudoku must have been solved, then print
    if (!find_zero(s, row, col)){
        std::cout << "Sudoku has successfully been brute-forced!" << std::endl << std::endl;
        display_sudoku(s);
        return 1;
    }
	for (int num=1;num<10;num++){ // Go through all numbers
		if (!all_check(s, row ,col, num)){ // sole candidate check
            s[row][col] = num; // try number
            if (brute_solve(s)) return 1; // recursion
            else s[row][col] = 0; // else revert number to 0 (backtrack)
		}
	}
    return 0;
}
