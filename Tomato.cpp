#include "Tomato.h"

Tomato::Tomato(float dt) : Regenerating(dt) {
 
  maxHarvests = 5;  
  waterConsumption = 10;
}

Tomato::~Tomato() {}

int Tomato::GetSellPrice() {
  return 15;  
}
