#ifndef CHICKEN_H
#define CHICKEN_H

#include "animal.h"

class Chicken : public Animal {
    private:
    Texture2D chickenTex = LoadTexture("assets/chickenPixel.png");

    public:
    Chicken(Texture2D chickenText);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    void updateDaily() override;
};

#endif