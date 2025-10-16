#include "cow.h"
#include <iostream>

Cow::Cow(Texture2D cowText){
    Animal(cowText, "Cow", "Milk", 200, 100, 100);
};

void Cow::updateDaily() {
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