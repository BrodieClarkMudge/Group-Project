#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include <raylib.h>

using namespace std;

class Animal {
    protected:
    string species;
    Texture2D sprite;
    int health;
    int hunger;
    int thirst;
    int maxHealth;
    int maxHunger;
    int maxThirst;
    int waterConsumption;
    int hungerConsumption;
    bool isAliveState;
    int timeSinceFed;
    string producesResource;
    int productionInterval;
    int timeSinceLastProduction;
    bool hasResourceReady;

    void calculateHealth();
    void checkSurvival();

    public:
    Animal();
    Animal(string species, string resource, int maxHealth,
        int maxHunger, int maxThirst);

    virtual ~Animal() = default;

    string getState();
    void drink();
    bool needsWater();
    void eat();
    bool needsFood();
    int getHealth();
    bool isAlive();

    string getSpecies();
    string getResourceType();
    
    virtual void updateDaily();
    void collectResource();
    bool hasResourceAvailable();

};

#endif //ANIMAL_H