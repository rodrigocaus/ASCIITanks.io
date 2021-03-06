/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "Rede.hpp"

//Função que roda em thread e fica verificando comandos de um jogador e atualizando
void funcRecebeComandos(std::vector<jogador> * jogadores, bool *deletar)
{
  while (jogadores->size() > 0) {
      for(uint32_t i = 0; !(*deletar) && i < jogadores->size() ; i++){
       	recv((*jogadores)[i].conexao_fd, &((*jogadores)[i].comando) , 1 , MSG_DONTWAIT);
        if((*jogadores)[i].comando == 'q'){
        	(*jogadores)[i].ativo = false;
        	*deletar = true;
        	break;
        }
      }
      while(*deletar){}
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

int Servidor::config(const char * endereco_ip) {

	//Converte o meu endereço IP na forma de string para forma da struct
	int conversao = inet_aton(endereco_ip, &(myself.sin_addr));
	if(conversao == 0)return 0;

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Configurando o meu socket para IPv4 e porta 3001
	myself.sin_family = AF_INET;
	myself.sin_port = htons(3001);

	//Tenta ligar a porta desejada ao nosso socket
	if (bind(this->socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
		std::cerr << "Erro ao efetuar o bind\n";
	}

	//Servidor aberto para requisição de comunicação
	listen(socket_fd, 2);
	std::cerr << "Ouvindo\n";
	return 1;

}

void Servidor::conectaClientes(int n_clientes , std::vector<jogador> & jogadores){

	char nome[21] = {0};
	uint32_t tamNome = 0;

	// Recebe uma conexao de cliente

	for (int i = 0; i < n_clientes; i++) {
      jogadores.push_back(jogador());
      jogadores[i].conexao_fd = accept(this->socket_fd, NULL, NULL);
      recv(jogadores[i].conexao_fd, &tamNome, sizeof(uint32_t), 0);
      recv(jogadores[i].conexao_fd, nome, tamNome, MSG_WAITALL);
      jogadores[i].nome = nome;
      jogadores[i].id = i;
      jogadores[i].ativo = true;
      send(jogadores[i].conexao_fd, &(jogadores[i].id), sizeof(uint32_t), 0);
      std::cout << nome << " se conectou\n";
      std::cout << "Aguardando " << (n_clientes - i - 1) << " jogadores\n";
      nome[0] = '\0';
  	}
  	std::cout << "Iniciando Partida\n";
}

void Servidor::transmitirLista(std::string & sEnvio , std::vector<jogador> & jogadores)
{
	if(sEnvio.length() > 0){
		for (uint32_t i = 0; i < jogadores.size(); i++) {
			//Enviando estado de jogo serializado
		    if (jogadores[i].ativo && send(jogadores[i].conexao_fd, (void *)sEnvio.c_str() , sEnvio.length() , 0) < 0) {
		      std::cerr << "Erro ao enviar mensagem das listas\n";
		    } else {
		      //std::cerr << "Lista serializada enviada\n";
		    }
		}
	}
}

void Servidor::transmitirTamanho(uint32_t * tamListas , std::vector<jogador> & jogadores)
{
	//Enviando o tamanho das listas
	for (uint32_t i = 0; i < jogadores.size(); i++) {
		if (jogadores[i].ativo && send(jogadores[i].conexao_fd, (void *)tamListas , 2*sizeof(uint32_t) , 0) < 0) {
	      std::cerr << "Erro ao enviar mensagem de tamanhos\n";
	    } else {
	      //std::cerr << "Tamanho enviado\n";
	    }
	}
}

void Servidor::initReceberComando(std::vector<jogador> * jogadores, bool *deletar)
{
	//Recebendo comandos dos jogadores
	(this->threadRecebeComandos)  = std::thread(funcRecebeComandos, jogadores, deletar);
}

Cliente::Cliente() {}

Cliente::~Cliente() { this->stop();}

void Cliente::stop()
{
	close(socket_fd);
}

int Cliente::config(const char * endereco_ip) {

	//Converte o meu endereço IP na forma de string para forma da struct
	int conversao = inet_aton(endereco_ip, &(target.sin_addr));
	if(conversao == 0) return 0;

	//Socket criado com IPv4 e TCP/IP
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Configurando o meu socket para IPv4 e porta 3001
	target.sin_family = AF_INET;
	target.sin_port = htons(3001);
	return 1;

}

void Cliente::conecta(std::string &nome_cliente, uint32_t * id_cliente) {

	//Estabelece a conexão
	if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
        std::cout << "Erro em conectar com o servidor\n";
    	std::cerr << "Erro em conectar com o servidor\n";
    	exit(0);
    } else {
		char nome[21] = {0};
		strncpy(nome, nome_cliente.c_str(), nome_cliente.length());
		uint32_t tamNome = strlen(nome);
		tamNome++;
		send(socket_fd, &tamNome, sizeof(uint32_t) , 0);
    	send(socket_fd, nome, 20, 0);
		recv(socket_fd, id_cliente, sizeof(uint32_t), 0);
		std::cout << "Conectado ao servidor\n";
    }
}

void Cliente::receberLista(std::string & buf, uint32_t tamanho)
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

void Cliente::receberTamanho(uint32_t * ldbTam , uint32_t * ldtTam)
{

	uint32_t tamListas[2] = {0,0};
	//Recebe o tamanho das listas serializadas
	if((recv(socket_fd, (void*)tamListas, 2*sizeof(uint32_t), MSG_WAITALL))>0){
		//std::cerr << "Recebi tamanho com sucesso\n";
	} else {
		std::cerr << "Erro ao receber tamanho\n";
	}

	*ldbTam = tamListas[0];
	*ldtTam = tamListas[1];
}

int Cliente::enviarComando(char c)
{
	int retorno = send(socket_fd, &c , sizeof(char) , 0);
	return retorno;
}
