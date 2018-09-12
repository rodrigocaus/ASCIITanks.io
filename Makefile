FLAGS=-lncurses -lpthread -lportaudio -std=c++11

all: model

model: model_mainloop.cpp oo_model.cpp oo_model.hpp 01-playback.hpp 01-playback.cpp
	g++ -omodel model_mainloop.cpp oo_model.cpp 01-playback.cpp $(FLAGS)

test: model
	./model
