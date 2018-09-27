/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "Tela.hpp"

using namespace std::chrono;


Tela::Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ) {
  this->ldt = ldt;
  this->ldb = ldb;
  this->maxI = maxI;
  this->maxJ = maxJ;
}

void Tela::init() {
  initscr();			   /* Start curses mode 	 */
  raw();				  /* Line buffering disabled*/
  curs_set(0);           /* Do not display cursor  */
  this->janelaDeJogo = newwin(this->maxI+1, this->maxJ+1, 0, 0);

  start_color();
  // Cor de fundo da janela como branco e texto em preto
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  // Cor para gerar tanques de outro time
  init_pair(2, COLOR_RED, COLOR_WHITE);

  //Limpa a tela para remover qualquer coisa que tenha
  erase();
  //Depois colore a tela com a cor branca
  wbkgd(this->janelaDeJogo, COLOR_PAIR(1));
  //E atualiza
  refresh();

}

void Tela::update() {
  Coordenada pos;
  // Apaga todos os corpos da tela
  werase(this->janelaDeJogo);

  // Desenha balas na tela
  std::vector<Bala *> *balas = this->ldb->getBalas();
  for (int k=0; k<balas->size(); k++)
  {
    pos = ((*balas)[k]->getPosicao());

    wmove(this->janelaDeJogo, (int) pos.x, (int) pos.y);   /* Move cursor to position */
    waddch(this->janelaDeJogo, 'o');  /* Prints character, advances a position */
  }

  // Desenha tanques na tela
  std::vector<Tanque *> *tanques = this->ldt->getTanques();

  for (int k=0; k<tanques->size(); k++)
  {
    pos = ((*tanques)[k]->getPosicao());

    wmove(this->janelaDeJogo, (int) pos.x, (int) pos.y);   /* Move cursor to position */

    //Colore o tanque de vermelho se for inimigo e preto se for a gente
    if((*tanques)[k]->getTime() == true){
        wattron(this->janelaDeJogo, COLOR_PAIR(2));
    }

    //Desenha o tanque com setas diferentes dependendo da posição apontada
    char dir = (*tanques)[k]->getDirecao();
    switch (dir) {
        case 'w':
            waddch(this->janelaDeJogo, '^');
            break;
        case 'a':
            waddch(this->janelaDeJogo, '<');
            break;
        case 's':
            waddch(this->janelaDeJogo, 'v');
            break;
        case 'd':
            waddch(this->janelaDeJogo, '>');
            break;
        default:
            // Erro de direcao
            waddch(this->janelaDeJogo, 'e');
            break;
    }
    if((*tanques)[k]->getTime() == true){
        wattroff(this->janelaDeJogo, COLOR_PAIR(2));
    }
  }
  // Atualiza tela
  wrefresh(this->janelaDeJogo);
}


void Tela::imprimeVida(int v) {
    // Limpa a região
    for(int i = 0; i < 5; i++) {
        move(0, this->maxJ+2+i);
        addch(' ');
    }
    // Representa a vida com '#'
    for(int i = 0; i < v; i++) {
        move(0, this->maxJ+2+i);
        addch('#');
    }
}

void Tela::fimDeJogo() {
    move(0, this->maxJ+2);
    printw("Game Over");
}

void Tela::stop() {
  delwin(this->janelaDeJogo);
  endwin();
}

Tela::~Tela() {
  this->stop();;
}
