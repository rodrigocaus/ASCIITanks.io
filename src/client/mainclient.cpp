/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Som.hpp"
#include "../Rede.hpp"
#include "../Teclado.hpp"
#include "../Tela.hpp"


//Tamanho da janela de jogo
#define MAXX 10
#define MAXY 20

//Pega o tempo em milisegundos
using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{

  // Cria o objeto responsável por receber o estado de jogo da rede
  Rede::Cliente * cliente = new Rede::Cliente();
  cliente->config();

  // Fazemos o processo de conexão
  std::string nome;
  int id;
  do {
	  std::cout << "Digite um nome com até 20 caracteres: ";
	  std::cin >> nome;
  } while(nome.length() > 20);

  cliente->conecta(nome, (size_t *) &id);

  std::cout << "Conectado com o id " << (int) id << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));


  ListaDeBalas *ldb = new ListaDeBalas();
  ListaDeTanques *ldt = new ListaDeTanques();

  //Inicializa a tela
  Tela *tela = new Tela(ldt, ldb, MAXX, MAXY);
  tela->init();

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
		if(c == 'q') std::this_thread::sleep_for(std::chrono::milliseconds(250));
		break;
	}

}


cliente->stop();
tela->stop();
teclado->stop();
std::cout << "Fim de jogo\n";

return 0;

}
