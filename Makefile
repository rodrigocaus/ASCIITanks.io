DIR = src/
HPP = $(shell find $(DIR) -name '*.hpp')
SRC = $(shell find $(DIR) -name '*.cpp')
EXE = tanks.ea
FLAGS = -std=c++11 -lncurses -lpthread -lportaudio


all: model

model: $(SRC) $(HPP)
	g++ $(SRC) -o $(EXE) $(FLAGS)

play: model
	./$(EXE)
