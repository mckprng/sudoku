#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>	// string
#include <vector>	// vector

class Sudoku{
	private:
		// Variables
		std::string file_path = "";
		int original_sudoku[9][9];
		
		// Misc private sudoku functions:

		// Read
		int read_sudoku(void);
		
		// Check
		int missing_numbers(int[9][9]);
		int hcheck(int[9][9], int, int, int);
		int vcheck(int[9][9], int, int, int);
		int bcheck(int[9][9], int, int, int);
		int all_check(int[9][9], int, int, int);
		int next_check(int[9][9], int, int, std::vector<int>);
        	int find_zero(int[9][9], int&, int&);

		// Solve
		int logic_solve(int[9][9]);
		int brute_solve(int[9][9]);

	public:
		// constructors
		Sudoku();
		Sudoku(std::string);
		
		// Set and get functions
		inline void set_path(std::string path){
			file_path = path;
		}
		inline std::string get_path(void){
			return file_path;
		}
		
		// Print function
		void display_sudoku(int[9][9]);

        	// Read file and solve sudoku
        	int solve(void);
};

#endif
