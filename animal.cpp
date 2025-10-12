#include "animal.h"
#include <iostream>

Animal::Animal(){

}

Animal::Animal(string species, string resource, int maxHealth, int maxHunger, int maxthirst) {
    species = species;
    sprite = LoadTexture("assets/cowPixel.png");
    health = maxHealth;
    hunger = maxHunger;
    thirst = maxThirst;
    maxHealth = maxHealth;
    maxHunger = maxHunger;
    maxThirst = maxThirst;
    waterConsumption = 10;
    hungerConsumption = 10;
    isAliveState = true;
    timeSinceFed = 0;
    resource = producesResource;
    productionInterval = 10;
    timeSinceLastProduction = 0;
    hasResourceReady = false;

}

void Animal::calculateHealth(){
    if (hunger = 0){
        health = health - 5;
    }
    if (thirst = 0){
        health = health - 5;
    }
    if (health > 0){
        health = 0;
    }
};

void Animal::checkSurvival(){
    if (health <= 0){
        isAliveState = false;
    }
};

string Animal::getState() {
    return species, ", HP: ", health, ", Hunger: ", hunger, ", Thirst: ",
    thirst, (isAlive ? ", Alive" : ", Dead");
};

void Animal::drink(){
    if (thirst > 0) {
        thirst = thirst - waterConsumption;
        checkSurvival();
    }
    else {
        checkSurvival();
    }
};

bool Animal::needsWater(){
    if (thirst >= maxThirst/4) {
        return true;
    }
    else
    return false;
};

void Animal::eat(){
        if (hunger > 0) {
        hunger = hunger - hungerConsumption;
        checkSurvival();
    }
    else {
        checkSurvival();
    }
};

bool Animal::needsFood(){
    if (hunger >= maxHunger/4) {
        return true;
    }
    else
    return false;
};

int Animal::getHealth(){
    return health;
};

bool Animal::isAlive(){
    return isAliveState;
};

string Animal::getSpecies(){
    return species;
};

string Animal::getResourceType(){
    return producesResource;
};
    
void Animal::collectResource(){
    if(hasResourceReady = true) {
        //********* produced output added to storage*/
        hasResourceReady = false;
        timeSinceLastProduction = 0;
    }
};

bool Animal::hasResourceAvailable(){
    return hasResourceReady;
};
