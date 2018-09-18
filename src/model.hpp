/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>

typedef struct {
	float x;
	float y;
} Coordenada;


class Tanque {
  private:
  Coordenada velocidade;
  Coordenada posicao;
  int vidaAtual;
  int vidaMax;
  int balaAtual;
  int balaMax;
  char direcao;

  public:
  Tanque(Coordenada velocidade, Coordenada posicao, int vidaMax, int balaAtual, int balaMax, char direcao);
  void updateMovi(Coordenada novaPosicao, Coordenada novaVelocidade, char novaDirecao);
  void updateBala(int novaBalaAtual);
  void updateVida(int novaVidaAtual);

  Coordenada getVelocidade();
  Coordenada getPosicao();
  int getVidaAtual();
  int getBalaAtual();
  char getDirecao();
};

class Bala {
  private:
  Coordenada velocidade;
  Coordenada posicao;

  public:
  Bala(Coordenada velocidade, Coordenada posicao);
  void updateMovi(Coordenada novaPosicao);

  Coordenada getVelocidade();
  Coordenada getPosicao();
};

class ListaDeBalas {
 private:
    std::vector<Corpo*> *corpos;

  public:
    ListaDeCorpos();
    void hard_copy(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    std::vector<Corpo*> *get_corpos();
};

class Fisica {
  private:
    ListaDeCorpos *lista;

  public:
    Fisica(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    void choque(char direction);
    void update(float deltaT);
};

class Tela {
  private:
    ListaDeCorpos *lista, *lista_anterior;
    int maxI, maxJ;
    float maxX, maxY;

  public:
    Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
};

void threadfun(char *keybuffer, int *control);

class Teclado {
  private:
    char ultima_captura;
    int rodando;

    std::thread kb_thread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
};

#endif
