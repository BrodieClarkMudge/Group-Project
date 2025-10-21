#include "Tomato.h"

Tomato::Tomato(float dt) : Regenerating(dt) {
  // tune Tomato behavior
  maxHarvests = 5;  // e.g., 5 harvest cycles
  waterConsumption = 10;
}

Tomato::~Tomato() {}

int Tomato::GetSellPrice() {
  return 15;  // small frequent payout
}
