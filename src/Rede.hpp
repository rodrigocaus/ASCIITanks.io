/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef REDE_HPP
#define REDE_HPP

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Rede {

class Transmissor {

    private:
        int socket_fd, connection_fd;
        struct sockaddr_in myself, client;
        socklen_t client_size;

    public:
        Transmissor();
        ~Transmissor();
        void config();
        void iniciaTransmissao();
        void stop();
        void transmitirJogo(std::string sEnvio);
};


class Receptor {

    private:
        int socket_fd;
        struct sockaddr_in target;

    public:
        Receptor();
        ~Receptor();
        void config();
        void conecta();
        void stop();
        void receberJogo();
}

}
#endif
