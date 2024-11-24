CC = g++
SRC = src
BIN = bin
CPPFLAGS = -Iexternal -std=c++17
HEADERS = $(SRC)/AlphaBeta.hpp $(SRC)/GameState.hpp
OBJECTS = $(BIN)/AlphaBetaTest.o $(BIN)/GameState.o

all: $(BIN)/AlphaBetaTest

$(BIN)/AlphaBetaTest: $(OBJECTS)
	mkdir -p $(BIN)
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN)/GameState.o: $(SRC)/GameState.cpp $(HEADERS) 
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/AlphaBetaTest.o: $(SRC)/test/AlphaBetaTest.cpp $(HEADERS) 
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

clean:
	rm -f $(BIN)/*
