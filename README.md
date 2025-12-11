# ct301_hw6

CT301 FA25 - HW6 - State and Rollback

Nicholas Wright

Files
    - GOLApp - Driver program. Responsible for file validation and initialization of a game string. Input file validation. Reads through the supplied file (or string) and verify it is a playable game.
    - GameOfLife - Game logic. Takes the game string and creates a board, prints lifecycle of the game.
    - ThreeState - Inherits from GameOfLife. Implements decayed state.
    - Wraparound - Inherits from GameOfLife. Implements wraparound game logic.

Usage:
    Flags:
    -f [input_file]
    -g [number_of_generations]
    -p [print_interval]
    -x [pause_interval]
        Pause Interval Commands:
        a [1-D Coordinate] - animate cell at 1-D coordinate
        a [x][y] - animate cell at 2-D coordinates
        d [1-D Coordinate] - kill cell at 1-D coordinate
        d [x][y] - kill cell at 2-D coordinates
        e [1-D Coordinate] - decay cell at 1-D coordinate
        e [x][y] - decay cell at 2-D coordinate
        x - exit pause state and continue
    -a [live_cell_character]
    -d [dead_cell_character]
    -e [decayed_cell_character]
    -o [output_file]




Implementation Notes
    -Wraparound formula - Provided in class: index-1 + width % width (Same for height)
    -CLA parsing - while ((c = getopt(argc, argv, “<flags>”)) != -1) - provided in slides
        - In the <flags> section, there are options:
        + -<flags> means the flag must start with a '-'
        + : means there is a required argument after the flag. If this fails to have the
            required following data, getopt will return ?, and optopt will contain the arg
            that failed to have the necessary option

Old Requirements (v5):
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
    - These flags must be present:
    -f [filename] : if after parsing no filename is found, print error message and return 1.
    -g [number of generations] : default is 10 if not defined
    -p [print intervals] : default is 1 if not defined
    -s [3 state game of life]
    -w [wraparound]
        ** if either -s or -w are not set, default to hw3 logic.
        ** if BOTH -s and -w are set, print error message and return 1.
New Requirements (v6):
    -New getopt flag:
        -r [buffer_size]
            - Default value of 100
            - Rolling buffer - old values fall off
            - i.e. if buffer is 10, store generations 0-9. Once you play
                generation 10 / game 11, generation 0 falls out, and
                generation 10 is appended.
            - Only store the last n generations.
    -New pause flag:
        r [rollback_count]
            - attempt to roll the game back rollback_count generations
            - If this amount is > total generations, or > the number
                of generations run so far, print an error
                but DO NOT EXIT