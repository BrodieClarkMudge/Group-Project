#include "Potato.h"

Potato::Potato(float dt)
    : Stalk(dt) {
    waterConsumption = 12;
}

Potato::~Potato() {}

int Potato::GetSellPrice() {
    return 18;
}
