OBJ_DIR=obj
BIN_DIR=bin
SRC_DIR=src
INCLUDE_DIR = src

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o,$(SRC_FILES))

CC=g++
CFLAGS=-std=c++11 -O3 -I $(INCLUDE_DIR)

.PHONY: default clean

default: main

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(BIN_DIR)/*
	rm $(OBJ_DIR)/*
