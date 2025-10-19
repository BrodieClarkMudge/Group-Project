#ifndef WINTER_H
#define WINTER_H

#include "Season.h"

class Winter : public Season {
 private:
  bool snowstormActive;

 public:
  Winter();

  std::string getName() const override;
  int getLength() const override;
  float getGrowthMultiplier() const override;
  float getWitheringChance() const override;
  bool canHaveRain() const override;
  std::map<std::string, float> getWeatherProbability() const override;

  // Winter-specific methods
  void triggerSnowstorm();
  bool isSnowstormActive() const;
};

#endif