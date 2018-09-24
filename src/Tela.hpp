/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef TELA_HPP
#define TELA_HPP

#include <ncurses.h>

class Tela {
  private:
    ListaDeBalas *ldb;
    ListaDeTanques *ldt;
    int maxI, maxJ;
    WINDOW *janelaDeJogo;
  public:
    Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ);
    ~Tela();
    void stop();
    void init();
    void update();
    void imprimeVida(int v);
    void fimDeJogo();
};

#endif
