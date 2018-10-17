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

  	//Cria o objeto responsável por receber o estado de jogo da rede
  	Rede::Receptor * receptor = new Rede::Receptor();
  	receptor->config();
  	receptor->conecta();

  	//Tamanhos das listas a serem desserializadas
  	size_t ldbTam , ldtTam;

  	//Strings das listas serializadas
  	std::string ldbSerial , ldtSerial;


  	while(1){

  	//Recebe os tamanhos das listas
  	receptor->receberTamanho(&ldbTam , &ldtTam);

  	ldb->limpaLista();
  	ldt->limpaLista();
  	ldbSerial.clear();
  	ldtSerial.clear();

  	receptor->receberLista(&ldbSerial , ldbTam);
  	receptor->receberLista(&ldtSerial , ldtTam);

  	ldb->deserializaLista(ldbSerial);
  	ldt->deserializaLista(ldtSerial);

  	// Atualiza tela
    tela->update();

  	}

  	tela->stop();
  	receptor->stop();

	return 0;
  	
}
