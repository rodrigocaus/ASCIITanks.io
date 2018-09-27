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

Tanque::Tanque(Coordenada posicao, int vida, int balaMax, char direcao, float velocidadePadrao, bool timeInimigo) {
  this->velocidade = {0.0, 0.0};
  this->posicao = posicao;
  this->vida = vida;
  this->balaAtual = 100;
  this->balaMax = balaMax;
  this->direcao = direcao;
  this->velocidadePadrao = velocidadePadrao;
  this->timeInimigo = timeInimigo;
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

  Coordenada Tanque::getVelocidade(){
    return velocidade;
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

  bool Tanque::getTime() {
	return timeInimigo;
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
              b = new Bala(this->direcao , posBala);

          }
          break;
        default:
          this->updateVelocidade({0.0, 0.0});
          break;

      }
      return b;
  }

Bala::Bala(Coordenada velocidade, Coordenada posicao) {
  this->velocidade = velocidade;
  this->posicao = posicao;
}

//Segunda opção de construtor: Polar (posição e direção ao invés de posição e velocidade)
Bala::Bala(char direcao, Coordenada posicao , float velocidadePadrao) {
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


ListaDeBalas::ListaDeBalas() {
  this->balas = new std::vector<Bala *>(0);
}

  void ListaDeBalas::hardCopy(ListaDeBalas *ldb) {
    std::vector<Bala *> *balas = ldb->getBalas();

    for (int k=0; k<balas->size(); k++) {
      Bala *b = new Bala((*balas)[k]->getVelocidade(), (*balas)[k]->getPosicao());
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

  std::vector<Bala*> *ListaDeBalas::getBalas() {
    return (this->balas);
  }


ListaDeTanques::ListaDeTanques() {
  this->tanques = new std::vector<Tanque *>(0);
}

  void ListaDeTanques::hardCopy(ListaDeTanques *ldt) {
    std::vector<Tanque *> *tanques = ldt->getTanques();

    for (int k=0; k<tanques->size(); k++) {
      Tanque *t = new Tanque((*tanques)[k]->getPosicao(), (*tanques)[k]->getVida(),(*tanques)[k]->getBalaMax(), (*tanques)[k]->getDirecao());
      this->addTanque(t);
    }
  }

  void ListaDeTanques::addTanque(Tanque *t) {
    (this->tanques)->push_back(t);
  }

  std::vector<Tanque*> *ListaDeTanques::getTanques() {
    return (this->tanques);
  }

  Tanque *ListaDeTanques::removeTanque(int index) {
    Tanque *t = (*(this->tanques))[index];
    (this->tanques)->erase((this->tanques)->begin() + index);
    return t;
  }

  //Verifica tanques que morreram com base na vida. Se 0 ou negativa, morreu, e deleta o tanque.
  bool ListaDeTanques::verificaTanquesMortos() {
    bool alguemMorreu = false;
    for (int i = 0; i < this->tanques->size(); i++) {
        if((*(this->tanques))[i]->getVida() <= 0) {
            this->removeTanque(i);
            i--;
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
