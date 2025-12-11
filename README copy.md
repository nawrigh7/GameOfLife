# ct301_hw5
CT301 FA25 - HW5 - File Output and Making It Pretty

Nicholas Wright

Files
    -GOLApp - Driver program. Responsible for file validation and initialization of a game string
    -GameOfLife - Game logic. Takes the game string and creates a board, prints lifecycle of the game.

Implementation Notes
    -Wraparound formula - Provided in class: index-1 + width % width (Same for height)
    -CLA parsing - while ((c = getopt(argc, argv, “<flags>”)) != -1) - provided in slides
        - In the <flags> section, there are options:
        + -<flags> means the flag must start with a '-'
        + : means there is a required argument after the flag. If this fails to have the
            required following data, getopt will return ?, and optopt will contain the arg
            that failed to have the necessary option

New Requirements:
    New getopt flags:
        1) -x [Pause interval]
            When the specifies a pause interval, the game should pause on that interval.
            When the game pauses, it should:
                Print: "Game paused. What would you like to do?"
                Prompt the user for input
                Print the current game board
            If the game is on the last interval on the pause interval, the game should not pause.
            Pause commands:
                a [1d coord] (set cell to alive at specified coordinate)
                a [x coord][y coord]
                d [1d coord] (set cell to dead at specified coordinate)
                d [x coord][y coord]
                e [1d coord] (set cell to decayed at specified coordinate)
                e [x coord][y coord]
                x (exit pause and continue simulation)
        2) -a [Live cell character]
        3) -d [Dead cell character]
        4) -e [decay cell character]
        5) -o [output File Name]
            In this case, no output should go to the terminal. All output, other than "paused" output
            should go to the file, instead of terminal.
Old Requirements:
    - These flags must be present:
    -f [filename] : if after parsing no filename is found, print error message and return 1.
    -g [number of generations] : default is 10 if not defined
    -p [print intervals] : default is 1 if not defined
    -s [3 state game of life]
    -w [wraparound]
        ** if either -s or -w are not set, default to hw3 logic.
        ** if BOTH -s and -w are set, print error message and return 1.