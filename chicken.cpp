#include "chicken.h"
#include <iostream>

Chicken::Chicken(Texture2D chickenText, Sound chickenSound)
    : Animal(chickenText, "Chicken", "Eggs", 50, 100, 100, 5, 5, 100, chickenSound)
{};
void Chicken::updateDaily() {
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

void Chicken::makeSound() {
    PlaySound(soundEffect);
}