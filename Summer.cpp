#include "Summer.h"

Summer::Summer() {
  name = "Summer";
  length = 30;
  growthMultiplier = 1.2f;
  witheringChance = 0.3f;
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

float Summer::getWitheringChance() const {
  return isHeatwave ? witheringChance + 0.2f : witheringChance;
}

bool Summer::canHaveRain() const { return canRain; }

std::map<std::string, float> Summer::getWeatherProbability() const {
  return weatherProbabilities;
}

void Summer::triggerHeatwave() { isHeatwave = true; }

bool Summer::isHeatwaveActive() const { return isHeatwave; }