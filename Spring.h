#ifndef SPRING_H
#define SPRING_H

#include "Season.h"

class Spring : public Season {
 private:
  bool bloomEventActive;

 public:
  Spring();

  std::string getName() const override;
  int getLength() const override;
  float getGrowthMultiplier() const override;
  bool canHaveRain() const override;
  std::map<std::string, float> getWeatherProbability() const override;

  // Spring only method
  void triggerBloom();  // enables bloom event
  bool isBloomActive() const;
};

#endif