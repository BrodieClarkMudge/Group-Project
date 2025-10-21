#include "sheep.h"
#include <iostream>

Sheep::Sheep(Texture2D sheepText, Sound sheepSound)
    : Animal(sheepText, "Sheep", "Wool", 100, 80, 100, 5, 5, 100, sheepSound)
{};

void Sheep::updateDaily(float changeTime)
{
    hunger += hungerConsumption;
    thirst += thirstConsumption;
    timeSinceFed += changeTime;
    timeSinceLastProduction+= changeTime;

    calculateHealth();
    checkSurvival();

    if (isAlive() && timeSinceLastProduction >= productionInterval) {
        hasResourceReady = true;
    }
}

void Sheep::makeSound() {
    PlaySound(soundEffect);
}

int Sheep::sellProduct() {
    if (hasResourceReady) {
        hasResourceReady = false;
        timeSinceLastProduction = 0.0f;
        return 110;
    } else {
        return 0;
    }
}