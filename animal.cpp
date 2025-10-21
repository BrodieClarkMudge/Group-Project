#include "animal.h"
#include <sstream>
#include <iostream>

Animal::Animal(){

}

Animal::Animal(Texture2D texture, string type, string resource,
    int mHealth, int mHunger, int mThirst, int consHunger,
    int consThirst, int prodInt, Sound soundFX) {
    species = type;
    sprite = texture;
    health = mHealth;
    maxHealth = mHealth;
    maxHunger = mHunger;
    maxThirst = mThirst;
    hunger = 0;
    thirst = 100;
    hungerConsumption = consHunger;
    thirstConsumption = consThirst;
    isAliveState = true;
    producesResource = resource;
    productionInterval = prodInt;
    timeSinceFed = 0;
    timeSinceLastProduction = 0;
    hasResourceReady = false;
    soundEffect = soundFX;
}

Animal::~Animal() = default;

void Animal::calculateHealth(){
    // Drain health only when out of water
    if (thirst == 0){
        health -= 5;
    }
    // Prevent negative health
    if (health < 0){
        health = 0;
    }
}

void Animal::checkSurvival(){
    if (health <= 0){
        isAliveState = false;
    }
}

string Animal::getState() {
    std::ostringstream oss;
    oss << species
        << ", HP: " << health
        << ", Hunger: " << hunger
        << ", Thirst: " << thirst
        << (isAlive() ? ", Alive" : ", Dead");
    return oss.str();
}

void Animal::drink(){
    thirst = maxThirst;
}

bool Animal::needsWater(){
    if (thirst >= maxThirst/4) {
        return true;
    }
    else
    return false;
}

void Animal::eat(){
    if (hunger > 0) {
        hunger = hunger - hungerConsumption;
        checkSurvival();
    }
    else {
        checkSurvival();
    }
}

bool Animal::needsFood(){
    if (hunger >= maxHunger/4) {
        return true;
    }
    else
    return false;
}

int Animal::getHealth(){
    return health;
}

bool Animal::isAlive(){
    return isAliveState;
}

string Animal::getSpecies(){
    return species;
}

string Animal::getResourceType(){
    return producesResource;
}
    
void Animal::collectResource(){
    if(hasResourceReady == true) {
        //********* produced output added to storage*/
        hasResourceReady = false;
        timeSinceLastProduction = 0;
    }
}

bool Animal::hasResourceAvailable(){
    return hasResourceReady;
}

void Animal::setTexture(Texture2D tex) {
    sprite = tex;
}

void Animal::draw(float x, float y, float width, float height) {
    if (isAliveState) {
        DrawTexturePro(
            sprite,
            { 0, 0, static_cast<float>(sprite.width), static_cast<float>(sprite.height) }, // source rect
            { x, y, width, height },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
}

void Animal::updateDaily() {
    thirst -= thirstConsumption;
    if (thirst < 0) {
        thirst = 0;
    }
}

void Animal::makeSound() {
    PlaySound(soundEffect);
}

void Animal::updateSoundTimer(float timeChange) {
    if (soundCooldown > 0) {
        soundCooldown -= timeChange;
    } else {
        // Random chance to play
        if (GetRandomValue(0, 100) < 20) { // 20% chance
            makeSound();
        }
        // Reset cooldown to random between 3-10 seconds
        soundCooldown = GetRandomValue(3000, 10000) / 1000.0f;
    }
}

int Animal::getThirst() {
    return thirst;
}

void Animal::setThirst(int x) {
    if (x >= 0) {
        thirst = x;
    }
}