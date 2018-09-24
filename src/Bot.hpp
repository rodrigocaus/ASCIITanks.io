/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 *
 *	Classe cujo proposito é controlar tanques cujo timeInimigo = true;
 */

#ifndef BOT_HPP
#define BOT_HPP

#include "model.hpp"
#include <random>

class Bot {
private:
	ListaDeTanques *ldt;
	Tanque *tanquePrincipal;
	int maxSpawnArea;
public:
	Bot(ListaDeTanques *ldt, Tanque *tanquePrincipal, int max = 10);
	~Bot();
	Tanque *spawn();
	void comanda();
	void atira(ListaDeBalas *ldb);
};


#endif
