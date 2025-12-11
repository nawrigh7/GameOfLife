#include "ThreeState.h"
#include <iostream>

void ThreeState::MakeGameBoard() {
    size_t index = 0;
    for(size_t i=0; i<game_board.size(); i++) {
        for(size_t j=0; j<game_board[i].size(); j++) {
            if(game_string[index] == aliveChar) {
                game_board[i][j] = 1;
            }
            if(game_string[index] == decayedChar) {
                game_board[i][j] = 2;
            }
            if(game_string[index] == deadChar){
                game_board[i][j] = 0;
            }
            index++;
        }
    }
}

ThreeState::NeighborCount& ThreeState::NeighborCount::operator+=(const NeighborCount& rhs) {
    for(int i=0; i<3; i++) {
        neighbors[i] += rhs.neighbors[i];
    }
    return *this;
}

const int& ThreeState::NeighborCount::operator[](int index) {
    return neighbors[index];
}

void ThreeState::CheckCell(size_t row, size_t col) {
    NeighborCount neighbors;
    // indices are alive, dead, decayed
    // Used to indicate status of current cell
    bool alive[3] = {false, false, false};
    switch(game_board[row][col]) {
        case 0:
            alive[1] = true;
            break;
        case 1:
            alive[0] = true;
            break;
        case 2:
            alive[2] = true;
            break;
    }
    if(height == 1) {
        if(col == 0) {
            neighbors += CheckRight(row, col);
        }else if(col == static_cast<size_t>(width)){
            neighbors += CheckLeft(row, col);
        }else {
            neighbors += CheckLeft(row, col);
            neighbors += CheckRight(row, col);
        }
    // Column (single vertical line) edge case    
    }else if (width == 1) {
        if(row == 0) {
            neighbors += CheckDown(row, col);
        }else if(row == static_cast<size_t>(height)) {
            neighbors += CheckUp(row, col);
        }else {
            neighbors += CheckUp(row, col);
            neighbors += CheckDown(row, col);
        }
    }else {
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

bool ThreeState::IsAlive(int cell) {
    if(cell == 1) {
        return true;
    }
    return false;
}

bool ThreeState::IsDead(int cell) {
    if(cell == 0) {
        return true;
    }
    return false;
}

bool ThreeState::IsDecayed(int cell) {
    if(cell == 2) {
        return true;
    }
    return false;
}

ThreeState::NeighborCount ThreeState::CheckAll(size_t row, size_t col) {
    NeighborCount neighbors;
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

ThreeState::NeighborCount ThreeState::CheckLeft(size_t row, size_t col) {
    if(IsAlive(game_board[row][col-1])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row][col-1])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row][col-1])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckRight(size_t row, size_t col) {
    if(IsAlive(game_board[row][col+1])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row][col+1])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row][col+1])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckDown(size_t row, size_t col) {
    if(IsAlive(game_board[row+1][col])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row+1][col])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row+1][col])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckUp(size_t row, size_t col) {
    if(IsAlive(game_board[row-1][col])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row-1][col])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row-1][col])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckDownLeft(size_t row, size_t col) {
    if(IsAlive(game_board[row+1][col-1])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row+1][col-1])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row+1][col-1])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckDownRight(size_t row, size_t col) {
    if(IsAlive(game_board[row+1][col+1])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row+1][col+1])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row+1][col+1])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckUpLeft(size_t row, size_t col) {
    if(IsAlive(game_board[row-1][col-1])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row-1][col-1])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row-1][col-1])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}
ThreeState::NeighborCount ThreeState::CheckUpRight(size_t row, size_t col) {
    if(IsAlive(game_board[row-1][col+1])) {
        return NeighborCount(1,0,0);
    }
    if(IsDead(game_board[row-1][col+1])) {
        return NeighborCount(0,1,0);
    }
    if(IsDecayed(game_board[row-1][col+1])) {
        return NeighborCount(0,0,1);
    }
    NeighborCount none;
    return none;
}

void ThreeState::ContemplateLife(ThreeState::NeighborCount neighbors, bool alive[], size_t row, size_t col) {
    if(alive[0]) {
        if(neighbors[0] < 2 || neighbors[0] > 3) {
            DecayCell(row, col);
        }
    }else if(alive[1]) {
        if(neighbors[0] == 3) {
            DecayCell(row, col);
        }
    }else if(alive[2]) {
        if(neighbors[0] == 3) {
            AnimateCell(row, col);
        }
        if(neighbors[2] == 2 && neighbors[0] == 0) {
            DecayCell(row, col);
        }else {
            KillCell(row, col);
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
void ThreeState::KillCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = deadChar;
}
void ThreeState::AnimateCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = aliveChar;
}
void ThreeState::DecayCell(size_t row, size_t col) {
    size_t location = (row * static_cast<size_t>(width)) + col;
    game_string[location] = decayedChar;
}

// Ctor initializes width, height, game_string, and game_board
ThreeState::ThreeState(int width, int height, const std::string &game_string): width(width), height(height), game_string(game_string), game_board(height, std::vector<int>(width, 0)) {
    generation = 0;
    aliveChar = '1';
    deadChar = '0';
    decayedChar = 'D';
    filePrint = false;
    MakeGameBoard();
}

void ThreeState::setAliveChar(char c) {
    aliveChar = c;
    MakeGameBoard();
}

void ThreeState::setDeadChar(char c) {
    deadChar = c;
    MakeGameBoard();
}

void ThreeState::setDecayedChar(char c) {
    decayedChar = c;
    MakeGameBoard();
}

void ThreeState::setFilePrint(std::string &output) {
    filePrint = true;
    output_file.open(output, std::ios::app);
}

void ThreeState::next() {
    for(size_t i=0; i<static_cast<size_t>(height); i++) {
        for(size_t j=0; j<static_cast<size_t>(width); j++) {
            CheckCell(i,j);
        }
    }
    ++generation;
    MakeGameBoard();
}

void ThreeState::nextNGen(int gen) {
    if(gen > 0) {
        for(int i=0; i<gen; i++) {
            next();
        }
    }
}

void ThreeState::printGame(bool file){
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
                //line = line + std::to_string(game_board[i][j]);
            }
            output_file << line << std::endl;
        }
    }else {
        std::cout << "Generation: " << generation << std::endl;
        for(size_t i=0; i<game_board.size(); i++) {
            std::string line = "";
            for(size_t j=0; j<game_board[i].size(); j++) {
                if(game_board[i][j] == 2) {
                    line = line + decayedChar;
                }else if(game_board[i][j] == 1){ 
                    line = line + aliveChar;
                }else if(game_board[i][j] == 0){
                    line = line + deadChar;
                }
            }
            std::cout << line << std::endl;
        }
    }
}

int ThreeState::getWidth() {
    return width;
}

int ThreeState::getHeight() {
    return height;
}