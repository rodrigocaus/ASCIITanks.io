/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef TECLADO_HPP
#define TECLADO_HPP

#include <ncurses.h>
#include "model.hpp"
#include "Rede.hpp"

void threadFunction(Rede::Cliente *cliente , char *keybuffer, int *control);

class Teclado {
  private:
    char ultimaCaptura;
    int rodando;
    std::thread kbThread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init(Rede::Cliente *cliente);
    char getChar();
};

#endif
