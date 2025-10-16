#include "pig.h"
#include <iostream>

Pig::Pig(Texture2D pigText){
    Animal(pigText, "Pig", "Bacon", 150, 80, 100);
};

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