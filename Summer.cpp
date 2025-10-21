#include "Summer.h"

#include <string>
#include <vector>

Summer::Summer() {
  name = "Summer";
  length = 30;
  growthMultiplier = 1.2f;
  canRain = true;
  canSnow = false;
  isHeatwave = false;

  weatherProbabilities = {{"Clear", 0.7f}, {"Rain", 0.2f}, {"Heatwave", 0.1f}};
}

std::string Summer::getName() const { return name; }

int Summer::getLength() const { return length; }

float Summer::getGrowthMultiplier() const {
  return isHeatwave ? growthMultiplier * 1.1f : growthMultiplier;
}

bool Summer::canHaveRain() const { return canRain; }

std::map<std::string, float> Summer::getWeatherProbability() const {
  return weatherProbabilities;
}

void Summer::triggerHeatwave() { isHeatwave = true; }

bool Summer::isHeatwaveActive() const { return isHeatwave; }