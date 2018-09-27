/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "Teclado.hpp"

//Função que roda em thread verificando input de teclado e salvando a ultima tecla registrada
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
