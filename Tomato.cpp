#include "Tomato.h"

Tomato::Tomato(float dt)
    : Regenerating(dt)
{
    // tune Tomato behavior
    maxHarvests   = 5;  // e.g., 5 harvest cycles
    waterConsumption = 9;
}

Tomato::~Tomato() {}

int Tomato::GetSellPrice() {
    return 6; // small frequent payout
}
