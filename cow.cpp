#include "cow.h"

#include <iostream>

Cow::Cow(Texture2D cowText, Sound cowSound)
    : Animal(cowText, "Cow", "Milk", 200, 100, 100, 5, 5, 100, cowSound) {};

void Cow::updateDaily() {
  hunger += hungerConsumption;
  thirst += thirstConsumption;
  timeSinceFed++;
  timeSinceLastProduction++;

  calculateHealth();
  checkSurvival();

  if (isAlive() && timeSinceLastProduction >= productionInterval) {
    hasResourceReady = true;
  }
}

void Cow::makeSound() { PlaySound(soundEffect); }