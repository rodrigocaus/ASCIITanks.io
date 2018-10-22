/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Fisica.hpp"
#include "../Bot.hpp"
#include "../Rede.hpp"
#include "../Teclado.hpp"

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

  ListaDeBalas *ldb = new ListaDeBalas();
  ListaDeTanques *ldt = new ListaDeTanques();

  //Inicialização do socket e aguarda contato do cliente
  Rede::Servidor * servidor = new Rede::Servidor();
  servidor->config();
  servidor->iniciaTransmissao();

  //Inicialização do tanque do jogador e inclusão na lista de tanques
  Tanque *meuTanque = new Tanque({10.0, 10.0}, 3, 3, 'd');
  ldt->addTanque(meuTanque);

  //Inicialização do modelo físico
  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  //Inicialização do primeiro tanque inimigo
  Bot *bot = new Bot(ldt, meuTanque, MAXX - 2);

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  bool alguemMorreu = false;
  int i = 0;
  int minhaVidaAntes = 0;
  int minhaVidaAgora = 0;
  char c;

  //String que receberá a serialização das listas de balas e tanques
  std::string ldbSerial;
  std::string ldtSerial;

  t1 = get_now_ms();

  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    //Vida antes da atualização do modelo
    minhaVidaAntes = meuTanque->getVida();
    // Atualiza modelo
    f->update(deltaT);
    //Vida após a atualização do modelo
    minhaVidaAgora = meuTanque->getVida();

    //Verifica se o tanque morreu
    if(minhaVidaAgora <= 0) {
        //Game Over

        //Envia tam da lista de tanques como zero, indincando que o jogo acabou
        size_t tamListas[2] = {0,0};
        servidor->transmitirTamanho(tamListas);
        
        break;
    } 
    //Verifica se levou dano
    ldt->verificaTanquesMortos();

    //Secção de serialização das listas e envio para a rede
    ldb->serializaLista(ldbSerial);
    ldt->serializaLista(ldtSerial);

    //Tamanho das listas geradas
    size_t tamListas[2] = {ldbSerial.size(),ldtSerial.size()};

    //Envia os tamanhos das listas para o cliente saber o quanto deve receber
    servidor->transmitirTamanho(tamListas);

    //Envia a lista de balas
    servidor->transmitirLista(ldbSerial);

    //Envia a lista de tanques
    servidor->transmitirLista(ldtSerial);

    //Recebe comando do cliente
    servidor->receberComando(&c);

    //Se houve uma bala gerada, reproduz som de tiro
    Bala *novaBala = meuTanque->comando(c);
    if(novaBala != NULL){
      ldb->addBala(novaBala);
    }
    if (c == 'q') {
      //Sair do jogo

      size_t tamListas[2] = {0,0};
      servidor->transmitirTamanho(tamListas);
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

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    i++;
  }

  //Encerra o programa
  servidor->stop();

  return 0;
}

