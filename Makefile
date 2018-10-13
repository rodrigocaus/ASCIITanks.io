DIR = src/
HPP := $(wildcard $(DIR)*.hpp)
SRC := $(wildcard $(DIR)*.cpp)
SERVER := $(wildcard $(DIR)server/*.cpp)
CLIENT := $(wildcard $(DIR)client/*.cpp)
GAME = tanks.ea
SPEC = watch.ea
FLAGS = -std=c++11 -lncurses -lpthread -lportaudio -DAUDIOON


all: $(GAME)

$(GAME): $(SRC) $(HPP) $(SERVER)
	g++ $(SRC) $(SERVER) -o $(GAME) $(FLAGS)

compilenoaudio: $(SRC) $(HPP) $(SERVER)
	g++ $(SRC) $(SERVER) -o $(GAME) $(FLAGS) -UAUDIOON

play: $(GAME)
	./$(GAME) 2>warning.log

$(SPEC): $(SRC) $(HPP) $(CLIENT)
	g++ $(SRC) $(CLIENT) -o $(SPEC) $(FLAGS)

spec: $(SPEC)
	./$(SPEC)

clear:
	rm -f $(GAME) $(SPEC) warning.log
