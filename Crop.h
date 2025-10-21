#ifndef CROP_H
#define CROP_H

#include <memory>
#include <string>
#include <vector>

#include "menu.h"
#include "raylib-cpp.hpp"

enum CropStage { SEED = 0, SEMI1 = 1, SEMI2 = 2, FULL = 3 };

class Crop {
 protected:
  CropStage stage; // current stage
  Texture2D stageTex[4]; //holds all textures to iterate through
  float dtPlanted; // time of creation
  // float timer;
  bool isWatered;
  int waterAmount;
  int waterConsumption;

 public:
  Crop(float dt);
  virtual ~Crop();

  void Grow(); // progress to next stage, to be called in game loop based on
                // time progressed
  virtual void Draw(Rectangle area);
  // returns true if crop is kept, false if removed
  virtual bool Harvest(int& payout) = 0;
  void Water();

  // gets and sets

  CropStage GetStage() const;
  bool IsMature() const;
  void SetTexture(CropStage s, Texture2D tex);
  virtual int GetSellPrice() = 0;

  int getWaterAmount();

  void SetWaterAmount(int x);
  int getWaterConsumption();

  virtual bool IsWithering();
};

#endif