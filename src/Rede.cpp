/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "Rede.hpp"

Tranmissor::Tranmissor() {}

Transmissor::~Transmissor() {}

Transmissor::stop()
{
	close(socket_fd);
}
 
Transmissor::config() {

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
		cerr << "Erro ao efetuar o bind";
	}
}

Transmissor::iniciaTransmissao(){

	//Servidor aberto para requisição de comunicação
	listen(socket_fd, 2);

	//Travando até receber alguma requisição
	this->connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
}

Transmissor::transmitirJogo(std::string sEnvio)
{
	//Enviando estado de jogo serializado
    if (send(connection_fd, (void *)sEnvios.c_str() , sEnvio.length() + 1 , 0) < 0) {
      printf("Erro ao enviar mensagem de retorno\n");
    } 
}

Receptor::Receptor() {}

Receptor::~Receptor() {}

Receptor::stop()
{
	close(socket_fd);
}

Receptor::config() {

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket criado\n");

	//Configurando o meu socket para IPv4 e porta 3001
	target.sin_family = AF_INET;
	target.sin_port = htons(3001);

	//Converte o meu endereço IP na forma de string para forma da struct
	inet_aton("127.0.0.1", &(target.sin_addr));
}

Receptor::conecta() {

	//Estabelece a conexão
	if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    	cerr << "Erro em conectar com o servidor";
    }
}

Receptor::recebeJogo()
{
	//TODO
}



