#include "Large.h"

Large::Large(float dt, int w, int h)
    : Crop(dt), width(w), height(h) {
    waterConsumption = 14;
}

Large::~Large() {}

bool Large::Harvest(int& payout) {
    if (!IsMature()) return false;
    payout = GetSellPrice();
    return true;
}

int Large::GetWidth() const { return width; }
int Large::GetHeight() const { return height; }
