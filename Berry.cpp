#include "Berry.h"

Berry::Berry(float dt)
    : Regenerating(dt)
{
    // tune berry behavior
    maxHarvests   = 5;  // e.g., 5 harvest cycles
    waterConsumption = 9;
}

Berry::~Berry() {}

int Berry::GetSellPrice() {
    return 6; // small frequent payout
}
