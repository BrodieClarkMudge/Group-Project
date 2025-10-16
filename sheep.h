#ifndef SHEEP_H
#define SHEEP_H

#include "animal.h"

class Sheep : public Animal {
    private:
    Texture2D sheepTex = LoadTexture("assets/sheepPixel.png");

    public:
    Sheep(Texture2D sheepText);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    void updateDaily() override;
};

#endif