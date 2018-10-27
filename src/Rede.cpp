/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "Rede.hpp"

using namespace Rede;

Servidor::Servidor(int n_clientes) { this->n_clientes = n_clientes;}

Servidor::~Servidor() { this->stop();}

void Servidor::stopTodos()
{
	for (size_t i = 0; i < n_clientes; i++) {
		if(nome_jogadores[i].length() > 0)
			close(conexoes_fd[i]);
	}
	close(socket_fd);
}

void Servidor::stopCliente(int id_cliente)
{

	close(conexoes_fd[id_cliente]);
	(nome_jogadores[id_cliente]).clear();

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

	for (size_t i = 0; i < MAX_JOGADORES; i++) {
		conexoes_fd[i] = 0;
	}

	//Servidor aberto para requisição de comunicação
	listen(socket_fd, 2);
	std::cerr << "Ouvindo\n";

}

void Servidor::conectaCliente(size_t id_cliente , std::string & nome_cliente){

	char nome[21] = {0};
	// Recebe uma conexao de cliente

	this->conexoes_fd[id_cliente] = accept(this->socket_fd, NULL, NULL);
	recv(this->conexoes_fd[id_cliente], nome, 20, 0);
	std::cerr << "Recebi a conexao de " << nome << std::endl;
	this->nome_jogadores[id_cliente] = nome;
	send(this->conexoes_fd[id_cliente], &id_cliente, sizeof(size_t), 0);
	nome_cliente = nome;

}

void Servidor::transmitirLista(std::string & sEnvio)
{
	if(sEnvio.length() > 0){
		for (size_t i = 0; i < n_clientes; i++) {
			//Enviando estado de jogo serializado
		    if (nome_jogadores[i].length() > 0 && send(conexoes_fd[i], (void *)sEnvio.c_str() , sEnvio.length() , 0) < 0) {
		      std::cerr << "Erro ao enviar mensagem das listas\n";
		    } else {
		      //std::cerr << "Lista serializada enviada\n";
		    }
		}
	}
}

void Servidor::transmitirTamanho(size_t * tamListas)
{
	//Enviando o tamanho das listas
	for (size_t i = 0; i < n_clientes; i++) {
		if (nome_jogadores[i].length() > 0 && send(conexoes_fd[i], (void *)tamListas , 2*sizeof(size_t) , 0) < 0) {
	      std::cerr << "Erro ao enviar mensagem de tamanhos\n";
	    } else {
	      //std::cerr << "Tamanho enviado\n";
	    }
	}
}

void Servidor::receberComando(char * c)
{
	//Recebendo comandos dos jogadores
	for (size_t i = 0; i < n_clientes; i++) {
		if (nome_jogadores[i].length() > 0 && recv(conexoes_fd[i], c[i] , MAX_JOGADORES , 0) <= 0) {
	      std::cerr << "Erro ao receber comandos dos jogadores\n";
	    } else {
	      //std::cerr << "Comandos recebidos\n";
	    }
	}
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

void Cliente::enviarComando(char c)
{
	if (send(socket_fd, &c , sizeof(char) , 0) < 0) {
      std::cerr << "Erro ao enviar comando ao servidor\n";
    }
}
