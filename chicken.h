#ifndef CHICKEN_H
#define CHICKEN_H

#include "animal.h"

class Chicken : public Animal
{
private:
    Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");
    Sound chickenSound = LoadSound("assets/chickenSound.wav");

public:
    Chicken(Texture2D chickenText, Sound chickenSound);
    void updateDaily(float changeTime) override;
    void makeSound() override;
    int sellProduct() override;
};

#endif