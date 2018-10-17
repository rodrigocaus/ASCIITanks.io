/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "Rede.hpp"

using namespace Rede;

Transmissor::Transmissor() {}

Transmissor::~Transmissor() {}

void Transmissor::stop()
{
	close(socket_fd);
}
 
void Transmissor::config() {

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

void Transmissor::iniciaTransmissao(){

	//Servidor aberto para requisição de comunicação
	listen(socket_fd, 2);
	std::cerr << "Ouvindo\n";

	//Travando até receber alguma requisição
	std::cerr << "Travado ate receber uma conexao\n";
	this->connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
	std::cerr << "Recebi uma conexao\n";
}

void Transmissor::transmitirLista(std::string & sEnvio)
{
	//Enviando estado de jogo serializado
    if (send(connection_fd, (void *)sEnvio.c_str() , sEnvio.length() , 0) < 0) {
      std::cerr << "Erro ao enviar mensagem das listas\n";
    } else {
      //std::cerr << "Lista serializada eniada\n";
    }
}

void Transmissor::transmitirTamanho(size_t * tamListas)
{
	//Enviando o tamanho das listas
    if (send(connection_fd, (void *)tamListas , 2*sizeof(size_t) , 0) < 0) {
      std::cerr << "Erro ao enviar mensagem de tamanhos\n";
    } else {
     // std::cerr << "Tamanho enviado\n";
    }
}

Receptor::Receptor() {}

Receptor::~Receptor() {}

void Receptor::stop()
{
	close(socket_fd);
}

void Receptor::config() {

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Configurando o meu socket para IPv4 e porta 3001
	target.sin_family = AF_INET;
	target.sin_port = htons(3001);

	//Converte o meu endereço IP na forma de string para forma da struct
	inet_aton("127.0.0.1", &(target.sin_addr));
}

void Receptor::conecta() {

	//Estabelece a conexão
	std::cerr << "tentando conectar\n";
	if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    	std::cerr << "Erro em conectar com o servidor\n";
    } else {
    	std::cerr << "Conectado com sucesso\n";
    }
}

void Receptor::receberLista(std::string & buf, size_t tamanho)
{
    char *auxbuf = (char *)calloc(tamanho,sizeof(char));
	//Recebe o jogo ate o tamanho especificado
	recv(socket_fd, (void *)auxbuf, tamanho, MSG_WAITALL);
    buf = auxbuf;
    free(auxbuf);
}

void Receptor::receberTamanho(size_t * ldbTam , size_t * ldtTam)
{

	size_t tamListas[2] = {0,0};
	//Recebe o tamanho das listas serializadas
	recv(socket_fd, (void*)tamListas, 2*sizeof(size_t), MSG_WAITALL);

	*ldbTam = tamListas[0];
	*ldtTam = tamListas[1];
}



