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

  	while(1){


  	// Atualiza tela
    tela->update();

  	}

  	tela->stop();

	return 0;
  	
}
