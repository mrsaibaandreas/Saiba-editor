CXX=g++
SRC_DIR=src
INC_DIR=include
BLD_DIR=build
CXXFLAGS=-Wall -Werror -I$(INC_DIR) -g

all:editor_main

editor_main: editor 
		$(CXX) $(CXXFLAGS) $(BLD_DIR)/editor.o $(BLD_DIR)/syscalls.o $(BLD_DIR)/screen.o editor.cpp -o $(BLD_DIR)/editor

editor: screen 
		$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Editor.cpp -o $(BLD_DIR)/editor.o

screen: asm1 
		$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Screen.cpp -o $(BLD_DIR)/screen.o 

asm1:
		as asm/custom_syscall.S -o $(BLD_DIR)/syscalls.o

clean:
		rm -rf $(BLD_DIR)/* 

run:
		$(BLD_DIR)/editor

