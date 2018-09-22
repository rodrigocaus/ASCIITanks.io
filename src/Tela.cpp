
#include "model.hpp"
#include "Tela.hpp"
#include <ncurses.h>

using namespace std::chrono;


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
    addch('o');  /* Prints character, advances a position */
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
            addch('^');
            break;
        case 'a':
            addch('<');
            break;
        case 's':
            addch('v');
            break;
        case 'd':
            addch('>');
            break;
        default:
            // Erro de direcao
            addch('e');
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
