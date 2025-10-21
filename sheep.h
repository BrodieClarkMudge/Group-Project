#ifndef SHEEP_H
#define SHEEP_H

#include "animal.h"

class Sheep : public Animal {
    private:
    Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");
    Sound sheepSound = LoadSound("assets/sheepSound.wav");

    public:
    Sheep(Texture2D sheepText, Sound sheepSound);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    void updateDaily() override;
    void makeSound() override;
};

#endif