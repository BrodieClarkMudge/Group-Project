#include "cow.h"
#include <iostream>

Cow::Cow(Texture2D cowText, Sound cowSound)
    : Animal(cowText, "Cow", "Milk", 200, 100, 100, 5, 5, 20, cowSound) {};

void Cow::updateDaily(float changeTime)
{

    timeSinceFed += changeTime;
    timeSinceLastProduction += changeTime;

    calculateHealth();
    checkSurvival();

    if (isAlive() && timeSinceLastProduction >= productionInterval)
    {
        hasResourceReady = true;
    }
}

void Cow::makeSound()
{
    PlaySound(soundEffect);
}

int Cow::sellProduct() {
    if (hasResourceReady == true) {
        hasResourceReady = false;
        timeSinceLastProduction = 0.0f;
        return 60;
    } else {
        return 0;
    }
}

