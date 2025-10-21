#include "Potato.h"

Potato::Potato(float dt)
    : Stalk(dt) {
    waterConsumption = 10;
    type = "PUMPKIN";
}

Potato::~Potato() {}

int Potato::GetSellPrice() {
    return 120;
}