/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>


typedef struct {
	float x;
	float y;
} Coordenada;

float distancia(Coordenada p, Coordenada q);

class Bala {
  private:
  Coordenada velocidade;
  Coordenada posicao;

  public:
  Bala(Coordenada velocidade, Coordenada posicao);
  Bala(char direcao, Coordenada posicao , float velocidadePadrao = 0.05);
  void updatePosicao(Coordenada novaPosicao);

  Coordenada getVelocidade();
  Coordenada getPosicao();
};


class Tanque {
  private:
  Coordenada velocidade;
  Coordenada posicao;
  int vida;
  int balaAtual;
  int balaMax;
  char direcao;
  float velocidadePadrao;
  bool timeInimigo;

  public:
  Tanque(Coordenada posicao, int vida, int balaMax, char direcao, float velocidadePadrao = 0.025, bool timeInimigo = false);
  void updatePosicao(Coordenada novaPosicao);
  void updateVelocidade(Coordenada novaVelocidade);
  void updateDirecao(char novaDirecao);
  void updateBala(int novaBalaAtual);
  void updateVida(int novaVida);

  Coordenada getVelocidade();
  Coordenada getPosicao();
  int getVida();
  int getBalaAtual();
  int getBalaMax();
  char getDirecao();
  bool getTime();

  Bala *comando(char c);
};

class ListaDeBalas {
 private:
    std::vector<Bala *> *balas;

  public:
    ListaDeBalas();
    void hardCopy(ListaDeBalas *ldb);
    void addBala(Bala *b);
	Bala *removeBala(int index);
    std::vector<Bala *> *getBalas();
};

class ListaDeTanques {
 private:
    std::vector<Tanque *> *tanques;

  public:
    ListaDeTanques();
    void hardCopy(ListaDeTanques *ldt);
    void addTanque(Tanque *t);
    std::vector<Tanque *> *getTanques();
	Tanque *removeTanque(int index);
	bool verificaTanquesMortos();
	void incrementaMunicao();
};

#endif
