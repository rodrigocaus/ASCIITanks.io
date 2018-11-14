/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include <chrono>
#include <cmath>
#include <cstdio>	// stdio.h
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>

//Tamanho da janela de jogo
#define MAXX 30
#define MAXY 60


typedef struct {
	float x;
	float y;
} Coordenada;

float distancia(Coordenada p, Coordenada q);

class Bala {
  private:
  Coordenada velocidade;
  Coordenada posicao;
  int idAtirador;

  public:
  Bala(Coordenada velocidade, Coordenada posicao, int idAtirador);
  Bala(char direcao, Coordenada posicao , int idAtirador, float velocidadePadrao = 0.03);
  void updatePosicao(Coordenada novaPosicao);

  Coordenada getVelocidade();
  Coordenada getPosicao();
  int getIdAtirador();

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& out, const Bala& b);

};


class Tanque {
  private:
  Coordenada velocidade;
  Coordenada posicao;
  int vida;
  int balaAtual;
  int balaMax;
  int id;
  int kills;
  int deaths;
  char direcao;
  float velocidadePadrao;


  public:
  Tanque(Coordenada posicao, int vida, int balaMax, char direcao, float velocidadePadrao , int id, int kills, int deaths);
  Tanque(int maxX, int maxY , int id);
  void updatePosicao(Coordenada novaPosicao);
  void updateVelocidade(Coordenada novaVelocidade);
  void updateDirecao(char novaDirecao);
  void updateBala(int novaBalaAtual);
  void updateVida(int novaVida);
  void updateDeaths(int novaDeaths);
  void updateKills(int novaKills);

  Coordenada getVelocidade();
  Coordenada getPosicao();
  int getVida();
  int getBalaAtual();
  int getBalaMax();
  float getVelocidadePadrao();
  char getDirecao();
  int getId();
  int getDeaths();
  int getKills();

  Bala *comando(char c);

  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& out, const Tanque& t);

};

class ListaDeBalas {
 private:
    std::vector<Bala *> *balas;

 public:
    ListaDeBalas();
	~ListaDeBalas();
    void hardCopy(ListaDeBalas *ldb);
    void addBala(Bala *b);
	Bala *removeBala(int index);
	void limpaLista();
    std::vector<Bala *> *getBalas();

	void serializaLista(std::string &buffer_saida);
	void deserializaLista(std::string buffer_entrada);
	Bala *operator[](size_t n);
	void operator=(const std::string& ldb_serial);
};

class ListaDeTanques {
 private:
    std::vector<Tanque *> *tanques;

  public:
    ListaDeTanques();
	  ~ListaDeTanques();
    void hardCopy(ListaDeTanques *ldt);
    void addTanque(Tanque *t);
	  void removeTanque(int id);
    Bala * comandaTanque(int id , char c);
	  void limpaLista();
    std::vector<Tanque *> *getTanques();
    Tanque * getTanque(int id);
	  bool verificaTanquesMortos(int maxX , int maxY);
	  void incrementaMunicao();
	  void serializaLista(std::string &buffer_saida);
	  void deserializaLista(std::string buffer_entrada);
    void ordena();
	  Tanque *operator[](size_t n);
	  void operator=(const std::string& ldt_serial);
};

//Função de comparação de tanques, inicialmente por kills
bool ordemRanking(Tanque *esquerdo, Tanque *direito);

#endif
