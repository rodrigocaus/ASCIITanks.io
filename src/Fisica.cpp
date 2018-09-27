/*
 *  Rodrigo Oliveira Caus - 186807
 *  Victor Ferrão Santolim - 187888
 */

#include "model.hpp"
#include "Fisica.hpp"

using namespace std::chrono;

Fisica::Fisica(ListaDeTanques *ldt , ListaDeBalas *ldb, float maxX, float maxY) {
  this->ldt = ldt;
  this->ldb = ldb;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Fisica::update(float deltaT) {
  // Atualiza posicao dos tanques
  std::vector<Tanque *> *t = this->ldt->getTanques();
  for(int i = 0; i < t->size(); i++) {
    Coordenada novaPosicao;
    novaPosicao = (*t)[i]->getPosicao();
    novaPosicao.x = novaPosicao.x + ((*t)[i]->getVelocidade()).x * deltaT;
    novaPosicao.y = novaPosicao.y + ((*t)[i]->getVelocidade()).y * deltaT;

    // O tanque para ao chegar na parede
    if(novaPosicao.x > this->maxX) {
      novaPosicao.x = this->maxX;
    }
    else if(novaPosicao.x < 0) {
      novaPosicao.x = 0;
    }
    if(novaPosicao.y > this->maxY) {
        novaPosicao.y = this->maxY;
    }
    else if(novaPosicao.y < 0) {
        novaPosicao.y = 0;
    }

    //Atualiza
    (*t)[i]->updatePosicao(novaPosicao);

  }

  // Atualiza posicao das balas de acordo com a velocidade
  std::vector<Bala *> *b = this->ldb->getBalas();
  for(int i = 0; i < b->size(); i++) {
    Coordenada novaPosicao;
    novaPosicao = (*b)[i]->getPosicao();
    novaPosicao.x = novaPosicao.x + ((*b)[i]->getVelocidade()).x * deltaT;
    novaPosicao.y = novaPosicao.y + ((*b)[i]->getVelocidade()).y * deltaT;
    
    // Fora dos limites, a bala é destruida
    if(novaPosicao.x > this->maxX || novaPosicao.y > this->maxY
        || novaPosicao.x < 0 || novaPosicao.y < 0) {
        delete this->ldb->removeBala(i);
        i--;
    }
    else {
        (*b)[i]->updatePosicao(novaPosicao);
    }

  }

  // Verificação de colisoes
  // Tanques colidindo com balas
  for(int i = 0; i < t->size(); i++) {
      for(int j = 0; j < b->size(); j++){
          if(distancia((*t)[i]->getPosicao(), (*b)[j]->getPosicao()) < 1.0)
          {
              // Deletamos a bala
              delete this->ldb->removeBala(j);
              j--;
              // Decrementamos a vida do tanque
              (*t)[i]->updateVida((*t)[i]->getVida() - 1);
          }
      }
  }
  // Tanques colidindo com tanques
  for(int i = 0; i < t->size(); i++) {
      for(int j = i+1; j < t->size(); j++){
          if(distancia((*t)[i]->getPosicao(), (*t)[j]->getPosicao()) < 1.0)
          {
              // Decrementamos a vida do tanque
              (*t)[i]->updateVida((*t)[i]->getVida() - 1);
               (*t)[j]->updateVida((*t)[j]->getVida() - 1);
          }
      }
  }
  // Balas colidindo com balas
  for(int i = 0; i < b->size(); i++) {
      for(int j = i+1; j < b->size(); j++){
          if(distancia((*b)[i]->getPosicao(), (*b)[j]->getPosicao()) < 1.0)
          {
              // Deletamos as balas
              delete this->ldb->removeBala(j);
              j--;
              delete this->ldb->removeBala(i);
              i--;
              break;
          }
      }
  }
}
