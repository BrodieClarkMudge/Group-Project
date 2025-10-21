#include "Tomato.h"

Tomato::Tomato(float dt)
    : Regenerating(dt)
{

    maxHarvests   = 5; 
    waterConsumption = 5;
}

Tomato::~Tomato() {}

int Tomato::GetSellPrice() {
    return 15; // small frequent payout
}
