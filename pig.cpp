#include "pig.h"
#include <iostream>

Pig::Pig(Texture2D pigText, Sound pigSound)
    : Animal(pigText, "Pig", "Bacon", 150, 80, 100, 5, 5, 20, pigSound)
{};

void Pig::updateDaily(float changeTime)
{
    timeSinceFed += changeTime;
    timeSinceLastProduction+= changeTime;

    calculateHealth();
    checkSurvival();

    if (isAlive() && timeSinceLastProduction >= productionInterval) {
        hasResourceReady = true;
    }
}

void Pig::makeSound() {
    PlaySound(soundEffect);
}

int Pig::sellProduct() {
    if (hasResourceReady) {
        hasResourceReady = false;
        timeSinceLastProduction = 0.0f;
        return 140;
    } else {
        return 0;
    }
}