/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "Rede.hpp"

using namespace Rede;

Servidor::Servidor() {}

Servidor::~Servidor() { this->stop();}

void Servidor::stop()
{
	close(connection_fd);
	close(socket_fd);
}

void Servidor::config() {

	this->client_size = (socklen_t)sizeof(client);

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Configurando o meu socket para IPv4 e porta 3001
	myself.sin_family = AF_INET;
	myself.sin_port = htons(3001);

	//Converte o meu endereço IP na forma de string para forma da struct
	inet_aton("127.0.0.1", &(myself.sin_addr));

	//Tenta ligar a porta desejada ao nosso socket
	if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
		std::cerr << "Erro ao efetuar o bind\n";
	}
}

void Servidor::iniciaTransmissao(){

	//Servidor aberto para requisição de comunicação
	listen(socket_fd, 2);
	std::cerr << "Ouvindo\n";

	//Travando até receber alguma requisição
	std::cerr << "Travado ate receber uma conexao\n";
	this->connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
	std::cerr << "Recebi uma conexao\n";
}

void Servidor::transmitirLista(std::string & sEnvio)
{
	if(sEnvio.length() > 0){
		//Enviando estado de jogo serializado
	    if (send(connection_fd, (void *)sEnvio.c_str() , sEnvio.length() , 0) < 0) {
	      std::cerr << "Erro ao enviar mensagem das listas\n";
	    } else {
	      //std::cerr << "Lista serializada eniada\n";
	    }
	}
}

void Servidor::transmitirTamanho(size_t * tamListas)
{
	//Enviando o tamanho das listas
    if (send(connection_fd, (void *)tamListas , 2*sizeof(size_t) , 0) < 0) {
      std::cerr << "Erro ao enviar mensagem de tamanhos\n";
    } else {
      //std::cerr << "Tamanho enviado\n";
    }
}

void Servidor::receberComando(char * c)
{
	char comando;
	if((recv(connection_fd, &comando , 1, 0))<=0){
		std::cerr << "Erro ao receber comando do cliente\n";
	}
	*c = comando;
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

void Cliente::conecta() {

	//Estabelece a conexão
	std::cerr << "tentando conectar\n";
	if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    	std::cerr << "Erro em conectar com o servidor\n";
    } else {
    	std::cerr << "Conectado com sucesso\n";
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
