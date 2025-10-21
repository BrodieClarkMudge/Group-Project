#ifndef COW_H
#define COW_H

#include "animal.h"

class Cow : public Animal {
    private:
    Texture2D cowTex = LoadTexture("assets/cowPixel.png");
    Sound cowSound = LoadSound("assets/cowSound.wav");

    public:
    Cow(Texture2D cowText, Sound cowSound);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    void updateDaily() override;
    void makeSound() override;
};

#endif