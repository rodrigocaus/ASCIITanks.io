/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef REDE_HPP
#define REDE_HPP

#include <cstdio>
#include <unistd.h>
#include <string>
#include <vector>
#include <thread>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>

struct jogador {
  int id;
  int conexao_fd;
  char comando;
  bool ativo;
  std::string nome;
};

void funcRecebeComandos(std::vector<jogador> *jogadores, bool *deletar);

namespace Rede {

class Servidor {

    private:
        int socket_fd;
        struct sockaddr_in myself;
        std::thread threadRecebeComandos;

    public:
        Servidor();
        ~Servidor();
        int config(const char * endereco_ip);
        void conectaClientes(int n_clientes , std::vector<jogador> & jogadores);
        void stop();
        void transmitirLista(std::string & sEnvio , std::vector<jogador> & jogadores);
        void transmitirTamanho(uint32_t * tamListas , std::vector<jogador> & jogadores);
        void initReceberComando(std::vector<jogador> * jogadores, bool *deletar);
};


class Cliente {

    private:
        int socket_fd;
        struct sockaddr_in target;

    public:
        Cliente();
        ~Cliente();
        int config(const char * endereco_ip);
        void conecta(std::string &nome_cliente, uint32_t *id_cliente);
        void stop();
        void receberLista(std::string & buf, uint32_t tamanho);
        void receberTamanho(uint32_t * ldbTam , uint32_t * ldtTam);
        int enviarComando(char c);
};

}
#endif
