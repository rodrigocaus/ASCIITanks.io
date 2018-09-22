/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include <thread>
#include <cmath>

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
  public:
  Tanque(Coordenada posicao, int vida, int balaMax, char direcao, float velocidadePadrao = 0.025);
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
	void verificaTanquesMortos();
};

class Fisica {
  private:
    ListaDeTanques *ldt;
    ListaDeBalas *ldb;
	float maxX, maxY;
  public:
    Fisica(ListaDeTanques *ldt, ListaDeBalas *ldb, float maxX, float maxY);
    void update(float deltaT);

};

class Tela {
  private:
    ListaDeBalas *ldb;
    ListaDeTanques *ldt;
    int maxI, maxJ;

  public:
    Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ);
    ~Tela();
    void stop();
    void init();
    void update();
};

void threadFunction(char *keybuffer, int *control);

class Teclado {
  private:
    char ultimaCaptura;
    int rodando;

    std::thread kbThread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getChar();
};

#endif
