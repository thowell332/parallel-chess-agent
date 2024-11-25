CC = g++
SRC = src
BIN = bin
CPPFLAGS = -Iexternal -std=c++17 -g
HEADERS = $(SRC)/AlphaBeta.hpp $(SRC)/GameNode.hpp
OBJECTS = $(BIN)/AlphaBetaTest.o $(BIN)/AlphaBeta.o $(BIN)/GameNode.o

all: $(BIN)/AlphaBetaTest

$(BIN)/AlphaBetaTest: $(OBJECTS)
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

clean:
	rm -f $(BIN)/*
