#include "Wraparound.h"

#include <iostream>

void Wraparound::MakeGameBoard() {
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

void Wraparound::CheckCell(size_t row, size_t col) {
    int neighbors = 0;
    bool alive = game_board[row][col];
    neighbors += CheckAll(row, col);
    ContemplateLife(neighbors, alive, row, col);
}

int Wraparound::CheckAll(size_t row, size_t col) {
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

int Wraparound::CheckLeft(size_t row, size_t col) {
    return game_board[row][(col-1 + width) % width];
}
int Wraparound::CheckRight(size_t row, size_t col) {
    return game_board[row][(col+1) % width];
}
int Wraparound::CheckDown(size_t row, size_t col) {
    return game_board[(row+1) % height][col];
}
int Wraparound::CheckUp(size_t row, size_t col) {
    return game_board[(row - 1 + height) % height][col];
}
int Wraparound::CheckDownLeft(size_t row, size_t col) {
    return game_board[(row+1) % height][(col-1 + width) % width];
}
int Wraparound::CheckDownRight(size_t row, size_t col) {
    return game_board[(row+1) % height][(col+1) % width];
}
int Wraparound::CheckUpLeft(size_t row, size_t col) {
    return game_board[(row - 1 + height) % height][(col-1 + width) % width];
}
int Wraparound::CheckUpRight(size_t row, size_t col) {
    return game_board[(row - 1 + height) % height][(col+1) % width];
}

void Wraparound::ContemplateLife(int neighbors, bool alive, size_t row, size_t col) {
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
void Wraparound::KillCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = deadChar;
}
void Wraparound::AnimateCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = aliveChar;
}

void Wraparound::DecayCell(size_t row, size_t col) {
    // Do nothing - this is only for pause behavior
    // Yes, I know this is bad practice.
    row = row;
    col = col;
}

// Ctor initializes width, height, game_string, and game_board
Wraparound::Wraparound(int width, int height, const std::string &game_string): width(width), height(height), game_string(game_string), game_board(height, std::vector<int>(width, 0)) {
    generation = 0;
    aliveChar = '1';
    deadChar = '0';
    filePrint = false;
    MakeGameBoard();
}

void Wraparound::setAliveChar(char c) {
    aliveChar = c;
}

void Wraparound::setDeadChar(char c) {
    deadChar = c;
}

void Wraparound::setFilePrint(std::string &output) {
    filePrint = true;
    output_file.open(output, std::ios::app);
}

void Wraparound::next() {
    for(size_t i=0; i<static_cast<size_t>(height); i++) {
        for(size_t j=0; j<static_cast<size_t>(width); j++) {
            CheckCell(i,j);
        }
    }
    ++generation;
    MakeGameBoard();
}

void Wraparound::nextNGen(int gen) {
    if(gen > 0) {
        for(int i=0; i<gen; i++) {
            next();
        }
    }
}

void Wraparound::printGame(bool file){
    if(file){
        output_file << "Generation: " << generation << std::endl;
        for(size_t i=0; i<game_board.size(); i++) {
            std::string line = "";
            for(size_t j=0; j<game_board[i].size(); j++) {
                if(game_board[i][j] == 0) {
                    line = line + deadChar;
                }else {
                    line = line + aliveChar;
                }
                //line = line + std::to_string(game_board[i][j]);
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
                //line = line + std::to_string(game_board[i][j]);
            }
            std::cout << line << std::endl;
        }
    }
}

int Wraparound::getWidth() {
    return width;
}

int Wraparound::getHeight() {
    return height;
}