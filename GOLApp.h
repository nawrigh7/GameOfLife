#ifndef GOLApp_H
#define GOLApp_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

class GOLApp {
    public:
        char aliveChar, deadChar, decayedChar;
        bool changeAlive = false, changeDead = false, changeDecayed = false, fileOut = false;
        std::string output_file;

        GOLApp();
        GOLApp(std::string argv);   // Deprecated
        bool isValid, wraparound, third_state;

        int GetLength();
        int GetHeight();
        std::string GetConvLine();

        void SetState();
        void SetWrap();

        void Initialize();
        void Print();
        void AddString(std::string argv);

    private:
        constexpr static bool DEBUG = false;
        int error_line,
            length,
            height;
        std::ifstream input_file;
        std::string file_name;
        std::string grid_line_format;
        std::string conv_line;
        std::string error_message;
        std::vector<std::string> grid;
        std::vector<std::string> grid_no_spaces;
        std::vector<std::string> grid_conv;

        // Error Handling
        void PrintError();

        //Processing
        void ReadFile();
        void ConvertGrid();
        void RemoveWhitespace();
        bool CheckIfDigit(std::string &dim);

        // Validation
        bool Validate();
        bool ValidateHeader();
        bool ValidateContents();
        bool ValidateLength();
        bool ValidateHeight();
        
};

#endif /* GOLApp_H */