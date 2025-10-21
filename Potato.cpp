#include "Potato.h"

Potato::Potato(float dt)
    : Stalk(dt) {
    waterConsumption = 10;
}

Potato::~Potato() {}

int Potato::GetSellPrice() {
    return 120;
}