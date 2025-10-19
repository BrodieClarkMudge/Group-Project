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
    Animal(Texture2D texture, string type, string resource,
    int mHealth, int mHunger, int mThirst, int consHunger,
    int consThirst, int prodInt);

    virtual ~Animal();

    string getState();
    void drink();
    bool needsWater();
    void eat();
    bool needsFood();
    int getHealth();
    bool isAlive();

    string getSpecies();
    string getResourceType();
    
    virtual void updateDaily() = 0;
    void collectResource();
    bool hasResourceAvailable();

    void setTexture(Texture2D sprite);
    void draw(float x, float y, float width, float height);

};

#endif //ANIMAL_H