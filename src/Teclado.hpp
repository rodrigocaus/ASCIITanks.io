/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#ifndef TECLADO_HPP
#define TECLADO_HPP

#include <ncurses.h>
#include "model.hpp"

void threadFunction(char *keybuffer);

class Teclado {
  private:
    char ultimaCaptura;
    std::thread kbThread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getChar();
};

#endif
