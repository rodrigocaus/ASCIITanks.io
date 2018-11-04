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
    int maxI, maxJ, id;
    WINDOW *janelaDeJogo;
    WINDOW *janelaInfoJogador;
  public:
    Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ);
    ~Tela();
    void stop();
    void init(int id);
    void update();
    void imprimeVida(int v);
    void fimDeJogo();
};

#endif
