#include "Withering.h"

Withering::Withering(float dt) : Crop(dt) {
    timeBeforeDeath = 0.0f; // default: never die after ripe unless changed
    ripeTimer = 0.0f;
}

bool Withering::Harvest(int& payout) {
    if (!IsMature()) return false;
    payout = GetSellPrice();
    return true; // remove after harvest

}

bool Withering::IsWithering() {return true;}
