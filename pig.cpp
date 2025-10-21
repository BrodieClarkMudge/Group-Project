#include "pig.h"
#include <iostream>

Pig::Pig(Texture2D pigText, Sound pigSound)
    : Animal(pigText, "Pig", "Bacon", 150, 80, 100, 5, 5, 100, pigSound)
{};

void Pig::updateDaily() {
    hunger += hungerConsumption;
    thirst += waterConsumption;
    timeSinceFed++;
    timeSinceLastProduction++;

    calculateHealth();
    checkSurvival();

    if (isAlive() && timeSinceLastProduction >= productionInterval) {
        hasResourceReady = true;
    }
}

void Pig::makeSound() {
    PlaySound(soundEffect);
}