#include "Autumn.h"

Autumn::Autumn() {
  name = "Autumn";
  length = 28;              // Slightly shorter season
  growthMultiplier = 1.0f;  // Normal growth
  witheringChance = 0.1f;   // Lower chance to wither
  canRain = true;
  canSnow = false;
  heavyRainActive = false;

  weatherProbabilities = {{"Clear", 0.5f}, {"Rain", 0.4f}, {"Fog", 0.1f}};
}

void Autumn::triggerHeavyRain() {
  heavyRainActive = true;
  weatherProbabilities["Rain"] = 0.7f;  // More likely to rain
  weatherProbabilities["Clear"] = 0.2f;
  weatherProbabilities["Fog"] = 0.1f;
}

bool Autumn::isHeavyRainActive() const { return heavyRainActive; }

std::string Autumn::getName() const { return name; }

int Autumn::getLength() const { return length; }

float Autumn::getGrowthMultiplier() const { return growthMultiplier; }

float Autumn::getWitheringChance() const { return witheringChance; }

bool Autumn::canHaveRain() const { return canRain; }

std::map<std::string, float> Autumn::getWeatherProbability() const {
  return weatherProbabilities;
}