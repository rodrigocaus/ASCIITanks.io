DIR = src/
SERVER_HPP = $(DIR)model.hpp $(DIR)Fisica.hpp $(DIR)Rede.hpp
SERVER_SRC = $(DIR)model.cpp $(DIR)Fisica.cpp $(DIR)Rede.cpp
CLIENT_HPP = $(DIR)model.hpp $(DIR)Rede.hpp $(DIR)Teclado.hpp $(DIR)Tela.hpp $(DIR)Som.hpp
CLIENT_SRC = $(DIR)model.cpp $(DIR)Rede.cpp $(DIR)Teclado.cpp $(DIR)Tela.cpp $(DIR)Som.cpp
CLIENT_MUDO_HPP = $(DIR)model.hpp $(DIR)Rede.hpp $(DIR)Teclado.hpp $(DIR)Tela.hpp
CLIENT_MUDO_SRC = $(DIR)model.cpp $(DIR)Rede.cpp $(DIR)Teclado.cpp $(DIR)Tela.cpp
SERVER_MAIN := $(wildcard $(DIR)server/*.cpp)
CLIENT_MAIN := $(wildcard $(DIR)client/*.cpp)

GAME = tanks.ea
GAME_MUDO = tanks_mudo.ea
SERVER = server.ea

FLAGS = -std=c++11 -lncurses -lpthread -lportaudio

IP=127.0.0.1

all: $(GAME)

$(GAME): $(CLIENT_HPP) $(CLIENT_SRC) $(CLIENT_MAIN)
	g++ $(CLIENT_SRC) $(CLIENT_MAIN) -o $(GAME) $(FLAGS) -DAUDIO_ON

$(GAME_MUDO): $(CLIENT_MUDO_HPP) $(CLIENT_MUDO_SRC) $(CLIENT_MAIN)
	g++ $(CLIENT_MUDO_SRC) $(CLIENT_MAIN) -o $(GAME_MUDO) $(FLAGS) -UAUDIO_ON

play_mudo: $(GAME_MUDO)
	./$(GAME_MUDO) $(IP) 2>gameMudo.log

play: $(GAME)
	./$(GAME) $(IP) 2>game.log

$(SERVER): $(SERVER_HPP) $(SERVER_SRC) $(SERVER_MAIN)
	g++ $(SERVER_SRC) $(SERVER_MAIN) -o $(SERVER) $(FLAGS)

server: $(SERVER)
	./$(SERVER) $(IP) 2>server.log

clear:
	rm -f $(GAME) $(GAME_MUDO) $(SERVER) game.log gameMudo.log server.log
