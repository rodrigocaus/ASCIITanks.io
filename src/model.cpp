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
  this->velocidade = {0.0, 0.0};
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

  Bala *ListaDeBalas::removeBala(int index) {
    Bala *b = (*(this->balas))[index];
    (this->balas)->erase((this->balas)->begin() + index);
    return b;
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

Fisica::Fisica(ListaDeTanques *ldt , ListaDeBalas *ldb, float maxX, float maxY) {
  this->ldt = ldt;
  this->ldb = ldb;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Fisica::update(float deltaT) {
  // Atualiza posicao dos tanques
  std::vector<Tanque *> *t = this->ldt->getTanques();
  for(int i = 0; i < t->size(); i++) {
    Coordenada novaPosicao;
    novaPosicao = (*t)[i]->getPosicao();
    novaPosicao.x = novaPosicao.x + ((*t)[i]->getVelocidade()).x * deltaT;
    novaPosicao.y = novaPosicao.y + ((*t)[i]->getVelocidade()).y * deltaT;
    (*t)[i]->updatePosicao(novaPosicao);
  }

  // Atualiza posicao das balas
  std::vector<Bala *> *b = this->ldb->getBalas();
  for(int i = 0; i < b->size(); i++) {
    Coordenada novaPosicao;
    novaPosicao = (*b)[i]->getPosicao();
    novaPosicao.x = novaPosicao.x + ((*b)[i]->getVelocidade()).x * deltaT;
    novaPosicao.y = novaPosicao.y + ((*b)[i]->getVelocidade()).y * deltaT;
    // Fora dos limites, a bala é destruida
    if(novaPosicao.x > this->maxX || novaPosicao.y > this->maxY
        || novaPosicao.x < 0 || novaPosicao.y < 0) {
        delete this->ldb->removeBala(i);
        i--;
    }
    else {
        (*b)[i]->updatePosicao(novaPosicao);
    }

  }

}

Tela::Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ) {
  this->ldt = ldt;
  this->ldb = ldb;
  this->maxI = maxI;
  this->maxJ = maxJ;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
  raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  Coordenada pos;
  // Apaga todos os corpos da tela
  clear();

  // Desenha balas na tela
  std::vector<Bala *> *balas = this->ldb->getBalas();

  for (int k=0; k<balas->size(); k++)
  {
    pos = ((*balas)[k]->getPosicao());

    move((int) pos.x, (int) pos.y);   /* Move cursor to position */
    echochar('o');  /* Prints character, advances a position */
  }

  // Desenha tanques na tela
  std::vector<Tanque *> *tanques = this->ldt->getTanques();

  for (int k=0; k<tanques->size(); k++)
  {
    pos = ((*tanques)[k]->getPosicao());

    move((int) pos.x, (int) pos.y);   /* Move cursor to position */
    char dir = (*tanques)[k]->getDirecao();
    switch (dir) {
        case 'w':
            echochar('^');
            break;
        case 'a':
            echochar('<');
            break;
        case 's':
            echochar('v');
            break;
        case 'd':
            echochar('>');
            break;
        default:
            // Erro de direcao
            echochar('e');
            break;
    }
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

void threadFunction(char *keybuffer, int *control)
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
