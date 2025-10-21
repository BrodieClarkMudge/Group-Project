#include "Winter.h"

Winter::Winter() {
  name = "Winter";
  length = 30;
  growthMultiplier = 0.6f;  // Slower crop growth
  canRain = false;
  canSnow = true;
  snowstormActive = false;

  weatherProbabilities = {
      {"Clear", 0.5f}, {"Snow", 0.4f}, {"Snowstorm", 0.1f}  // Winter-only event
  };
}

std::string Winter::getName() const { return name; }

int Winter::getLength() const { return length; }

float Winter::getGrowthMultiplier() const {
  return snowstormActive ? growthMultiplier - 0.2f : growthMultiplier;
}

bool Winter::canHaveRain() const { return canRain; }

std::map<std::string, float> Winter::getWeatherProbability() const {
  return weatherProbabilities;
}

void Winter::triggerSnowstorm() { snowstormActive = true; }

bool Winter::isSnowstormActive() const { return snowstormActive; }