/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include <thread>

typedef struct {
	float x;
	float y;
} Coordenada;


class Tanque {
  private:
  Coordenada velocidade;
  Coordenada posicao; //Posicao do caractere do quadrado mais proximo ao dois pontos ██::
  int vida;
  int balaAtual;
  int balaMax;
  char direcao;

  public:
  Tanque(Coordenada posicao, int vida, int balaMax, char direcao);
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
};

class Bala {
  private:
  Coordenada velocidade;
  Coordenada posicao;

  public:
  Bala(Coordenada velocidade, Coordenada posicao);
  void updatePosicao(Coordenada novaPosicao);

  Coordenada getVelocidade();
  Coordenada getPosicao();
};

class ListaDeBalas {
 private:
    std::vector<Bala*> *balas;

  public:
    ListaDeBalas();
    void hardCopy(ListaDeBalas *ldb);
    void addBala(Bala *b);
    std::vector<Bala*> *getBalas();
};

class ListaDeTanques {
 private:
    std::vector<Tanque*> *tanques;

  public:
    ListaDeTanques();
    void hardCopy(ListaDeTanques *ldt);
    void addTanque(Tanque *t);
    std::vector<Tanque*> *getTanques();
};

class Fisica {
  private:
    ListaDeTanques * ldt;
    ListaDeBalas * ldb;

  public:
    Fisica(ListaDeTanques * ldt , ListaDeBalas *ldb);
    void update(float deltaT);

};

class Tela {
  private:
    ListaDeBalas *ldb;
    ListaDeTanques *ldt;
    int maxI, maxJ;
    
  public:
    Tela(ListaDeBalas *ldb, ListaDeTanques *ldt int maxI, int maxJ);
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
