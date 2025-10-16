#include "sheep.h"
#include <iostream>

Sheep::Sheep(Texture2D sheepText){
    Animal(sheepText, "Sheep", "Wool", 100, 80, 100);
};

void Sheep::updateDaily() {
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