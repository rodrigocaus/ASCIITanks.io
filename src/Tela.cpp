/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "Tela.hpp"
#include "Cor.hpp"

using namespace std::chrono;


Tela::Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ) {
  this->ldt = ldt;
  this->ldb = ldb;
  this->maxI = maxI;
  this->maxJ = maxJ;
}

void Tela::init(int id) {
  initscr();			   /* Start curses mode 	 */
  raw();				  /* Line buffering disabled*/
  curs_set(0);           /* Do not display cursor  */

  this->id = id;
  this->janelaDeJogo = newwin(this->maxI+1, this->maxJ+1, 0, 0);
  this->janelaInfoJogador = newwin(3, this->maxJ+1 , this->maxI+1, 0);
  this->janelaRanking = newwin(this->maxI+1, 20 , 0 , this->maxJ+1);

  start_color();
  // Cor de fundo da janela como branco e texto em preto
  init_pair(Cor::preto + 1, COLOR_BLACK, COLOR_WHITE);
  // Cor de fundo da janela como branco e texto em vermelho
  init_pair(Cor::vermelho + 1, COLOR_RED, COLOR_WHITE);
  // Cor de fundo da janela como branco e texto em verde
  init_pair(Cor::verde + 1, COLOR_GREEN, COLOR_WHITE);
  // Cor de fundo da janela como branco e texto em 'amarelo'
  init_pair(Cor::amarelo + 1, COLOR_YELLOW, COLOR_WHITE);
  // Cor de fundo da janela como branco e texto em azul
  init_pair(Cor::azul + 1, COLOR_BLUE, COLOR_WHITE);
  // Cor de fundo da janela como branco e texto em magenta
  init_pair(Cor::magenta + 1, COLOR_MAGENTA, COLOR_WHITE);
  // Cor de fundo da janela como branco e texto em ciano
  init_pair(Cor::ciano + 1, COLOR_CYAN, COLOR_WHITE);

  //Limpa a tela para remover qualquer coisa que tenha
  erase();
  //Depois colore a tela com a cor branca
  wbkgd(this->janelaDeJogo, COLOR_PAIR(1));
  wbkgd(this->janelaInfoJogador, COLOR_PAIR(1));
  wbkgd(this->janelaRanking, COLOR_PAIR(1));
  box(this->janelaInfoJogador, 0, 0);
  box(this->janelaRanking, 0, 0);
  //E atualiza
  refresh();

}

void Tela::update() {
  Coordenada pos;

  // Apaga todos os corpos da tela
  werase(this->janelaDeJogo);
  werase(this->janelaRanking);
  box(this->janelaInfoJogador, 0, 0);
  box(this->janelaRanking, 0, 0);

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

    //Desenhando o meu estado de jogo
    if((*tanques)[k]->getId() == this->id){

      wattron(this->janelaInfoJogador, COLOR_PAIR(this->id +1));

      mvwprintw(this->janelaInfoJogador , 1, 1, "Meu tanque: Vida: %d/3 | Balas: %d/%d | Kills: %d | Deaths: %d",
                (*tanques)[k]->getVida() , (*tanques)[k]->getBalaAtual() , (*tanques)[k]->getBalaMax(), (*tanques)[k]->getKills(), (*tanques)[k]->getDeaths());
              
      wattroff(this->janelaInfoJogador, COLOR_PAIR(this->id +1));
    }

    pos = ((*tanques)[k]->getPosicao());
    wmove(this->janelaDeJogo, (int) pos.x, (int) pos.y);   /* Move cursor to position */

    //Colore o tanque e o rank dele
    wattron(this->janelaDeJogo, COLOR_PAIR((*tanques)[k]->getId()+1));
    wattron(this->janelaRanking, COLOR_PAIR((*tanques)[k]->getId()+1));

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

    mvwprintw(this->janelaRanking , 1+k , 1 , "ID:%d |K:%d |D:%d",
              (*tanques)[k]->getId() , (*tanques)[k]->getKills(), (*tanques)[k]->getDeaths()); 
              
    wattroff(this->janelaDeJogo, COLOR_PAIR((*tanques)[k]->getId()+1));
    wattroff(this->janelaRanking, COLOR_PAIR((*tanques)[k]->getId()+1));

  }

  // Atualiza tela
  wrefresh(this->janelaDeJogo);
  wrefresh(this->janelaInfoJogador);
  wrefresh(this->janelaRanking);

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
  delwin(this->janelaInfoJogador);
  delwin(this->janelaRanking);
  endwin();
}

Tela::~Tela() {
  this->stop();
}
