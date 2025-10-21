#ifndef SEASON_H
#define SEASON_H

#include <map>
#include <string>

class Season {
 protected:
  int length;
  std::string name;
  int defaultWater;
  int dayLength;
  double dayScale;
  float growthMultiplier;
  bool canRain;
  bool canSnow;
  std::map<std::string, float> weatherProbabilities;

 public:
  // Virtual destructor to allow proper cleanup of derived classes (e.g.,
  // Summer, Winter)
  virtual ~Season() = default;

  // Returns the name of the season (e.g., "Summer", "Winter")
  virtual std::string getName() const = 0;

  // Returns how long this season lasts in days
  virtual int getLength() const = 0;

  // Returns the growth rate multiplier for crops during this season
  // (e.g., 1.0 = normal growth, >1.0 = faster, <1.0 = slower)
  virtual float getGrowthMultiplier() const = 0;

  // Indicates whether this season supports rain events
  virtual bool canHaveRain() const = 0;

  // Returns a map of possible weather conditions and their probabilities for
  // this season e.g., { {"Clear", 0.7}, {"Rain", 0.3} }
  virtual std::map<std::string, float> getWeatherProbability() const = 0;
};

#endif