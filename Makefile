DIR = src/
SERVER_HPP = $(DIR)model.hpp $(DIR)Fisica.hpp $(DIR)Rede.hpp
CLIENT_HPP = $(DIR)model.hpp $(DIR)Rede.hpp $(DIR)Teclado.hpp $(DIR)Tela.hpp $(DIR)Som.hpp
SERVER_SRC = $(DIR)model.cpp $(DIR)Fisica.cpp $(DIR)Rede.cpp
CLIENT_SRC = $(DIR)model.cpp $(DIR)Rede.cpp $(DIR)Teclado.cpp $(DIR)Tela.cpp $(DIR)Som.cpp
SERVER_MAIN := $(wildcard $(DIR)server/*.cpp)
CLIENT_MAIN := $(wildcard $(DIR)client/*.cpp)

GAME = tanks.ea
SERVER = server.ea

FLAGS = -std=c++11 -lncurses -lpthread -lportaudio -DAUDIOON


all: $(GAME)

$(GAME): $(CLIENT_HPP) $(CLIENT_SRC) $(CLIENT_MAIN)
	g++ $(CLIENT_SRC) $(CLIENT_MAIN) -o $(GAME) $(FLAGS)

noaudio: $(CLIENT_HPP) $(CLIENT_SRC) $(CLIENT_MAIN)
	g++ $(CLIENT_SRC) $(CLIENT_MAIN) -o $(GAME) $(FLAGS) -UAUDIOON

play: $(GAME)
	./$(GAME) 127.0.0.1 2>game.log

$(SERVER): $(SERVER_HPP) $(SERVER_SRC) $(SERVER_MAIN)
	g++ $(SERVER_SRC) $(SERVER_MAIN) -o $(SERVER) $(FLAGS)

server: $(SERVER)
	./$(SERVER) 127.0.0.1 2>server.log

clear:
	rm -f $(GAME) $(SERVER) game.log server.log
