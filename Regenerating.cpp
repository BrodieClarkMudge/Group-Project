#include "Regenerating.h"

Regenerating::Regenerating(float dt) : Crop(dt) {
  maxHarvests = 1;  // to be changed
  totalHarvests = 0;
}

Regenerating::~Regenerating() {}

bool Regenerating::Harvest(int& payout) {
  if (!IsMature()) return false;  // not ready, do nothing

  // If this harvest would exceed the limit, pay and remove.
  if (totalHarvests + 1 >= maxHarvests) {
    payout = GetSellPrice();
    totalHarvests++;
    return true;  // signal to remove crop
  }

  // Otherwise, pay and reset stage for regrowth
  payout = GetSellPrice();
  totalHarvests++;
  stage = SEMI1;
  return false;  // keep crop
}
