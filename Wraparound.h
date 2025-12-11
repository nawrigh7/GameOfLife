#ifndef WRAPAROUND_H
#define WRAPAROUND_H

#include <string>
#include <vector>
#include <fstream>

class Wraparound{
    private:

        int width, height, generation;
        char aliveChar, deadChar;
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
        // Check all directions from specified cell
        int CheckAll(size_t row, size_t col);
        /**
         * The following methods check the direction in their names
         * from the specified cell. For example, CheckDownRight()
         * evaluates whether or not the cell that is one row down
         * and one column to the right of the current cell is alive or dead.
         * Each of these returns the int from the game_board in its relative cell.
         * (0 = dead, 1 = alive)
         */
        virtual int CheckLeft(size_t row, size_t col);
        virtual int CheckRight(size_t row, size_t col);
        virtual int CheckUp(size_t row, size_t col);
        virtual int CheckDown(size_t row, size_t col);
        virtual int CheckUpLeft(size_t row, size_t col);
        virtual int CheckUpRight(size_t row, size_t col);
        virtual int CheckDownLeft(size_t row, size_t col);
        virtual int CheckDownRight(size_t row, size_t col);
        // Use the determined information about a cell and its neighbors
        // To determine if, in the next generation, it will be alive or dead.
        void ContemplateLife(int neighbors, bool alive, size_t row, size_t col);

    public:

        Wraparound(int width, int height, const std::string &game_string);
        void setAliveChar(char c);
        void setDeadChar(char c);

        // Kill the current cell (in the game_string)
        void KillCell(size_t row, size_t col);
        // Bring the current cell to life (in the game_string)
        void AnimateCell(size_t row, size_t col);
        // Deprecated
        void DecayCell(size_t row, size_t col);

        void setFilePrint(std::string &output);
        bool filePrint;
        void next();
        // Move forward by N generations
        void nextNGen(int gen);
        // Print the generation, followed by the current state of the game
        void printGame(bool file);
        int getWidth();
        int getHeight();
};

#endif /* WRAPAROUND_H */