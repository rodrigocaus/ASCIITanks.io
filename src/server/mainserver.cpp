/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Fisica.hpp"
#include "../Rede.hpp"
#include "../Cor.hpp"

#define MAX_JOGADORES 4

//Pega o tempo em milisegundos
using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main (int argc, char *argv[])
{

  std::vector<jogador> jogadores;
  bool deletar = false;

  ListaDeBalas *ldb = new ListaDeBalas();
  ListaDeTanques *ldt = new ListaDeTanques();

  // Implementação do sistema de matchmaking
  //Pergunta e valida a resposta sobre quantos jogarores terao
  int n_clientes;
  std::string nome_jogador;
  while(1){
    std::cout << "Digite o numero de jogadores: ";
    if (std::cin >> n_clientes && n_clientes <= MAX_JOGADORES) {
        break;
    } else {
        std::cout << "Digite um valor positivo e menor ou igual a " << MAX_JOGADORES << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  //Inicialização do socket e aguarda contato do jogador
  //Pergunta pelo endereco IP do servidor para poder configuirar o socket
  //Ou pula essa etapa se o IP ja estiver passado como parametro argv
  Rede::Servidor * servidor = new Rede::Servidor();
  std::string endereco_ip;

  if(argc < 2){
    while(1){
      std::cout << "Digite o endereco IP desse servidor no formato xxx.xxx.xxx.xxx: ";
      if (std::cin >> endereco_ip && servidor->config(endereco_ip.c_str())) {
          break;
      } else {
          std::cout << "O IP nao esta em um formato valido !" << std::endl;
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  } else {
    servidor->config(argv[1]);
  }
  std::cout << "Aguardando conexao dos clientes ..." << std::endl;
 
  //Conecta os clientes, construindo as structs jogadores com informações de nome e id
  servidor->conectaClientes(n_clientes , jogadores);

  //Cria os tanques do jogadores
  for(int id = 0; id < n_clientes ; id++){

    Tanque *tanque = new Tanque({10.0, 10.0}, 3, 3, 'd' , 0.12 , id);
    ldt->addTanque(tanque);
  }

  //Inicialização do modelo físico
  Fisica *f = new Fisica(ldt, ldb, (float) MAXX, (float) MAXY);

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  int periodo = 0;

  //String que receberá a serialização das listas de balas e tanques
  std::string ldbSerial;
  std::string ldtSerial;

  //Inicializa a thread que assincronamente recebe os comandos dos clientes
  servidor->initReceberComando(&jogadores, &deletar);

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

    //Tamanho das listas geradasw
    size_t tamListas[2] = {ldbSerial.size(),ldtSerial.size()};

    //Envia os tamanhos das listas para os clientes saberem o quanto deve receber
    servidor->transmitirTamanho(tamListas , jogadores);

    //Envia a lista de balas
    servidor->transmitirLista(ldbSerial , jogadores);

    //Envia a lista de tanques
    servidor->transmitirLista(ldtSerial , jogadores);

    //Verifica os comandos dos jogadores
    for(size_t i = 0; i < jogadores.size(); i++) {
        if (jogadores[i].ativo == false) {

            std::cout << "Jogador " << jogadores[i].nome << " saiu da partida" << std::endl;
            ldt->removeTanque(jogadores[i].id);
            close(jogadores[i].conexao_fd);
            jogadores.erase(jogadores.begin() + i);
            deletar = false;
            i--;

        } else {

            Bala *novaBala = ldt->comandaTanque(jogadores[i].id , jogadores[i].comando);
            jogadores[i].comando = 0;
            if(novaBala != NULL) ldb->addBala(novaBala);
        }
    }

    if(jogadores.size() == 0) break;

    // Recarrega os tanques periodicamente
    if (periodo == 100) {
        ldt->incrementaMunicao(); //Recarrega
        periodo = 0;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    periodo++;
  }

  //Encerra o programa
  servidor->stop();
  std::cout << "Fim de jogo" << std::endl;

  return 0;
}
