/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "playback.hpp"
#include "Teclado.hpp"
#include "Fisica.hpp"
#include "Tela.hpp"

#define MAXX 30
#define MAXY 60

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{
/*
  Audio::Sample *asample;
  asample = new Audio::Sample();
  asample->load("assets/blip.dat");

  Audio::Player *player;
  player = new Audio::Player();
  player->init();
*/

  Bala *b2 = new Bala({0.0, 0.01}, {7, 5});
  Bala *b3 = new Bala({0.01, 0.0}, {2, 10});

  ListaDeBalas *ldb = new ListaDeBalas();
  ldb->addBala(b2);
  ldb->addBala(b3);

  Tanque *tanque = new Tanque({10.0, 10.0}, 1, 1, 'd');
  Tanque *morre = new Tanque({20.0, 20.0}, 1, 1, 'a', 0.025, true);
  morre->updateVelocidade({0.002, 0.002});
  ListaDeTanques *ldt = new ListaDeTanques();
  ldt->addTanque(tanque);
  ldt->addTanque(morre);


  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;

  int i = 0;

  t1 = get_now_ms();

  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    // Atualiza modelo
    f->update(deltaT);

    // Atualiza tela
    tela->update();

    //Verifica se o tanque morreu
    if(tanque->getVida() <= 0) {
        //Game Over
        break;
    }
    ldt->verificaTanquesMortos();

    // Lê o teclado
    char c = teclado->getChar();
    Bala *novaBala = tanque->comando(c);
    if(novaBala != NULL){
        ldb->addBala(novaBala);
    }
    if (c == 'q') {
      //Sair do jogo
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    i++;
  }

  //player->stop();
  tela->stop();
  teclado->stop();

  return 0;
}
