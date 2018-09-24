/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "Som.hpp"
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

  Audio::Sample *somFight = new Audio::Sample();
  somFight->load("assets/fight.dat");

  Audio::Sample *somTiro = new Audio::Sample();
  somTiro->load("assets/tiro.dat");

  Audio::Sample *somBoom = new Audio::Sample();
  somBoom->load("assets/boom.dat");

  Audio::Sample *somHit = new Audio::Sample();
  somHit->load("assets/hit.dat");

  Audio::Sample *somGameOver = new Audio::Sample();
  somGameOver->load("assets/gameOver.dat");

  Audio::Player *player = new Audio::Player();
  player->init();
  player->play(somFight);

  ListaDeBalas *ldb = new ListaDeBalas();
  Tanque *meuTanque = new Tanque({10.0, 10.0}, 3, 3, 'd');
  ListaDeTanques *ldt = new ListaDeTanques();
  ldt->addTanque(meuTanque);

  Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  tela->init();
  
  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  Teclado *teclado = new Teclado();
  teclado->init();

  Bot *bot = new Bot(ldt, meuTanque, MAXX - 2);

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t tSom0, tSom1;
  bool alguemMorreu = false;
  int i = 0;
  int minhaVidaAntes = 0;
  int minhaVidaAgora = 0;

  t1 = get_now_ms();

  //Limpa eventuais comandos pre-buffered
  teclado->getChar();

  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    minhaVidaAntes = meuTanque->getVida();

    // Atualiza modelo
    f->update(deltaT);

    // Atualiza tela
    tela->update();

    minhaVidaAgora = meuTanque->getVida();

    //Verifica se o tanque morreu
    if(minhaVidaAgora <= 0) {
        //Game Over

        player->pause();
        player->play(somGameOver);

        tSom0 = get_now_ms();
        while (1) {
          std::this_thread::sleep_for (std::chrono::milliseconds(10));
          tSom1 = get_now_ms();
          if (tSom1-tSom0 > 1700) break;
        }
        break;
    } //Verifica se levou dano
    else if(minhaVidaAgora < minhaVidaAntes)
    {
        player->pause();
        somHit->set_position(0);
        player->play(somHit);
    }
    
    //Verifica se os tanques inimigos morreram e produz som
    alguemMorreu = ldt->verificaTanquesMortos();
    if(alguemMorreu){

      player->pause();
      somBoom->set_position(0);
      player->play(somBoom);
      alguemMorreu = false;
    }

    // Lê o teclado
    char c = teclado->getChar();
    Bala *novaBala = meuTanque->comando(c);
    if(novaBala != NULL){

        player->pause();
        somTiro->set_position(0);
        player->play(somTiro);

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

  player->stop();
  tela->stop();
  teclado->stop();

  return 0;
}
