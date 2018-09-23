/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef TELA_HPP
#define TELA_HPP

class Tela {
  private:
    ListaDeBalas *ldb;
    ListaDeTanques *ldt;
    int maxI, maxJ;

  public:
    Tela(ListaDeTanques *ldt, ListaDeBalas *ldb, int maxI, int maxJ);
    ~Tela();
    void stop();
    void init();
    void update();
};

#endif
