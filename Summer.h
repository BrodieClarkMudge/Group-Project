#ifndef SUMMER_H
#define SUMMER_H

#include "Season.h"

class Summer : public Season {
 private:
  bool isHeatwave;

 public:
  Summer();

  std::string getName() const override;
  int getLength() const override;
  float getGrowthMultiplier() const override;
  float getWitheringChance() const override;
  bool canHaveRain() const override;
  std::map<std::string, float> getWeatherProbability() const override;

  // Unique Summer-only logic
  void triggerHeatwave();  // changes internal state
  bool isHeatwaveActive() const;
};

#endif