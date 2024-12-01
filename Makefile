CC = g++
SRC = src
BIN = bin
CPPFLAGS = -Iexternal -std=c++17 -g -fopenmp
HEADERS = $(SRC)/AlphaBeta.hpp $(SRC)/GameNode.hpp
# no main .o files, main .o file linked by name in recipe
OBJECTS = $(BIN)/AlphaBeta.o $(BIN)/GameNode.o

all: $(BIN)/AlphaBetaTest $(BIN)/ParallelAlphaBetaTest

$(BIN)/AlphaBetaTest: $(OBJECTS) $(BIN)/AlphaBetaTest.o
	mkdir -p $(BIN)
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN)/ParallelAlphaBetaTest: $(OBJECTS) $(BIN)/ParallelAlphaBetaTest.o
	mkdir -p $(BIN)
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN)/GameNode.o: $(SRC)/GameNode.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/AlphaBeta.o: $(SRC)/AlphaBeta.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/AlphaBetaTest.o: $(SRC)/test/AlphaBetaTest.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/ParallelAlphaBetaTest.o: $(SRC)/test/ParallelAlphaBetaTest.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

clean:
	rm -f $(BIN)/*
