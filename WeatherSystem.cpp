
#include "WeatherSystem.h"

#include <cstdlib> // for rand()
#include <ctime> // for time()

#include "Autumn.h"
#include "Spring.h"
#include "Summer.h"
#include "Winter.h"

using namespace std;

WeatherSystem::WeatherSystem() {
  srand(time(NULL)); // Seed random generator
  currentSeason = make_unique<Summer>(); // Start in Summer
  currentDayInSeason = 0;
  generateWeather();
}

// Updates day count, regenerates weather, and handles season change
void WeatherSystem::updateDaily() {
  currentDayInSeason++;
  if (currentDayInSeason >= currentSeason->getLength()) {
    changeSeason();
  }
  generateWeather();
}

// Randomly selects weather based on the current season's probabilities
void WeatherSystem::generateWeather() {
  map<string, float> probabilities = currentSeason->getWeatherProbability();
  float roll = static_cast<float>(rand()) / RAND_MAX;
  float cumulative = 0.0f;

  for (const auto& entry : probabilities) {
    cumulative += entry.second;
    if (roll <= cumulative) {
      currentWeather = entry.first;
      return;
    }
  }

  currentWeather = "Clear"; // default
}

string WeatherSystem::getCurrentWeather() const { return currentWeather; }

Season* WeatherSystem::getCurrentSeason() const { return currentSeason.get(); }

bool WeatherSystem::isRaining() const { return currentWeather == "Rain"; }

// Moves to the next season in loop

void WeatherSystem::changeSeason() {
  string current = currentSeason->getName();
  if (current == "Summer")
    currentSeason = make_unique<Autumn>();
  else if (current == "Autumn")
    currentSeason = make_unique<Winter>();
  else if (current == "Winter")
    currentSeason = make_unique<Spring>();
  else
    currentSeason = make_unique<Summer>();

  currentDayInSeason = 0;
}

int WeatherSystem::getCurrentDay() const { return currentDayInSeason; }