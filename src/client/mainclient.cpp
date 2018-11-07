/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "../model.hpp"
#include "../Rede.hpp"
#include "../Teclado.hpp"
#include "../Tela.hpp"

#ifdef AUDIO_ON
	#include "../Som.hpp"
#endif

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

  #ifdef AUDIO_ON
  //Cria objetos de audio e importa os sons que serão usados no jogo
  Audio::Sample *somFight = new Audio::Sample();
  somFight->load("assets/fight.dat");
  Audio::Sample *somTiro = new Audio::Sample();
  somTiro->load("assets/tiro.dat");
  Audio::Sample *somBoom = new Audio::Sample();
  somBoom->load("assets/boom.dat");
  Audio::Sample *somHit = new Audio::Sample();
  somHit->load("assets/hit.dat");
  
  //Cria o objeto de tocador de audio
  Audio::Player *player = new Audio::Player();
  #endif
 
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

  //Variavel que indica se eh o primeiro loop, para reproduzir o som de Fight!
  bool primeiroLoop = true;

  #ifdef AUDIO_ON
  //Varáveis dos meu estado para por comparacao com os novos recebidos, poder disparar sons
  int minhaVidaAntes = 3;
  int meusKillsAntes = 0;
  int meusDeathsAntes = 0;
  Tanque * meuTanque;
  #endif

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

	//Ordena a lista de tanques de acordo com o rank
	ldt->ordena();

	#ifdef AUDIO_ON
	if(primeiroLoop){
		player->init();
  		player->play(somFight);
  		primeiroLoop = false;
	}

	//Verifica se deve tocar algum som, e se sim, qual
	meuTanque = ldt->getTanque(id);
	if(minhaVidaAntes > meuTanque->getVida()){
		//Toca som de tomar hit
        player->pause();
        somHit->set_position(0);
        player->play(somHit);

	} else if(meusKillsAntes < meuTanque->getKills()){
		//Toca som de explosao
		player->pause();
      	somBoom->set_position(0);
      	player->play(somBoom);
	}

	minhaVidaAntes = meuTanque->getVida();
	meusKillsAntes = meuTanque->getKills();
	#endif

	// Atualiza tela
	tela->update();

	// Lê o teclado e envia o comando ao servidor
	char c = teclado->getChar();
    if(c != 0) {
        cliente->enviarComando(c);

        #ifdef AUDIO_ON
        //Reproduz som de tiro se tinha bala
        if(c == ' ' && meuTanque->getBalaAtual() > 0){
        	player->pause();
        	somTiro->set_position(0);
        	player->play(somTiro);
        }
        #endif
    }
	if(c == 'q'){
		break;
	}

}


cliente->stop();
tela->stop();
teclado->stop();
std::cout << "Fim de jogo\n";

#ifdef AUDIO_ON 
player->stop();
#endif

return 0;

}
