#ifndef PIG_H
#define PIG_H

#include "animal.h"

class Pig : public Animal {
    private:
    Texture2D pigTex = LoadTexture("assets/pigPixel.png");

    public:
    Pig(Texture2D pigText);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    //void updateDaily() override;
};

#endif