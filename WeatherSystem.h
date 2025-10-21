#ifndef WEATHERSYSTEM_H
#define WEATHERSYSTEM_H

#include "Season.h"

#include <memory>
#include <string>
using namespace std;

class WeatherSystem {
 private:
  // Pointer to the current season (Summer, Winter)
  // Allows us to use polymorphism to call season-specific behavior
  std::unique_ptr<Season> currentSeason;
  // refernce: https://en.cppreference.com/w/cpp/memory/unique_ptr.html

  // stores the currently active weather event ("Rain", "Clear")
  // This is generated based on the current season's weather probabilities
  std::string currentWeather;

  // Tracks how many days have passed in the current season
  // Used to determine when to transition to the next season
  int currentDayInSeason;

 public:
  WeatherSystem();      
  void updateDaily();                              
  void generateWeather();  
  std::string getCurrentWeather() const;
  Season* getCurrentSeason() const;
  bool isRaining() const;
  void changeSeason();  
  int getCurrentDay() const;
};

#endif