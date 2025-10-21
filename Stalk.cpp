

#include "Stalk.h"

#include <memory>
#include <string>
#include <vector>

#include "Crop.h"
#include "menu.h"
#include "raylib-cpp.hpp"

Stalk::Stalk(float dt)

    : Crop(dt) {  // calls base constructer
  waterConsumption = 8;
}

Stalk::~Stalk() {}

bool Stalk::Harvest(int& payout) {
  // if mature, harvest and add money
  if (!IsMature()) return false;
  payout = GetSellPrice();
  return true;
}
