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
  bool canHaveRain() const override;
  std::map<std::string, float> getWeatherProbability() const override;

  //  Summer only 
  void triggerHeatwave(); 
  bool isHeatwaveActive() const;
};

#endif