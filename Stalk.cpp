

#include <vector>
#include <string>
#include <memory>
#include "raylib-cpp.hpp"
#include "menu.h"
#include "Crop.h"
#include "Stalk.h"




Stalk::Stalk(float dt)

    : Crop(dt) {// calls base constructer
    waterConsumption = 8;
}

Stalk::~Stalk() {}

bool Stalk::Harvest(int& payout) {
    // if mature, harvest and add money
    if (!IsMature()) return false; 
    payout = GetSellPrice();
    return true;
}
