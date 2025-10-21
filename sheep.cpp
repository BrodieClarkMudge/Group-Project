#include "sheep.h"

#include <iostream>

Sheep::Sheep(Texture2D sheepText, Sound sheepSound)
    : Animal(sheepText, "Sheep", "Wool", 100, 80, 100, 5, 5, 100, sheepSound) {
      };

void Sheep::updateDaily() {
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

void Sheep::makeSound() { PlaySound(soundEffect); }