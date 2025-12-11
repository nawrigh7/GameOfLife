#include "GameOfLife.h"
#include <iostream>

void GameOfLife::MakeGameBoard() {
    size_t index = 0;
    for(size_t i=0; i<game_board.size(); i++) {
        for(size_t j=0; j<game_board[i].size(); j++) {
            if(game_string[index] == aliveChar) {
                game_board[i][j] = 1;
            }else if(game_string[index] == deadChar){
                game_board[i][j] = 0;
            }
            index++;
        }
    }
}

void GameOfLife::CheckCell(size_t row, size_t col) {
    int neighbors = 0;
    bool alive = game_board[row][col];
    // Flat (single horizontal line) edge case
    if(height == 1 && width > 1) {
        if(col == 0) {
            neighbors += CheckRight(row, col);
        }else if(col == static_cast<size_t>(width)){
            neighbors += CheckLeft(row, col);
        }else {
            neighbors += CheckLeft(row, col);
            neighbors += CheckRight(row, col);
        }
    // Column (single vertical line) edge case    
    }else if (width == 1 && height > 1) {
        if(row == 0) {
            neighbors += CheckDown(row, col);
        }else if(row == static_cast<size_t>(height)) {
            neighbors += CheckUp(row, col);
        }else {
            neighbors += CheckUp(row, col);
            neighbors += CheckDown(row, col);
        }
    }else if(width == 1 && height == 1) {
        // do nothing
    }else if(width > 1 && height > 1){
        // both row & col are non 0
        if(row && col) {
            // row & col are not at the bottom row, or furthest right col
            if(row != static_cast<size_t>(height-1) && col != static_cast<size_t>(width-1)) {
                neighbors += CheckAll(row, col);
            }
            // bottom row, but not furthest col
            if(row == static_cast<size_t>(height-1) && col != static_cast<size_t>(width-1)) {
                neighbors += CheckLeft(row, col);
                neighbors += CheckUpLeft(row, col);
                neighbors += CheckUp(row, col);
                neighbors += CheckUpRight(row, col);
                neighbors += CheckRight(row, col);
            }
            // not bottom row, but furthest right col
            if(row != static_cast<size_t>(height-1) && col == static_cast<size_t>(width-1)) {
                neighbors += CheckDown(row, col);
                neighbors += CheckDownLeft(row, col);
                neighbors += CheckLeft(row, col);
                neighbors += CheckUpLeft(row, col);
                neighbors += CheckUp(row, col);
            }
            // bottom right cell
            if(row == static_cast<size_t>(height-1) && col == static_cast<size_t>(width-1)) {
                neighbors += CheckLeft(row, col);
                neighbors += CheckUpLeft(row, col);
                neighbors += CheckUp(row, col);
            }
        // Left Edge
        }else if(row && !col) {
            if(row != static_cast<size_t>(height-1)) {
                neighbors += CheckDown(row, col);
                neighbors += CheckDownRight(row, col);
            }
            neighbors += CheckRight(row, col);
            neighbors += CheckUp(row, col);
            neighbors += CheckUpRight(row, col);
        // top row
        }else if(!row && col) {
            if(col != static_cast<size_t>(width-1)) {
                neighbors += CheckRight(row, col);
                neighbors += CheckDownRight(row, col);
            }
            neighbors += CheckLeft(row, col);
            neighbors += CheckDown(row, col);
            neighbors += CheckDownLeft(row, col);
        // Origin (0,0)
        }else if(!row && !col) {
            neighbors += CheckRight(row, col);
            neighbors += CheckDownRight(row, col);
            neighbors += CheckDown(row, col);
        }
    }
    ContemplateLife(neighbors, alive, row, col);
}

int GameOfLife::CheckAll(size_t row, size_t col) {
    int neighbors = 0;
    neighbors += CheckLeft(row, col);
    neighbors += CheckRight(row, col);
    neighbors += CheckUp(row, col);
    neighbors += CheckDown(row, col);
    neighbors += CheckUpLeft(row, col);
    neighbors += CheckUpRight(row, col);
    neighbors += CheckDownLeft(row, col);
    neighbors += CheckDownRight(row, col);
    return neighbors;
}

int GameOfLife::CheckLeft(size_t row, size_t col) {
    return game_board[row][col-1];
}
int GameOfLife::CheckRight(size_t row, size_t col) {
    return game_board[row][col+1];
}
int GameOfLife::CheckDown(size_t row, size_t col) {
    return game_board[row+1][col];
}
int GameOfLife::CheckUp(size_t row, size_t col) {
    return game_board[row-1][col];
}
int GameOfLife::CheckDownLeft(size_t row, size_t col) {
    return game_board[row+1][col-1];
}
int GameOfLife::CheckDownRight(size_t row, size_t col) {
    return game_board[row+1][col+1];
}
int GameOfLife::CheckUpLeft(size_t row, size_t col) {
    return game_board[row-1][col-1];
}
int GameOfLife::CheckUpRight(size_t row, size_t col) {
    return game_board[row-1][col+1];
}

void GameOfLife::ContemplateLife(int neighbors, bool alive, size_t row, size_t col) {
    if(alive) {
        if(neighbors < 2 || neighbors > 3) {
            KillCell(row, col);
        }
    }else {
        if(neighbors == 3) {
            AnimateCell(row, col);
        }
    }
}

/**
 * 2D to 1D Indexing
 * (row*width) + col
 * 1D to 2D Indexing
 * Row = index / width
 * Col = index % width
 */
void GameOfLife::KillCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = deadChar;
}
void GameOfLife::AnimateCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = aliveChar;
}

void GameOfLife::DecayCell(size_t row, size_t col) {
    // Do nothing - this is only for pause behavior
    // Yes, I know this is bad practice.
    row = row;
    col = col;
}

// Ctor initializes width, height, game_string, default alive / dead values, and game_board
GameOfLife::GameOfLife(int width, int height, const std::string &game_string): width(width), height(height), game_string(game_string), game_board(height, std::vector<int>(width, 0)) {
    aliveChar = '1';
    deadChar = '0';
    generation = 0;
    filePrint = false;
    MakeGameBoard();
}

void GameOfLife::setAliveChar(char c) {
    aliveChar = c;
}

void GameOfLife::setDeadChar(char c) {
    deadChar = c;
}

void GameOfLife::setFilePrint(std::string &output) {
    filePrint = true;
    output_file.open(output);
}

void GameOfLife::next() {
    for(size_t i=0; i<static_cast<size_t>(height); i++) {
        for(size_t j=0; j<static_cast<size_t>(width); j++) {
            CheckCell(i,j);
        }
    }
    ++generation;
    MakeGameBoard();
}

void GameOfLife::nextNGen(int gen) {
    if(gen > 0) {
        for(int i=0; i<gen; i++) {
            next();
        }
    }
}

void GameOfLife::printGame(bool file){
    MakeGameBoard();
    if(file) {
        output_file << "Generation: " << generation << std::endl;
        for(size_t i=0; i<game_board.size(); i++) {
            std::string line = "";
            for(size_t j=0; j<game_board[i].size(); j++) {
                if(game_board[i][j] == 0) {
                    line = line + deadChar;
                }else {
                    line = line + aliveChar;
                }
            }
            output_file << line << std::endl;
        }
    }else {
        std::cout << "Generation: " << generation << std::endl;
        for(size_t i=0; i<game_board.size(); i++) {
            std::string line = "";
            for(size_t j=0; j<game_board[i].size(); j++) {
                if(game_board[i][j] == 0) {
                    line = line + deadChar;
                }else {
                    line = line + aliveChar;
                }
            }
            std::cout << line << std::endl;
        }
    }
}

int GameOfLife::getWidth() {
    return width;
}

int GameOfLife::getHeight() {
    return height;
}
