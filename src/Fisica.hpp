/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef FISICA_HPP
#define FISICA_HPP

class Fisica {
  private:
    ListaDeTanques *ldt;
    ListaDeBalas *ldb;
	float maxX, maxY;
  public:
    Fisica(ListaDeTanques *ldt, ListaDeBalas *ldb, float maxX, float maxY);
    void update(float deltaT);

};

#endif
