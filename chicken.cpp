#include "chicken.h"
#include <iostream>

Chicken::Chicken(Texture2D chickenText)
    : Animal(chickenText, "Chicken", "Eggs", 50, 100, 100, 5, 5, 100)
{};
/*
void Chicken::updateDaily() {
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
    */