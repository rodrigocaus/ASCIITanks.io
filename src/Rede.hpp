/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef REDE_HPP
#define REDE_HPP

#define MAX_JOGADORES 2

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Rede {

class Servidor {

    private:
        int socket_fd, conexoes_fd[MAX_JOGADORES];
        std::string nome_jogadores[MAX_JOGADORES];
        struct sockaddr_in myself;

    public:
        Servidor();
        ~Servidor();
        void config();
        void conectaCliente(size_t id_cliente , std::string & nome_cliente);
        void stop();
        void transmitirLista(std::string & sEnvio);
        void transmitirTamanho(size_t * tamListas);
        void receberComando(char * c);
};


class Cliente {

    private:
        int socket_fd;
        struct sockaddr_in target;

    public:
        Cliente();
        ~Cliente();
        void config();
        void conecta();
        void stop();
        void receberLista(std::string & buf, size_t tamanho);
        void receberTamanho(size_t * ldbTam , size_t * ldtTam);
        void enviarComando(char c);
};

}
#endif
