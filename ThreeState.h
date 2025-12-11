#ifndef THREESTATE_H
#define THREESTATE_H

#include <string>
#include <vector>
#include <fstream>

class ThreeState {
    public:
        ThreeState() = delete;
        ThreeState(int width, int height, const std::string &game_string);
        void setAliveChar(char c);
        void setDeadChar(char c);
        void setDecayedChar(char c);
        void setFilePrint(std::string &output);
        bool filePrint;
        /*
        A cell’s next state depends on the number of live neighbors it has (up, down, left, right, and diagonals).
        Cells on the edge of the board have fewer neighbors and do not wrap around.

        Any live cell with fewer than two live neighbors dies (underpopulation).  
        Any live cell with two or three live neighbors lives on to the next generation (survival).  
        Any live cell with more than three live neighbors dies (overpopulation).
        Decayed cells are considered dead when taking into account a cell animating or permanently dying.
        A decaying cell with exactly 2 decaying neighbors stays in the Decaying state.
        A decaying cell with exactly 3 living neighbors comes back to life
        A decaying cell with less than 2 or more than 3 living OR decaying neighbors dies.
        A dead cell with Exactly 3 Decaying neighbors becomes a decaying cell
        Any other combination, a dead cell stays dead
        */
        void next();
        // Move forward by N generations
        void nextNGen(int gen);
        // Print the generation, followed by the current state of the game
        void printGame(bool file);
        int getWidth();
        int getHeight();

        // Kill the current cell (in the game_string)
        void KillCell(size_t row, size_t col);
        // Bring the current cell to life (in the game_string)
        void AnimateCell(size_t row, size_t col);
        // Decay the current cell (in the game_string)
        void DecayCell(size_t row, size_t col);

        // Used to count neighbors.
        // Indices equate to alive_count, dead_count, decayed_count
        struct NeighborCount {
            int neighbors[3];
            NeighborCount() : neighbors(0, 0, 0){}
            NeighborCount(int alive, int dead, int decayed) : neighbors(alive, dead, decayed){}
            NeighborCount& operator+=(const NeighborCount& rhs);
            const int& operator[](int index);
        };

    private:

        int width, height, generation;
        char aliveChar, deadChar, decayedChar;
        std::ofstream output_file;
        std::string game_string;
        // game_board stores the game state upon receipt
        // as well as representing it in a 2d format
        std::vector<std::vector<int>> game_board;
        // populate the game board from the game_string
        // This is done upon object creation, as well as
        // after each call to next();
        void MakeGameBoard();
        // Check cell and evaluate its neighbors
        void CheckCell(size_t row, size_t col);
        // Checks status of cell on board
        bool IsAlive(int);
        bool IsDead(int);
        bool IsDecayed(int);
        // Check all directions from specified cell
        NeighborCount CheckAll(size_t row, size_t col);
        /**
         * The following methods check the direction in their names
         * from the specified cell. For example, CheckDownRight()
         * evaluates whether or not the cell that is one row down
         * and one column to the right of the current cell is alive or dead.
         * Each of these returns the int from the game_board in its relative cell.
         * (0 = dead, 1 = alive)
         */
        NeighborCount CheckLeft(size_t row, size_t col);
        NeighborCount CheckRight(size_t row, size_t col);
        NeighborCount CheckUp(size_t row, size_t col);
        NeighborCount CheckDown(size_t row, size_t col);
        NeighborCount CheckUpLeft(size_t row, size_t col);
        NeighborCount CheckUpRight(size_t row, size_t col);
        NeighborCount CheckDownLeft(size_t row, size_t col);
        NeighborCount CheckDownRight(size_t row, size_t col);
        // Use the determined information about a cell and its neighbors
        // To determine if, in the next generation, it will be alive or dead.
        void ContemplateLife(NeighborCount neighbors, bool alive[], size_t row, size_t col);
};

#endif /* THREESTATE_H */