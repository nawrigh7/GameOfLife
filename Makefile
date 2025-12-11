# Compiler
GPP = g++

# Compilation flags
GPPFlags = -Wall -Wextra -Werror -Wfatal-errors -std=c++20
Debug = -g

# source and output names
# SRC is a space separate list of files need to compile
# For any file you need for your code. Do not include any test files
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

# Space Seprated List of Header Files
# At a minimum you need a GameOfLife.h
HEADER = $(wildcard *.h)

HWNUM = HW6
TARNAME = Nicholas_Wright
TARGET = GOLApp

#Compile
all: $(TARGET)

$(TARGET): $(OBJ)
	$(GPP) $(GPPFlags) $(Debug) $(OBJ) -o $(TARGET)

%.o: %.cpp %.h
	$(GPP) $(GPPFlags) -c $< -o $@

# clean
clean:
	rm -f *.a *.o
	@echo "Removed all object files."

package:
	tar -c Makefile $(SRC) $(HEADER) -f $(HWNUM)_$(TARNAME).tar
