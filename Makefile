DIR = src/
HPP := $(wildcard $(DIR)*.hpp)
SRC := $(wildcard $(DIR)*.cpp)
EXE = tanks.ea
FLAGS = -std=c++11 -lncurses -lpthread -lportaudio


all: $(EXE)

$(EXE): $(SRC) $(HPP)
	g++ $(SRC) -o $(EXE) $(FLAGS)

play: $(EXE)
	./$(EXE) 2>warning.log

clear: $(EXE)
	rm $(EXE)
