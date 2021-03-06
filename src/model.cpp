/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"

using namespace std::chrono;

//Calcula distancia entre duas coordenadas
float distancia(Coordenada p, Coordenada q) {
	return (float) sqrt((p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y));
}

bool ordemRanking(Tanque *esquerdo, Tanque *direito){
    return (esquerdo->getKills() > direito->getKills());
}

Tanque::Tanque(Coordenada posicao, int vida, int balaMax, char direcao, float velocidadePadrao, int id, int kills , int deaths) {
  this->velocidade = {0.0, 0.0};
  this->posicao = posicao;
  this->vida = vida;
  this->balaAtual = balaMax;
  this->balaMax = balaMax;
  this->direcao = direcao;
  this->velocidadePadrao = velocidadePadrao;
  this->id = id;
  this->deaths = deaths;
  this->kills = kills;
}

Tanque::Tanque(int maxX , int maxY,  int id) {
  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::minstd_rand0 geradorAleatorio(seed);
  this->posicao = {(float) (geradorAleatorio()%maxX), (float) (geradorAleatorio()%maxY)};
  this->velocidade = {0.0, 0.0};
  this->vida = 3;
  this->balaAtual = 3;
  this->balaMax = 3;
  this->direcao = 'd';
  this->velocidadePadrao = 0.025;
  this->id = id;
  this->deaths = 0;
  this->kills = 0;
}

  void Tanque::updatePosicao(Coordenada novaPosicao){
    this->posicao = novaPosicao;
  }

  void Tanque::updateVelocidade(Coordenada novaVelocidade){
    this->velocidade = novaVelocidade;
  }

  void Tanque::updateDirecao(char novaDirecao){
    this->direcao = novaDirecao;
  }

  //Só incrementa bala se tiver menos balas que o máximo
  void Tanque::updateBala(int novaBalaAtual){
	if(novaBalaAtual > this->balaMax) {
		this->balaAtual = this->balaMax;
	}
	else {
		this->balaAtual = novaBalaAtual;
	}
  }

  void Tanque::updateVida(int novaVida){
    this->vida = novaVida;
  }

  void Tanque::updateDeaths(int novaDeaths){
    this->deaths = novaDeaths;
  }

  void Tanque::updateKills(int novaKills){
    this->kills = novaKills;
  }

  Coordenada Tanque::getVelocidade(){
    return velocidade;
  }

  float Tanque::getVelocidadePadrao(){
    return velocidadePadrao;
  }

  Coordenada Tanque::getPosicao(){
    return posicao;
  }

  int Tanque::getVida(){
    return vida;
  }

  int Tanque::getBalaAtual(){
    return balaAtual;
  }

   int Tanque::getBalaMax() {
    return balaMax;
  }

  char Tanque::getDirecao() {
    return direcao;
  }

  int Tanque::getId() {
	return id;
  }

  int Tanque::getDeaths() {
  return deaths;
  }

  int Tanque::getKills() {
  return kills;
  }

  //Realiza movimentos e ações do tanque
  Bala *Tanque::comando(char c) {
      Bala *b = NULL;

      //Nas direções w e s, ou seja, na vertical, há uma escala de redução, pois aproximamos
      //que dois espaços tem a mesma distância horizontal que uma linha tem na horizontal.
      switch (c) {
        case 'w':
          this->updateDirecao('w');
          this->updateVelocidade({-(this->velocidadePadrao)/2, 0.0});
          break;
        case 'a':
          this->updateDirecao('a');
          this->updateVelocidade({0.0, -(this->velocidadePadrao)});
          break;
        case 's':
          this->updateDirecao('s');
          this->updateVelocidade({(this->velocidadePadrao)/2, 0.0});
          break;
        case 'd':
          this->updateDirecao('d');
          this->updateVelocidade({0.0, (this->velocidadePadrao)});
          break;
        case ' ':
          //Atira bala
          if(this->balaAtual > 0) {
              this->balaAtual -= 1;
              Coordenada posBala = this->getPosicao();
              //A bala é criada uma posição a frente da direção onde o tanque está apontando
              switch (this->direcao) {
                  case 'w':
                    posBala.x -= 1.0;
                    break;
                  case 'a':
                    posBala.y -= 1.0;
                    break;
                  case 's':
                    posBala.x += 1.0;
                    break;
                  case 'd':
                  default:
                    posBala.y += 1.0;
                    break;
              }
              b = new Bala(this->direcao , posBala , this->id);
          }
          break;
        default:
          this->updateVelocidade({0.0, 0.0});
          break;

      }
      return b;
  }

  std::string Tanque::toString() const{
	  std::string s = "";

	  s += std::to_string((this->velocidade).x) + "," + std::to_string((this->velocidade).y) + ",";
	  s += std::to_string((this->posicao).x) + "," + std::to_string((this->posicao).y) + ",";
	  s += std::to_string(this->vida) + "," + std::to_string(this->direcao) + ",";
	  s += std::to_string(this->balaAtual) + "," + std::to_string(this->balaMax) + ",";
	  s += std::to_string(this->velocidadePadrao) + "," + std::to_string(this->id) + ",";
    s += std::to_string(this->kills) + "," + std::to_string(this->deaths) + "\n";

	  return s;
  }

  std::ostream& operator<<(std::ostream& out, const Tanque& t) {
	  return out << t.toString();
  }

Bala::Bala(Coordenada velocidade, Coordenada posicao , int idAtirador) {
  this->velocidade = velocidade;
  this->posicao = posicao;
  this->idAtirador = idAtirador;
}

//Segunda opção de construtor: Polar (posição e direção ao invés de posição e velocidade)
Bala::Bala(char direcao, Coordenada posicao, int idAtirador, float velocidadePadrao ) {
    switch (direcao) {
        case 'w':
          this->velocidade = {-velocidadePadrao/2 , 0.0};
          break;
        case 'a':
          this->velocidade = {0.0 , -velocidadePadrao};
          break;
        case 's':
          this->velocidade = {velocidadePadrao/2 , 0.0};
          break;
        case 'd':
        default:
          this->velocidade = {0.0 , velocidadePadrao};
          break;
    }
    this->posicao = posicao;
    this->idAtirador = idAtirador;
}

  void Bala::updatePosicao(Coordenada novaPosicao){
    this->posicao = novaPosicao;
  }

  Coordenada Bala::getVelocidade(){
    return velocidade;
  }

  Coordenada Bala::getPosicao(){
    return posicao;
  }

  int Bala::getIdAtirador(){
    return idAtirador;
  }

  std::string Bala::toString() const{
	  std::string s = "";
	  s += std::to_string((this->velocidade).x) + "," + std::to_string((this->velocidade).y) + ",";
	  s += std::to_string((this->posicao).x) + "," + std::to_string((this->posicao).y) + ",";
    s += std::to_string(this->idAtirador) + "\n";
	  return s;
  }

  std::ostream& operator<<(std::ostream& out, const Bala& b) {
	  return out << b.toString();
  }

ListaDeBalas::ListaDeBalas() {
  this->balas = new std::vector<Bala *>(0);
}

ListaDeBalas::~ListaDeBalas() {
	this->limpaLista();
	delete this->balas;
}

  void ListaDeBalas::hardCopy(ListaDeBalas *ldb) {
    std::vector<Bala *> *balas = ldb->getBalas();

    for (int k=0; k<balas->size(); k++) {
      Bala *b = new Bala((*balas)[k]->getVelocidade(), (*balas)[k]->getPosicao() , (*balas)[k]->getIdAtirador());
      this->addBala(b);
    }
  }

  void ListaDeBalas::addBala(Bala *b) {
    (this->balas)->push_back(b);
  }

  Bala *ListaDeBalas::removeBala(int index) {
    Bala *b = (*(this->balas))[index];
    (this->balas)->erase((this->balas)->begin() + index);
    return b;
  }

  void ListaDeBalas::limpaLista() {
	  for (int i = (this->balas)->size(); i > 0; i--) {
	  	this->removeBala(i - 1);
	  }
  }

  std::vector<Bala*> *ListaDeBalas::getBalas() {
    return (this->balas);
  }

  /*
   * Transforma a lista em uma grande string de objetos (toString)
   */
  void ListaDeBalas::serializaLista(std::string &buffer_saida) {
	  std::string s = "";

	  for (int i = 0; i < (this->balas)->size(); i++) {
	  	s += ((*this)[i])->toString();
	  }
	  buffer_saida = s;
  }

  /*
   * Transforma uma string de objetos (segue a formatação do toString)
   * e adiciona ao final da lista cada objeto.
   * Para reconstruir uma lista, é necessário limpá-la antes da deserialização
   */
  void ListaDeBalas::deserializaLista(std::string buffer_entrada) {
	  Coordenada vel;
	  Coordenada pos;
    int idAtirador;
	  char *s = (char *)buffer_entrada.c_str();
	  while (sscanf(s, "%f,%f,%f,%f,%d\n", &(vel.x), &(vel.y), &(pos.x), &(pos.y), &idAtirador) > 0) {
		  for (; *s != '\n'; s++);
		  s++;
		  Bala *novaBala = new Bala(vel, pos, idAtirador);
		  this->addBala(novaBala);
	  }
  }

  Bala *ListaDeBalas::operator[](size_t n) {
	  return (*(this->balas))[n];
  }

  void ListaDeBalas::operator=(const std::string& ldb_serial) {
	  this->limpaLista();
	  this->deserializaLista(ldb_serial);
  }


ListaDeTanques::ListaDeTanques() {
  this->tanques = new std::vector<Tanque *>(0);
}

ListaDeTanques::~ListaDeTanques() {
	this->limpaLista();
	delete this->tanques;
}
  void ListaDeTanques::hardCopy(ListaDeTanques *ldt) {
    std::vector<Tanque *> *tanques = ldt->getTanques();

    for (int k=0; k<tanques->size(); k++) {
      Tanque *t = new Tanque((*tanques)[k]->getPosicao(), (*tanques)[k]->getVida(),(*tanques)[k]->getBalaMax(),
                             (*tanques)[k]->getDirecao() , (*tanques)[k]->getVelocidadePadrao(), (*tanques)[k]->getId(), (*tanques)[k]->getKills(), (*tanques)[k]->getDeaths());
      this->addTanque(t);
    }
  }

  void ListaDeTanques::addTanque(Tanque *t) {
    (this->tanques)->push_back(t);
  }

  std::vector<Tanque*> *ListaDeTanques::getTanques() {
    return (this->tanques);
  }

  Tanque * ListaDeTanques::getTanque(int id){
    for(int i = 0 ; i < (this->tanques)->size() ; i++){
      if((*(this->tanques))[i]->getId() == id){
        return (*(this->tanques))[i];
      }
    }
    return NULL;
  }

  void ListaDeTanques::removeTanque(int id) {

    for(int i = 0 ; i < (this->tanques)->size() ; i++)
    {
      if((*(this->tanques))[i]->getId() == id)
      {
        delete (*(this->tanques))[i];
        (this->tanques)->erase((this->tanques)->begin() + i);
        break;
      }
    }
  }


  Bala *ListaDeTanques::comandaTanque(int id, char c) {

    Bala * bala;
    for(int i = 0 ; i < (this->tanques)->size() ; i++)
    {
      if((*(this->tanques))[i]->getId() == id)
      {
        bala = (*(this->tanques))[i]->comando(c);
        break;
      }
    }
    return bala;
  }

  void ListaDeTanques::limpaLista() {
	  for (int i = (this->tanques)->size() - 1; i >= 0; i--) {
	  	  delete (*(this->tanques))[i];
        (this->tanques)->erase((this->tanques)->begin() + i);
	  }
  }
  //Verifica tanques que morreram com base na vida. Se 0 ou negativa, morreu, "renasce" em algum lugar do mapa.
  bool ListaDeTanques::verificaTanquesMortos(int maxX , int maxY) {
    bool alguemMorreu = false;

    for (int i = 0; i < this->tanques->size(); i++) {
        if((*this)[i]->getVida() <= 0) {

            unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::minstd_rand0 geradorAleatorio(seed);
            (*(this->tanques))[i]->updatePosicao({(float) (geradorAleatorio()%maxX), (float) (geradorAleatorio()%maxY)});
            (*(this->tanques))[i]->updateVelocidade({0.0, 0.0});
            (*(this->tanques))[i]->updateVida(3);
            (*(this->tanques))[i]->updateBala(3);
            (*(this->tanques))[i]->updateDirecao('w');
            (*(this->tanques))[i]->updateDeaths((*(this->tanques))[i]->getDeaths() + 1);

            alguemMorreu = true;
        }
    }
    return alguemMorreu;
  }

  void ListaDeTanques::incrementaMunicao() {
	  for (int i = 0; i < this->tanques->size(); i++) {
		  (*(this->tanques))[i]->updateBala((*(this->tanques))[i]->getBalaAtual() + 1);
      }
  }

  /*
   * Transforma a lista em uma grande string de objetos (toString)
   */
  void ListaDeTanques::serializaLista(std::string &buffer_saida) {
	  std::string s = "";

	  for (int i = 0; i < (this->tanques)->size(); i++) {
	  	s += ((*this)[i])->toString();
	  }
	  buffer_saida = s;
  }

  /*
   * Transforma uma string de objetos (segue a formatação do toString)
   * e adiciona ao final da lista cada objeto.
   * Para reconstruir uma lista, é necessário limpá-la antes da deserialização
   * Problema: o tanque do jogador principal é perdido (nova alocação de Tanque)
   */
  void ListaDeTanques::deserializaLista(std::string buffer_entrada) {
	 Coordenada vel;
	 Coordenada pos;
	 int vida;
	 int direcao;
	 int balaAtual;
	 int balaMax;
	 float velocidadePadrao;
	 int id;
   int kills;
   int deaths;
	 char *s = (char *)buffer_entrada.c_str();

	 while 	(\
		 		sscanf(s, "%f,%f,%f,%f,%d,%d,%d,%d,%f,%d,%d,%d\n", \
				&(vel.x), &(vel.y), &(pos.x), &(pos.y), \
				&vida, &direcao, &balaAtual, &balaMax, \
				&velocidadePadrao, &id, &kills , &deaths) \
			> 0)
	{
		 for (; *s != '\n'; s++);
		 s++;
		 Tanque *novoTanque = new Tanque(pos, vida, balaMax, (char) direcao, velocidadePadrao, id, kills, deaths);
		 novoTanque->updateVelocidade(vel);
		 novoTanque->updateBala(balaAtual);
		 this->addTanque(novoTanque);
	 }
  }

  void ListaDeTanques::ordena(){
    std::sort((*tanques).begin(), (*tanques).end(), ordemRanking);
  }

  Tanque *ListaDeTanques::operator[](size_t n) {
	  return (*(this->tanques))[n];
  }

  void ListaDeTanques::operator=(const std::string& ldt_serial) {
	  this->limpaLista();
	  this->deserializaLista(ldt_serial);
  }
