/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "playback.hpp"
#include "Teclado.hpp"
#include "Fisica.hpp"
#include "Tela.hpp"
#include "Bot.hpp"

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

  ListaDeBalas *ldb = new ListaDeBalas();

  Tanque *meuTanque = new Tanque({10.0, 10.0}, 3, 3, 'd');
  ListaDeTanques *ldt = new ListaDeTanques();
  ldt->addTanque(meuTanque);


  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

  Bot *bot = new Bot(ldt, meuTanque, MAXX - 2);

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
    if(meuTanque->getVida() <= 0) {
        //Game Over
        break;
    }
    ldt->verificaTanquesMortos();

    // Lê o teclado
    char c = teclado->getChar();
    Bala *novaBala = meuTanque->comando(c);
    if(novaBala != NULL){
        ldb->addBala(novaBala);
    }
    if (c == 'q') {
      //Sair do jogo
      break;
    }

    // Secção de comandos para os tanques
    if (i%25 == 0) {
        ldt->incrementaMunicao();
    }
    if (i%50 == 0) {
        bot->comanda();
    }
    if (i%35 == 0) {
        bot->atira(ldb);
    }
    if (i == 100) {
        ldt->addTanque(bot->spawn());
        i = 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    i++;
  }

  //player->stop();
  tela->stop();
  teclado->stop();

  return 0;
}
