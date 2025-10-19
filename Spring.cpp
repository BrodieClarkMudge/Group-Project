#include"Spring.h"

Spring::Spring() {
  name = "Spring";
  length = 30;
  growthMultiplier = 1.3f;  // Already faster
  witheringChance = 0.1f;   // Pretty low
  canRain = true;
  canSnow = false;
  bloomEventActive = false;

  weatherProbabilities = {
      {"Clear", 0.5f}, {"Rain", 0.4f}, {"Bloom", 0.1f}
      // Special spring weather!
  };
}

std::string Spring::getName() const { return name; }

int Spring::getLength() const { return length; }

float Spring::getGrowthMultiplier() const {
  return bloomEventActive ? growthMultiplier + 0.2f : growthMultiplier;
}

float Spring::getWitheringChance() const { return witheringChance; }

bool Spring::canHaveRain() const { return canRain; }

std::map<std::string, float> Spring::getWeatherProbability() const {
  return weatherProbabilities;
}

void Spring::triggerBloom() { bloomEventActive = true; }

bool Spring::isBloomActive() const { return bloomEventActive; }