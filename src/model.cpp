/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "model.hpp"

#include <ncurses.h>
using namespace std::chrono;

Tanque::Tanque(Coordenada posicao, int vida, int balaMax, char direcao) {
  this->velocidade = {0.0,0.0};
  this->posicao = posicao;
  this->vida = vida;
  this->balaAtual = 0;
  this->balaMax = balaMax;
  this->direcao = direcao;
}

  void Tanque::updatePosicao(Coordenada novaPosicao){
    this->posicao = novaPosicao;
  }

  void Tanque::updateVelocidade(Coordenada novaVelocidade){
    this->velocidade = novaVelocidade;
  }

  void Tanque::updateDirecao(char novaDirecao){
    this->direcao = novaDirecao;
  }

  void Tanque::updateBala(int novaBalaAtual){
    this->balaAtual = novaBalaAtual;
  }

  void Tanque::updateVida(int novaVida){
    this->vida = novaVida;
  }

  Coordenada Tanque::getVelocidade(){
    return velocidade;
  }

  Coordenada Tanque::getPosicao(){
    return posicao;
  }

  int Tanque::getVida(){
    return vida;
  }

  int Tanque::getBalaAtual(){
    return balaAtual;
  }

   int Tanque::getBalaMax(){
    return balaMax;
  }

  char Tanque::getDirecao(){
    return direcao;
  }


Bala::Bala(Coordenada velocidade, Coordenada posicao) {
  this->velocidade = velocidade;
  this->posicao = posicao;
}

  void Bala::updatePosicao(Coordenada novaPosicao){
    this->posicao = novaPosicao;
  }

  Coordenada Bala::getVelocidade(){
    return velocidade;
  }

  Coordenada Bala::getPosicao(){
    return posicao;
  }


ListaDeBalas::ListaDeBalas() {
  this->balas = new std::vector<Bala *>(0);
}

  void ListaDeBalas::hardCopy(ListaDeBalas *ldb) {
    std::vector<Bala *> *balas = ldb->getBalas();

    for (int k=0; k<balas->size(); k++) {
      Bala *b = new Bala((*balas)[k]->getVelocidade(), (*balas)[k]->getPosicao());
      this->addBala(b);
    }
  }

  void ListaDeBalas::addBala(Bala *b) {
    (this->balas)->push_back(b);
  }

  std::vector<Bala*> *ListaDeBalas::getBalas() {
    return (this->balas);
  }


ListaDeTanques::ListaDeTanques() {
  this->tanques = new std::vector<Tanque *>(0);
}

  void ListaDeTanques::hardCopy(ListaDeTanques *ldt) {
    std::vector<Tanque *> *tanques = ldt->getTanques();

    for (int k=0; k<tanques->size(); k++) {
      Tanque *t = new Tanque((*tanques)[k]->getPosicao(), (*tanques)[k]->getVida(),(*tanques)[k]->getBalaMax(), (*tanques)[k]->getDirecao());
      this->addTanque(t);
    }
  }

  void ListaDeTanques::addTanque(Tanque *t) {
    (this->tanques)->push_back(t);
  }

  std::vector<Tanque*> *ListaDeTanques::getTanques() {
    return (this->tanques);
  }

Fisica::Fisica(ListaDeTanques *ldt , ListaDeBalas *ldb) {
  this->ldt = ldt;
  this->ldb = ldb;
}

void Fisica::update(float deltaT) {
  // Atualiza posicao dos tanques!
  std::vector<Tanque *> *t = this->ldt->getTanques();
  for(int i = 0; i < (*t).size(); i++) {
    Coordenada novaPosicao;
    novaPosicao = (*t)[i]->getPosicao();
    novaPosicao.x = novaPosicao.x + ((*t)[i]->getVelocidade()).x * deltaT;
    novaPosicao.y = novaPosicao.y + ((*t)[i]->getVelocidade()).y * deltaT;
    (*t)[i]->updatePosicao(novaPosicao);
  }

  // Atualiza posicao das balas
  std::vector<Bala *> *b = this->ldb->getBalas();
  for(int i = 0; i < (*b).size(); i++) {
    Coordenada novaPosicao;
    novaPosicao = (*b)[i]->getPosicao();
    novaPosicao.x = novaPosicao.x + ((*b)[i]->getVelocidade()).x * deltaT;
    novaPosicao.y = novaPosicao.y + ((*b)[i]->getVelocidade()).y * deltaT;
    (*b)[i]->updatePosicao(novaPosicao);
  }  
  
}

Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  int i;
  int offset = this->maxI/2;

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  // Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++)
  {
    i = (int) ((*corpos_old)[k]->get_posicao()) * \
        (this->maxI / this->maxX);
    i += offset;

    if(i < this->maxI && i > 0){
        move(i, k);   /* Move cursor to position */
        echochar(' ');  /* Prints character, advances a position */
    }
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++)
  {
    i = (int) ((*corpos)[k]->get_posicao()) * \
        (this->maxI / this->maxX);
    i += offset;

    if(i < this->maxI && i > 0){
        move(i, k);   /* Move cursor to position */
        echochar('*');  /* Prints character, advances a position */
    }

    // Atualiza corpos antigos
    (*corpos_old)[k]->update(  (*corpos)[k]->get_velocidade(),\
                               (*corpos)[k]->get_posicao());
  }

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}

void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
  // Inicializa ncurses
  raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  (this->kbThread) = std::thread(threadFunction, &(this->ultimaCaptura), &(this->rodando));
}

void Teclado::stop() {
  this->rodando = 0;
  (this->kbThread).join();
}

char Teclado::getChar() {
  char c = this->ultimaCaptura;
  this->ultimaCaptura = 0;
  return c;
}
