/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "Rede.hpp"

//Função que roda em thread e fica verificando comandos de um jogador e atualizando
void funcRecebeComandos(std::vector<jogador> * jogadores, bool *editando, std::mutex *mtx)
{
  int i = 0;
  while (jogadores->size() > 0) {
      if (!(*editando)) {
          mtx->lock();
          recv((*jogadores)[i].conexao_fd, &((*jogadores)[i].comando) , 1 , MSG_DONTWAIT);
          mtx->unlock();
          i++;
          if (i >= jogadores->size()) i = 0;
      }
  }
  return;
}

using namespace Rede;

Servidor::Servidor() {}

Servidor::~Servidor() { close(socket_fd);}

void Servidor::stop()
{
	close(socket_fd);
	threadRecebeComandos.join();
}

void Servidor::config() {

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Configurando o meu socket para IPv4 e porta 3001
	myself.sin_family = AF_INET;
	myself.sin_port = htons(3001);

	//Converte o meu endereço IP na forma de string para forma da struct
	inet_aton("127.0.0.1", &(myself.sin_addr));

	//Tenta ligar a porta desejada ao nosso socket
	if (bind(this->socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
		std::cerr << "Erro ao efetuar o bind\n";
	}

	//Servidor aberto para requisição de comunicação
	listen(socket_fd, 2);
	std::cerr << "Ouvindo\n";

}

void Servidor::conectaClientes(int n_clientes , std::vector<jogador> & jogadores){

	char nome[21] = {0};

	// Recebe uma conexao de cliente

	for (int i = 0; i < n_clientes; i++) {
      jogadores.push_back(jogador());
      jogadores[i].conexao_fd = accept(this->socket_fd, NULL, NULL);
      recv(jogadores[i].conexao_fd, nome, 20, 0);
      jogadores[i].nome = nome;
      jogadores[i].id = i;
      send(jogadores[i].conexao_fd, &(jogadores[i].id), sizeof(int), 0);
      std::cerr << nome << " se conectou\n";
      std::cerr << "Aguardando " << (n_clientes - i - 1) << " jogadores\n";
  	}
  	std::cerr << "Iniciando Partida\n";
}

void Servidor::transmitirLista(std::string & sEnvio , std::vector<jogador> & jogadores)
{
	if(sEnvio.length() > 0){
		for (size_t i = 0; i < jogadores.size(); i++) {
			//Enviando estado de jogo serializado
		    if (send(jogadores[i].conexao_fd, (void *)sEnvio.c_str() , sEnvio.length() , 0) < 0) {
		      std::cerr << "Erro ao enviar mensagem das listas\n";
		    } else {
		      //std::cerr << "Lista serializada enviada\n";
		    }
		}
	}
}

void Servidor::transmitirTamanho(size_t * tamListas , std::vector<jogador> & jogadores)
{
	//Enviando o tamanho das listas
	for (size_t i = 0; i < jogadores.size(); i++) {
		if (send(jogadores[i].conexao_fd, (void *)tamListas , 2*sizeof(size_t) , 0) < 0) {
	      std::cerr << "Erro ao enviar mensagem de tamanhos\n";
	    } else {
	      //std::cerr << "Tamanho enviado\n";
	    }
	}
}

void Servidor::initReceberComando(std::vector<jogador> * jogadores, bool *editando, std::mutex *mtx)
{
	//Recebendo comandos dos jogadores
	(this->threadRecebeComandos)  = std::thread(funcRecebeComandos, jogadores, editando, mtx);
}

Cliente::Cliente() {}

Cliente::~Cliente() { this->stop();}

void Cliente::stop()
{
	close(socket_fd);
}

void Cliente::config() {

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Configurando o meu socket para IPv4 e porta 3001
	target.sin_family = AF_INET;
	target.sin_port = htons(3001);

	//Converte o meu endereço IP na forma de string para forma da struct
	inet_aton("127.0.0.1", &(target.sin_addr));
}

void Cliente::conecta(std::string &nome_cliente, size_t *id_cliente) {

	//Estabelece a conexão
	if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    	std::cerr << "Erro em conectar com o servidor\n";
    	exit(0);
    } else {
		char nome[21] = {0};
		strncpy(nome, nome_cliente.c_str(), nome_cliente.length());
    	send(socket_fd, nome, 20, 0);
		recv(socket_fd, id_cliente, sizeof(size_t), 0);
		std::cout << "Conectado ao servidor\n";
    }
}

void Cliente::receberLista(std::string & buf, size_t tamanho)
{
	if(tamanho > 0){

	    char *auxbuf = (char *)calloc(tamanho+1,sizeof(char));
		//Recebe o jogo ate o tamanho especificado
		if((recv(socket_fd, (void *)auxbuf, tamanho, MSG_WAITALL))>0){
			//std::cerr << "Recebi lista com sucesso\n";
		} else {
			std::cerr << "Erro ao receber lista\n";
		}
	    buf = auxbuf;
	    free(auxbuf);
	}
}

void Cliente::receberTamanho(size_t * ldbTam , size_t * ldtTam)
{

	size_t tamListas[2] = {0,0};
	//Recebe o tamanho das listas serializadas
	if((recv(socket_fd, (void*)tamListas, 2*sizeof(size_t), MSG_WAITALL))>0){
		//std::cerr << "Recebi tamanho com sucesso\n";
	} else {
		std::cerr << "Erro ao receber tamanho\n";
	}

	*ldbTam = tamListas[0];
	*ldtTam = tamListas[1];
}

int Cliente::enviarComando(char c)
{
	return send(socket_fd, &c , sizeof(char) , 0);
}
