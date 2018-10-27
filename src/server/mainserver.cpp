/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Fisica.hpp"
#include "../Bot.hpp"
#include "../Rede.hpp"
#include "../Cor.hpp"

//Tamanho da janela de jogo
#define MAXX 30
#define MAXY 60

#define MAX_JOGADORES 2

//Pega o tempo em milisegundos
using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{

  ListaDeBalas *ldb = new ListaDeBalas();
  ListaDeTanques *ldt = new ListaDeTanques();

  // Implementação do sistema de matchmaking
  int n_clientes;
  std::string nome_jogador;
  std::cout << "Digite o numero de jogadores: ";
  std::cin >> n_clientes;
  while(n_clientes <= 0 || n_clientes > MAX_JOGADORES) {
      std::cout << "Digite um valor positivo e menor ou igual a " << MAX_JOGADORES << std::endl;
      std::cin >> n_clientes;
  }

  //Inicialização do socket e aguarda contato do cliente
  Rede::Servidor * servidor = new Rede::Servidor(n_clientes);
  servidor->config();

  
  for (size_t i = 0; i < n_clientes; i++) {
      servidor->conectaCliente(i, nome_jogador);
      std::cout << nome_jogador << " se conectou\n";
      std::cout << "aguardando " << (n_clientes - i - 1) << " jogadores\n";
  }

  //Inicialização do modelo físico
  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  int i = 0;
  char comandos_clientes[MAX_JOGADORES];

  //String que receberá a serialização das listas de balas e tanques
  std::string ldbSerial;
  std::string ldtSerial;

  t1 = get_now_ms();

  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    // Atualiza modelo
    f->update(deltaT);

    //Verifica os tanques que morreram e da renasce eles
    ldt->verificaTanquesMortos(MAXX , MAXY);

    //Secção de serialização das listas e envio para a rede
    ldb->serializaLista(ldbSerial);
    ldt->serializaLista(ldtSerial);

    //Tamanho das listas geradas
    size_t tamListas[2] = {ldbSerial.size(),ldtSerial.size()};

    //Envia os tamanhos das listas para os clientes saberem o quanto deve receber
    servidor->transmitirTamanho(tamListas);

    //Envia a lista de balas
    servidor->transmitirLista(ldbSerial);

    //Envia a lista de tanques
    servidor->transmitirLista(ldtSerial);

    //Recebe comandos dos clientes
    servidor->receberComando(comandos_clientes);

    //Verifica os jogadores que sairam
    for(size_t id_cliente = 0; id_cliente < n_clientes; id_cliente++){
      if(comandos_clientes[id_cliente] == 'q'){
        servidor->stopCliente(id_cliente);
        ldt->removeTanque(id_cliente);
      }
    }
  
    // Recarrega os tanques periodicamente
    if (i == 50) {
        ldt->incrementaMunicao(); //Recarrega
        i = 0;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    i++;
  }

  //Encerra o programa
  servidor->stopTodos();

  return 0;
}
