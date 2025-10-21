#ifndef STALK_H
#define STALK_H

#include <memory>
#include <string>
#include <vector>

#include "Crop.h"
#include "menu.h"
#include "raylib-cpp.hpp"

class Stalk : public Crop {
 public:
  Stalk(float dt);
  ~Stalk();

  bool Harvest(int& payout) override;
};

#endif