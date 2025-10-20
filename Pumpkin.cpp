#include "Pumpkin.h"

Pumpkin::Pumpkin(float dt)
    : Large(dt, 2, 2) {}

Pumpkin::~Pumpkin() {}

int Pumpkin::GetSellPrice() {
    return 30;
}
