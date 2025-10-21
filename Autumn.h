#ifndef AUTUMN_H
#define AUTUMN_H

#include <map>
#include <string>

#include "Season.h"

class Autumn : public Season {
 private:
  bool heavyRainActive;

 public:
  Autumn();

  // Autumn-specific method
  void triggerHeavyRain();
  bool isHeavyRainActive() const;

  // Overrides
  std::string getName() const override;
  int getLength() const override;
  float getGrowthMultiplier() const override;
  bool canHaveRain() const override;
  std::map<std::string, float> getWeatherProbability() const override;
};

#endif