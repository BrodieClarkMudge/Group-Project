#ifndef ANIMAL_H
#define ANIMAL_H

#include <raylib.h>

#include <string>

using namespace std;

class Animal {
 protected:
  string species;
  Texture2D sprite;
  int health;
  int hunger;
  int thirst;
  int maxHealth;
  int maxHunger;
  int maxThirst;
  int waterConsumption;
  int hungerConsumption;
  bool isAliveState;
  int timeSinceFed;
  string producesResource;
  int productionInterval;
  int timeSinceLastProduction;
  bool hasResourceReady;

  Sound soundEffect;
  float soundCooldown = 0;

  void calculateHealth();
  void checkSurvival();

 public:
  Animal();
  Animal(Texture2D texture, string type, string resource, int mHealth,
         int mHunger, int mThirst, int consHunger, int consThirst, int prodInt,
         Sound soundFX);

  virtual ~Animal();

  virtual void
  updateDaily();  // ✅ ADDED THIS LINE to allow override in subclasses

  string getState();
  void drink();
  bool needsWater();
  void eat();
  bool needsFood();
  int getHealth();
  int getThirst();
  bool isAlive();

  string getSpecies();
  string getResourceType();

  void collectResource();
  bool hasResourceAvailable();

  void setTexture(Texture2D sprite);
  void draw(float x, float y, float width, float height);
  void setThirst(int);
  virtual void makeSound();
  void updateSoundTimer(float timeChange);
};

#endif  // ANIMAL_H