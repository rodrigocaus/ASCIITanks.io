/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Som.hpp"
#include "../Rede.hpp"
#include "../Teclado.hpp"
#include "../Tela.hpp"

//Pega o tempo em milisegundos
using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main(int argc, char *argv[])
{

  //Obtendo e validando o nome do jogador
  std::string nome;
  int id;
  do {
	  std::cout << "Digite um nome com até 20 caracteres: ";
	  std::cin >> nome;
  } while(nome.length() > 20);

  //Criando o objeto de cliente da rede
  Rede::Cliente * cliente = new Rede::Cliente();
  std::string endereco_ip;

  //Configurando a conexao
  //Pergunta pelo endereco IP do servidor para poder configuirar o socket
  //Ou pula essa etapa se o IP ja estiver passado como parametro argv
  if(argc < 2){
    while(1){
      std::cout << "Digite o endereco IP do servidor que deseja se conectar no formato xxx.xxx.xxx.xxx: ";
      if (std::cin >> endereco_ip && cliente->config(endereco_ip.c_str())) {
          break;
      } else {
          std::cout << "O IP nao esta em um formato valido !" << std::endl;
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  } else {
    cliente->config(argv[1]);
  }
  
  //Estabelece a conexao com o servidor
  cliente->conecta(nome, (size_t *) &id);
  std::cout << "Conectado com o id " << (int) id << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  //Cria as listas de balas e tanques
  ListaDeBalas *ldb = new ListaDeBalas();
  ListaDeTanques *ldt = new ListaDeTanques();

  //Inicializa a tela
  Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  tela->init(id);

  //Inicializa o teclado
  Teclado *teclado = new Teclado();
  teclado->init();

  //Limpa eventuais comandos pre-buffered
  teclado->getChar();

  //Tamanhos das listas a serem desserializadas
  size_t ldbTam , ldtTam;

  //Strings das listas serializadas
  std::string ldbSerial , ldtSerial;

 while(1){

	//Zera as strings de serial e as listas de tanques e balas, para receber as novas
	ldb->limpaLista();
	ldt->limpaLista();
	ldbSerial.clear();
	ldtSerial.clear();

	//Recebe os tamanhos das listas
	cliente->receberTamanho(&ldbTam , &ldtTam);

	//Recebe as novas listas de balas e tanques de forma serial
	cliente->receberLista(ldbSerial , ldbTam);
	cliente->receberLista(ldtSerial , ldtTam);

	//Deserializa as listas para criar os novos objetos de lista
	ldb->deserializaLista(ldbSerial);
	ldt->deserializaLista(ldtSerial);

	// Atualiza tela
	tela->update();

	// Lê o teclado e envia o comando ao servidor
	char c = teclado->getChar();
    if(c != 0) {
        cliente->enviarComando(c);
    }
	if(c == 'q'){
		break;
	}

}


cliente->stop();
tela->stop();
teclado->stop();
std::cout << "Fim de jogo\n";

return 0;

}
