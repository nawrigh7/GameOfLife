#include <cmath>
#include <unistd.h>
#include <string.h>
#include <typeinfo>
#include "GOLApp.h"
#include "GameOfLife.h"
#include "Wraparound.h"
#include "ThreeState.h"

using namespace std;

// Error Handling
void GOLApp::PrintError() {
    cerr << "Error on line " << error_line << ": " << error_message << endl;
    cerr << "Error code: " << error_line + 1 << endl;
    exit(error_line+1);
}

bool GOLApp::CheckIfDigit(string &dim) {
    error_message = "Invalid Header - Please enter an Integer";
    error_line = 1;
    if(dim.empty()) {
        PrintError();
    }
    for(char c : dim) {
        if(!isdigit(c)) {
            PrintError();
        }
    }
    return true;
}

bool GOLApp::ValidateHeader() {
    error_message = "Invalid header";
    input_file.open(file_name);
    if(input_file.is_open()) {
        try {
            stringstream ss(grid[0]);
            string strlen, strheight;
            ss >> strheight >> strlen;
            if(CheckIfDigit(strlen) && CheckIfDigit(strheight)) {
                if(ss.good()) {
                    PrintError();
                }
                length = stoi(strlen);
                height = stoi(strheight);
            }
            
        } catch(invalid_argument& e) {
            PrintError();
        }
    }
    error_line++;
    return true;
}

bool GOLApp::ValidateContents() {
    error_message = "Invalid character found";
    error_line = 2;
    string line;
    for(size_t i=0; i<grid_no_spaces.size(); i++) {
        line = grid_no_spaces[i];
        for(char c : line) {
            if(c != 'X' && c != 'O') {
                PrintError();
            }
        }
        error_line++;
    }
    return true;
}

bool GOLApp::ValidateLength() {
    error_message = "Invalid row length";
    error_line = 2;
    for(size_t i=0; i<grid_no_spaces.size(); i++) {
        if(grid_no_spaces[i].length() != static_cast<size_t>(length)) {
            PrintError();
        }
        error_line++;
    }
    return true;
}

bool GOLApp::ValidateHeight() {
    error_message = "Invalid height";
    error_line = 1;
    int count = 0;
    for(string line : grid_no_spaces) {
        count++;
        if(count > height) {
            PrintError();
        }
        error_line++;
    }
    if(count < height) {
        PrintError();
    }

    return true;
}

bool GOLApp::Validate() {
    if(ValidateHeader()) {
        RemoveWhitespace();
        if(ValidateContents() && ValidateLength() && ValidateHeight()) {
            return true;
        }
    }
    return false;
}

void GOLApp::ReadFile() {
    error_message = "Could not open file";
    input_file.open(file_name);
    string line;
    if(input_file.is_open()) {
        while(getline(input_file, line)) {
            grid.push_back(line);
        }
    } else {
        PrintError();
    }
    error_line++;
}

void GOLApp::RemoveWhitespace() {
    for(size_t i=1; i<grid.size(); i++) {
        string removed;
        string line = grid[i];
        if(line.empty()) {
            continue;
        }
        for(char c : line) {
            if(isspace(c)) {
                continue;
            }
            grid_line_format.push_back(c);
            removed.push_back(c);
        }
        grid_no_spaces.push_back(removed);
    }
}

void GOLApp::ConvertGrid() {
    for(size_t i=0; i<grid_no_spaces.size(); i++) {
        string line = grid_no_spaces[i];
        string res;
        for(char c : line) {
            if(c == 'X') {
                res.push_back('0');
            }else {
                res.push_back('1');
            }
        }
        conv_line = conv_line + res;
        grid_conv.push_back(res);
    }
}

GOLApp::GOLApp() {
    file_name = "";
    error_line = 0;
    isValid = false;
    wraparound = false;
    third_state = false;
}

// Deprecated
GOLApp::GOLApp(std::string argv) {
    file_name = argv;
    error_line = 0;
    isValid = false;
    wraparound = false;
    third_state = false;
}

void GOLApp::Initialize() {
    if(third_state && wraparound) {
        cerr << "Error: cannot play both 3 State and Wraparound game." << endl;
        cerr << "Error code: 1" << endl;
        exit(1);
    }
    ReadFile();
    isValid = Validate();
    if(isValid) {
        ConvertGrid();
    }
}

void GOLApp::Print() {
    cout << conv_line << endl;
    for(string line : grid_conv) {
        if(line.empty()) {
            continue;
        }
        cout << line << endl;
    }
}

int GOLApp::GetLength() {
    return length;
}

int GOLApp::GetHeight() {
    return height;
}

std::string GOLApp::GetConvLine() {
    return conv_line;
}

void GOLApp::SetState() {
    third_state = true;
}

void GOLApp::SetWrap() {
    wraparound = true;
}

void GOLApp::AddString(std::string argv) {
    file_name = argv;
}

template <typename T>
void Pause(T &game) {
    std::cout << "Game paused. What would you like to do?" << std::endl;
    std::cout << "Current GameBoard:" << std::endl;
    bool resumeGame = false;
    std::string input;
    size_t x, y;
    char command;
    while(!resumeGame) {
        game.printGame(false);
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if(!(ss >> command)) {
            continue;
        }
        if(command == 'x') {
            resumeGame = true;
            continue;
        }
        if(command == 'a' || command == 'd' || command == 'e') {
            bool oneDim = false;
            int coord1 = -1;
            int coord2 = -2;
            if(ss >> coord1) {
                if(!(ss >> coord2)) {
                    oneDim = true;
                }
            }else {
                std::cerr << "Error: You must enter valid coordinates" << std::endl;
                // error message for no coordinates?
            }
            if(oneDim) {
                x = coord1 % game.getWidth();
                y = coord1 / game.getWidth();
            }else {
                x = coord1;
                y = coord2;
            }
        }
        if(x > static_cast<size_t>(game.getWidth())) {
            std::cerr << "Error: Invalid x-coordinate" << std::endl;
        }

        if(y > static_cast<size_t>(game.getHeight())) {
            std::cerr << "Error: Invalid y-coordinate" << std::endl;
        }
        switch(command){
            case 'a':
                // animate cell at (row,col)
                game.AnimateCell(y,x);
                break;
            case 'd':
                // kill cell at (row,col)
                game.KillCell(y,x);
                break;
            case 'e':
                // decay cell at (row,col)
                game.DecayCell(y,x);
                break;
        }
    }
}

template <typename T>
void Play(T &game, int generations, int print_interval, int pause_interval) {
    game.printGame(game.filePrint);
    game.next();
    for(int i=1; i<=generations; i++) {
        int time_to_pause;
        if(pause_interval > 0) {
            time_to_pause = i % pause_interval;
        }
        if(time_to_pause == 0) {
            if(i != generations) {
                Pause(game);
            }
        }
        if(print_interval == 0) {
            game.printGame(game.filePrint);
            game.next();
        }else {
            int time_to_print = i % print_interval;
            if(time_to_print == 0) {
                game.printGame(game.filePrint);
            }
            game.next();
        }
    }
}

int main(int argc, char** argv) {
    int generations = 10;
    int print_interval = 1;
    int pause_interval = 0;

    int arg;
    GOLApp parser;
    while((arg = getopt(argc, argv, "-f:g:p:x:a:d:e:o:sw")) != -1) {
        switch(arg) {
            case 'f':
                parser.AddString(optarg);
                break;
            case 'g':
                generations = stoi(optarg);
                break;
            case 'p':
                print_interval = stoi(optarg);
                break;
            case 'x':
                pause_interval = stoi(optarg);
                break;
            case 'a':
                parser.changeAlive = true;
                parser.aliveChar = *optarg;
                break;
            case 'd':
                parser.changeDead = true;
                parser.deadChar = *optarg;
                break;
            case 'e':
                parser.changeDecayed = true;
                parser.decayedChar = *optarg;
                break;
            case 'o':
                parser.fileOut = true;
                parser.output_file = optarg;
                break;
            case 's':
                parser.SetState();
                break;
            case 'w':
                parser.SetWrap();
                break;
            case '?':
                if(optopt == 'f') {
                    cerr << "Error: Please supply a file to read." << endl << "Error Code: 1" << endl;
                    exit(1);
                }
        }
    }

    parser.Initialize();
    //cout << "Generations: " << generations << endl;
    //cout << "Print Interval: " << print_interval << endl; HW3 functionality
    if(parser.isValid) { 
        //parser.Print(); // HW1 Functionality
        if(parser.wraparound) {
            Wraparound wrap(parser.GetLength(), parser.GetHeight(), parser.GetConvLine());
            if(parser.changeAlive) {
                wrap.setAliveChar(parser.aliveChar);
            }
            if(parser.changeDead) {
                wrap.setDeadChar(parser.deadChar);
            }
            if(parser.fileOut) {
                wrap.setFilePrint(parser.output_file);
            }
            Play(wrap, generations, print_interval, pause_interval);

        }else if (parser.third_state) {
            ThreeState three(parser.GetLength(), parser.GetHeight(), parser.GetConvLine());
            if(parser.changeAlive) {
                three.setAliveChar(parser.aliveChar);
            }
            if(parser.changeDead) {
                three.setDeadChar(parser.deadChar);
            }
            if(parser.changeDecayed) {
                three.setDecayedChar(parser.decayedChar);
            }
            if(parser.fileOut) {
                three.setFilePrint(parser.output_file);
            }
            Play(three, generations, print_interval, pause_interval);
        }else {
            GameOfLife game(parser.GetLength(), parser.GetHeight(), parser.GetConvLine());
            if(parser.changeAlive) {
                game.setAliveChar(parser.aliveChar);
            }
            if(parser.changeDead) {
                game.setDeadChar(parser.deadChar);
            }
            if(parser.fileOut) {
                game.setFilePrint(parser.output_file);
            }
            Play(game, generations, print_interval, pause_interval);
        }
    }
    return 0;
}