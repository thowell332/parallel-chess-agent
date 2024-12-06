CC = g++
SRC = src
BIN = bin
CPPFLAGS = -Iexternal -std=c++17 -g -fopenmp
HEADERS = $(SRC)/AlphaBeta.hpp $(SRC)/GameNode.hpp
# no main .o files, main .o file linked by name in recipe
OBJECTS = $(BIN)/AlphaBeta.o $(BIN)/GameNode.o

all: $(BIN)/AlphaBetaTest $(BIN)/TimingTests

####################################[BIN]#######################################
$(BIN)/AlphaBetaTest: $(OBJECTS) $(BIN)/AlphaBetaTest.o
	mkdir -p $(BIN)
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN)/TimingTests: $(OBJECTS) $(BIN)/TimingTests.o
	mkdir -p $(BIN)
	$(CC) $(CPPFLAGS) $^ -o $@

################################################################################

$(BIN)/GameNode.o: $(SRC)/GameNode.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/AlphaBeta.o: $(SRC)/AlphaBeta.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/AlphaBetaTest.o: $(SRC)/test/AlphaBetaTest.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

$(BIN)/TimingTests.o: $(SRC)/test/TimingTests.cpp $(HEADERS)
	mkdir -p $(BIN)
	${CC} $(CPPFLAGS) $< -c -o $@

clean:
	rm -f $(BIN)/*
