#include "Pumpkin.h"

Pumpkin::Pumpkin(float dt)
    : Withering(dt) // call Withering constructor
{
    // set how long it survives after maturity before dying
    SetTimeBeforeDeath(7.0f); // 7 seconds before auto-death
}

Pumpkin::~Pumpkin() = default;

int Pumpkin::GetSellPrice() {
    return 30; // worth more
}
