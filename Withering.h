#ifndef WITHERING_H
#define WITHERING_H

#include "Crop.h"

class Withering : public Crop {
 protected:
  float timeBeforeDeath; // time after FULL before it dies
  float ripeTimer; // tracks how long it's been FULL

 public:
  Withering(float dt);

  // sets & gets
  void SetTimeBeforeDeath(float t) { timeBeforeDeath = t; }
  float GetTimeBeforeDeath() const { return timeBeforeDeath; }

  bool Harvest(int& payout) override;

  bool IsWithering() override;
};

#endif
