/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 *
 *	Classe cujo proposito é controlar tanques cujo timeInimigo = true;
 */

#include "Bot.hpp"
#include <chrono>

Bot::Bot(ListaDeTanques *ldt, Tanque *tanquePrincipal, int max) {
	this->ldt = ldt;
	this->tanquePrincipal = tanquePrincipal;
	this->maxSpawnArea = max;
	if(this->maxSpawnArea <= 0) {
		this->maxSpawnArea = 10;
	}
}

  Tanque *Bot::spawn() {
	  // O spawn ocorre em um lugar aleatorio
	  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	  std::minstd_rand0 geradorAleatorio(seed);
	  float pos = (float) (geradorAleatorio()%(this->maxSpawnArea));
	  return new Tanque({pos, 2*pos}, 1, 1, 'd', 0.0025, true);
  }

  void Bot::comanda() {
	  std::vector<Tanque *> *t = this->ldt->getTanques();
	  // Um comando de movimentação é gerado aleatoriamente
	  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	  std::minstd_rand0 geradorAleatorio(seed);
	  for (int i = 0; i < t->size(); i++) {
		  if ((*t)[i]->getTime() == true) {
			  int cmd = (int) geradorAleatorio()%4;
			  switch (cmd) {
			  	case 0:
					(*t)[i]->comando('w');
					break;
				case 1:
					(*t)[i]->comando('a');
					break;
				case 2:
					(*t)[i]->comando('s');
					break;
				case 3:
				default:
					(*t)[i]->comando('d');
					break;
			  }
		  }
	  }
  }

  void Bot::atira(ListaDeBalas *ldb) {
	   std::vector<Tanque *> *t = this->ldt->getTanques();
	  for (int i = 0; i < t->size(); i++) {
		  if ((*t)[i]->getTime() == true) {

			  Bala *novaBala = (*t)[i]->comando(' ');
			  if(novaBala != NULL) {
				  ldb->addBala(novaBala);
			  }
		  }
	  }
  }

Bot::~Bot() {

}
