/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Som.hpp"
#include "../Teclado.hpp"
#include "../Fisica.hpp"
#include "../Tela.hpp"
#include "../Bot.hpp"

//Tamanho da janela de jogo
#define MAXX 30
#define MAXY 60

//Pega o tempo em milisegundos
using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{
#ifdef AUDIOON
  //Cria objetos de audio e importa os sons que serão usados no jogo
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

  //Cria o objeto de tocador de audio e inicia com o som "Fight !"
  Audio::Player *player = new Audio::Player();
  player->init();
  player->play(somFight);
#endif
  ListaDeBalas *ldb = new ListaDeBalas();
  ListaDeTanques *ldt = new ListaDeTanques();

  //Inicialização do tanque do jogador e inclusão na lista de tanques
  Tanque *meuTanque = new Tanque({10.0, 10.0}, 3, 3, 'd');
  ldt->addTanque(meuTanque);

  Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  tela->init();

  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  Teclado *teclado = new Teclado();
  teclado->init();

  //Inicialização do primeiro tanque inimigo
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

    //Vida antes da atualização do modelo
    minhaVidaAntes = meuTanque->getVida();
    // Atualiza modelo
    f->update(deltaT);
    // Atualiza tela
    tela->update();
    //Vida após a atualização do modelo
    minhaVidaAgora = meuTanque->getVida();

    //Verifica se o tanque morreu
    if(minhaVidaAgora <= 0) {
        //Game Over

        //Toca som de game over
#ifdef AUDIOON
        player->pause();
        player->play(somGameOver);
#endif
        //Espera o som acabar antes de encerrar o jogo
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

        //Toca som de hit
#ifdef AUDIOON
        player->pause();
        somHit->set_position(0);
        player->play(somHit);
#endif
    }

    //Verifica se os tanques inimigos morreram e produz som
    alguemMorreu = ldt->verificaTanquesMortos();
    if(alguemMorreu){
#ifdef AUDIOON
      player->pause();
      somBoom->set_position(0);
      player->play(somBoom);
#endif
      alguemMorreu = false;
    }

    // Lê o teclado
    char c = teclado->getChar();

    //Se houve uma bala gerada, reproduz som de tiro
    Bala *novaBala = meuTanque->comando(c);
    if(novaBala != NULL){
#ifdef AUDIOON
        player->pause();
        somTiro->set_position(0);
        player->play(somTiro);
#endif
        ldb->addBala(novaBala);
    }
    if (c == 'q') {
      //Sair do jogo
      break;
    }

    // Secção de comandos para os tanques
    // Realiza ações com base no número de ciclos passados
    if (i%25 == 0) {
        ldt->incrementaMunicao(); //Recarrega
    }
    if (i%50 == 0) {
        bot->comanda(); //Move
    }
    if (i%35 == 0) {
        bot->atira(ldb); //Atira
    }
    if (i == 100) {
        ldt->addTanque(bot->spawn()); //Novo tanque inimigo criado
        i = 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    i++;
  }

  //Encerra o programa
#ifdef AUDIOON
  player->stop();
#endif
  tela->stop();
  teclado->stop();

  return 0;
}
