#include <vector>
#include <iostream>
#include <string>
#include <cstring>

class SudokuSolver {
private:
    std::vector<std::vector<int>> board;
    
    bool isValid(int row, int col, int num) {
        // Check row
        for (int x = 0; x < 9; x++) {
            if (board[row][x] == num) {
                return false;
            }
        }
        
        // Check column
        for (int x = 0; x < 9; x++) {
            if (board[x][col] == num) {
                return false;
            }
        }
        
        // Check 3x3 box
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    bool solve() {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                // Find an empty cell
                if (board[row][col] == 0) {
                    // Try placing numbers 1-9
                    for (int num = 1; num <= 9; num++) {
                        if (isValid(row, col, num)) {
                            // Place the number if valid
                            board[row][col] = num;
                            
                            // Recursively solve the rest of the puzzle
                            if (solve()) {
                                return true;
                            }
                            
                            // If placing the number doesn't lead to a solution, backtrack
                            board[row][col] = 0;
                        }
                    }
                    // No valid number was found for this cell
                    return false;
                }
            }
        }
        // All cells are filled, puzzle is solved
        return true;
    }
    
public:
    SudokuSolver() {
        // Initialize empty 9x9 board
        board = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
    }
    
    void setBoard(const std::vector<std::vector<int>>& newBoard) {
        board = newBoard;
    }
    
    void setBoard(const std::string& boardStr) {
        int index = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (index < boardStr.size()) {
                    char c = boardStr[index++];
                    board[i][j] = (c == '.' || c == '0') ? 0 : (c - '0');
                }
            }
        }
    }
    
    std::vector<std::vector<int>> getBoard() const {
        return board;
    }
    
    std::string getBoardString() const {
        std::string result;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                result += (board[i][j] == 0) ? '.' : ('0' + board[i][j]);
            }
        }
        return result;
    }
    
    bool solvePuzzle() {
        return solve();
    }
};

// Compile this to WebAssembly for use in the web interface
extern "C" {
    void freeMemory(char* ptr) {
        delete[] ptr;
    }
    // Function to solve a Sudoku puzzle represented as a string
    // Input format: 81 characters, digits 1-9 for filled cells, '.' or '0' for empty cells
    // Output format: 81 characters representing the solved puzzle
    const char* solveSudoku(const char* inputBoard) {
        // Validate input length
        if (std::strlen(inputBoard) != 81) {
            return ""; // Return an empty string for invalid input
        }

        // Validate input characters
        for (size_t i = 0; i < 81; i++) {
            if (!((inputBoard[i] >= '1' && inputBoard[i] <= '9') || inputBoard[i] == '.' || inputBoard[i] == '0')) {
                return ""; // Invalid input
            }
        }

        std::string result;
        SudokuSolver solver;
        solver.setBoard(std::string(inputBoard));

        if (solver.solvePuzzle()) {
            result = solver.getBoardString();
        } else {
            result = ""; // Empty string indicates no solution
        }

        char* output = new char[result.size() + 1];
        std::strcpy(output, result.c_str());
        return output;
        }
    }

// Main function for standalone testing (not used in WebAssembly)
//Dont use webassembly code
#ifndef __EMSCRIPTEN__
int main() {
    SudokuSolver solver;
    std::string input = "5.....6.........3.4.42....6....1.....5.83.7....6.....8....71.9.1.........4.....5";
    
    solver.setBoard(input);
    std::cout << "Input Puzzle:" << std::endl;
    
    const auto& board = solver.getBoard();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    if (solver.solvePuzzle()) {
        std::cout << "Solved Puzzle:" << std::endl;
        const auto& solvedBoard = solver.getBoard();
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << solvedBoard[i][j] << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No solution exists." << std::endl;
    }
    
    return 0;
}
#endif