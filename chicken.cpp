#include "chicken.h"
#include <iostream>

Chicken::Chicken(Texture2D chickenText, Sound chickenSound)
    : Animal(chickenText, "Chicken", "Eggs", 50, 100, 100, 5, 5, 100, chickenSound) {};

    void Chicken::updateDaily(float changeTime)
{
    hunger += hungerConsumption;
    thirst += thirstConsumption;
    timeSinceFed += changeTime;
    timeSinceLastProduction+= changeTime;

    calculateHealth();
    checkSurvival();

    if (isAlive() && timeSinceLastProduction >= productionInterval)
    {
        hasResourceReady = true;
    }
}

void Chicken::makeSound()
{
    PlaySound(soundEffect);
}

int Chicken::sellProduct() {
    if (hasResourceReady) {
        hasResourceReady = false;
        timeSinceLastProduction = 0.0f;
        return 50;
    } else {
        return 0;
    }
}