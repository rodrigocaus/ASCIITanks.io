/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Som.hpp"
#include "../Teclado.hpp"
#include "../Fisica.hpp"
#include "../Tela.hpp"
#include "../Bot.hpp"
#include "../Rede.hpp"

//Tamanho da janela de jogo
#define MAXX 30
#define MAXY 60

//Pega o tempo em milisegundos
using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{

	ListaDeBalas *ldb = new ListaDeBalas();
	ListaDeTanques *ldt = new ListaDeTanques();

	Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  	tela->init();

  	//Inicializa o teclado
  	Teclado *teclado = new Teclado();
	teclado->init();

	//Limpa eventuais comandos pre-buffered
	teclado->getChar();

  	//Cria o objeto responsável por receber o estado de jogo da rede
  	Rede::Cliente * cliente = new Rede::Cliente();
  	cliente->config();
  	cliente->conecta();

  	//Tamanhos das listas a serem desserializadas
  	size_t ldbTam , ldtTam;

  	//Strings das listas serializadas
  	std::string ldbSerial , ldtSerial;

  	while(1){

	  	//Recebe os tamanhos das listas
	  	cliente->receberTamanho(&ldbTam , &ldtTam);
	  	if(ldtTam < 1) break; //Encerra o programa se não houver mais tanques (jogador morreu)

	  	//Zera as srings de serial e as listas de tanques e balas, para receber as novas
	  	ldb->limpaLista();
	  	ldt->limpaLista();
	  	ldbSerial.clear();
	  	ldtSerial.clear();

	  	//Recebe as novas listas de balas e tanques de forma serial
	  	cliente->receberLista(ldbSerial , ldbTam);
	  	cliente->receberLista(ldtSerial , ldtTam);

	  	// Lê o teclado e envia o comando ao servidor
		char c = teclado->getChar();
		cliente->enviarComando(c);

	  	//Deserializa as listas para criar os novos objetos de lista
	  	ldb->deserializaLista(ldbSerial);
	  	ldt->deserializaLista(ldtSerial);

	  	// Atualiza tela
	    tela->update();
  	}

  	tela->stop();
  	teclado->stop();
  	cliente->stop();
  	std::cerr << "Matou o programa\n";

	return 0;
  	
}
